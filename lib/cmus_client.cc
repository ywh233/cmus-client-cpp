//******************************************
//  Author : Yuwei Huang   
//  Created On : Fri Feb 16 2018
//  File : cmus_client.cc
//******************************************

#include "cmus_client.h"

#include <asio.hpp>

#include<iostream>

namespace cmusclient {

CmusClient::CmusClient(std::unique_ptr<ConnectionInterface>&& interface) {
  interface_ = std::move(interface);
}

CmusClient::~CmusClient() {}

bool CmusClient::IsAuthenticated() {
  std::string reply = SendCommandAndGetReply("status");
  return reply.find("authentication failed") != 0;
}

void CmusClient::SetPassword(const std::string& password) {
  ProcessCommand("passwd " + password);
}

Status CmusClient::GetStatus() {
  return Status::ParseStatus(SendCommandAndGetReply("status"));
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
  return interface_->Receive();
}

void CmusClient::ProcessCommand(const std::string& command) {
  std::string result = SendCommandAndGetReply(command);
  if (!result.empty()) {
    std::cerr << result << std::endl;
  }
}

}  // namespace cmusclient
