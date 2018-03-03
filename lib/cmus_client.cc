//******************************************
//  Author : Yuwei Huang
//  Created On : Fri Feb 16 2018
//  File : cmus_client.cc
//******************************************

#include "cmus_client.h"

#include <assert.h>
#include <asio.hpp>

#include <iostream>

#include "exceptions.h"
#include "ip_connection_interface.h"
#include "unix_connection_interface.h"

namespace cmusclient {

CmusClient::CmusClient()
    : CmusClient(std::unique_ptr<ConnectionInterface>(
        new UnixConnectionInterface()), "") {}

CmusClient::CmusClient(const std::string& hostname,
                       const std::string& port,
                       const std::string& password)
    : CmusClient(
          std::unique_ptr<ConnectionInterface>(
              new IpConnectionInterface(hostname, port)),
          password) {}

CmusClient::CmusClient(std::unique_ptr<ConnectionInterface>&& interface,
                       const std::string& password)
    : interface_(std::move(interface)) {
  assert (interface_);

  std::string check_authentication_reply = SendCommandAndGetReply(
      password.empty() ? "status" : "passwd " + password);
  if (check_authentication_reply.find("authentication failed") == 0) {
    throw AuthenticationError();
  }
}

CmusClient::~CmusClient() {}

Status CmusClient::GetStatus() {
  return Status::ParseStatus(SendCommandAndGetReply("status"));
}

void CmusClient::GoToView(View view) {
  ProcessCommand("view " + std::to_string(static_cast<unsigned int>(view)));
}

std::vector<Metadata> CmusClient::GetMetadataList(MetadataListSource source) {
  std::string command = "save ";
  switch (source) {
    case MetadataListSource::LIBRARY:
      command += "-l";
      break;
    case MetadataListSource::FILTERED_LIBRARY:
      command += "-L";
    case MetadataListSource::PLAYLIST:
      command += "-p";
      break;
    case MetadataListSource::QUEUE:
      command += "-q";
      break;
    default:
      assert(false);
      return {};
  }
  command += " -e -";
  return Metadata::ParseMetadataList(SendCommandAndGetReply(command));
}

void CmusClient::Play() {
  ProcessCommand("player-play");
}

void CmusClient::Pause() {
  ProcessCommand("player-pause");
}

void CmusClient::Stop() {
  ProcessCommand("player-stop");
}

void CmusClient::Next() {
  ProcessCommand("player-next");
}

void CmusClient::Previous() {
  ProcessCommand("player-prev");
}

void CmusClient::Search(const std::string& str) {
  ProcessCommand("/" + str);
}

void CmusClient::Activate() {
  ProcessCommand("win-activate");
}

void CmusClient::Seek(const std::string& position) {
  ProcessCommand("seek " + position);
}

void CmusClient::SetValue(const std::string& key, const std::string& value) {
  ProcessCommand("set " + key + "=" + value);
}

void CmusClient::ToggleValue(const std::string& key) {
  ProcessCommand("toggle " + key);
}

void CmusClient::SetVolume(const std::string& volume) {
  ProcessCommand("vol " + volume);
}

// private

std::string CmusClient::SendCommandAndGetReply(const std::string& command) {
  interface_->Send(command + '\n');
  std::string reply = interface_->Receive();
  static const std::string error_prefix = "Error: ";
  if (reply.find(error_prefix) == 0) {
    throw CommandError(reply.substr(error_prefix.length()));
  }
  return reply;
}

void CmusClient::ProcessCommand(const std::string& command) {
  SendCommandAndGetReply(command);
}

}  // namespace cmusclient
