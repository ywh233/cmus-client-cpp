//******************************************
//  Author : Author
//  Created On : Mon Feb 19 2018
//  File : cmus_client_unittests.cc
//******************************************

#include "lib/cmus_client.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lib/connection_interface.h"
#include "lib/exceptions.h"

namespace cmusclient {

using testing::Return;

const char kAuthenticationFailedText[] = "authentication failed\n";

const char kStatusText[] = R"(
status playing
file /Media/Music/Songs from a Secret Garden/05 Papillon.mp3
duration 208
position 4
tag tracknumber 5
tag title Papillon
tag artist Secret Garden
tag album Songs from a Secret Garden
tag genre Classical
tag comment Other
set aaa_mode all
set continue true
set play_library true
set play_sorted true
set replaygain disabled
set replaygain_limit true
set replaygain_preamp 0.000000
set repeat false
set repeat_current false
set shuffle false
set softvol false
set vol_left 100
set vol_right 100)";

const char kListText[] = R"(
file /Media/Music/Alice in Wonderworld/mugs feat.630.mp3
duration 318
codec mp3
bitrate 329999
tag tracknumber 2
tag title mugs feat.630
tag artist 古川本舗
tag album Alice in Wonderword
tag comment Other
file /Media/Music/FZ- Side Z (FEZ) (2013) [FLAC]/05 - Pressure.flac
duration 210
codec flac
bitrate 988599
tag album FZ: Side Z
tag albumartist Various Artists
tag artist bignic
tag date 2013
tag title Pressure
tag tracknumber 5)";

class MockConnectionInterface : public ConnectionInterface {
 public:
  MOCK_METHOD1(Send, void(const std::string&));
  MOCK_METHOD0(Receive, std::string());
};

class CmusClientTest : public testing::Test {
 public:
  void SetUp() override;

 protected:
  void ExpectSendAndReply(const std::string& input, const std::string& reply);

  std::unique_ptr<MockConnectionInterface> owned_interface_;

 private:
  MockConnectionInterface* interface_;
};

void CmusClientTest::SetUp() {
  owned_interface_.reset(new MockConnectionInterface());
  interface_ = owned_interface_.get();
}

void CmusClientTest::ExpectSendAndReply(const std::string& input,
                                        const std::string& reply) {
  testing::Sequence sequence;
  EXPECT_CALL(*interface_, Send(input))
      .Times(1)
      .InSequence(sequence);
  EXPECT_CALL(*interface_, Receive())
      .InSequence(sequence)
      .WillOnce(Return(reply));
}

TEST_F(CmusClientTest, ConstructWithoutPassword_queryStatus) {
  ExpectSendAndReply("status\n", kStatusText);
  CmusClient client(std::move(owned_interface_), "");
}

TEST_F(CmusClientTest, ConstructWithPassword_sendPasswd) {
  ExpectSendAndReply("passwd 123456\n", "\n");
  CmusClient client(std::move(owned_interface_), "123456");
}

TEST_F(CmusClientTest, ConstructWithoutPassword_authenticationError) {
  ExpectSendAndReply("status\n", kAuthenticationFailedText);
  try {
    CmusClient client(std::move(owned_interface_), "");
    FAIL() << "Should throw authentication error.";
  } catch (const AuthenticationError& err) {
    // OK
  }
}

TEST_F(CmusClientTest, ConstructWithIncorrectPassword_authenticationError) {
  ExpectSendAndReply("passwd 123456\n", kAuthenticationFailedText);
  try {
    CmusClient client(std::move(owned_interface_), "123456");
    FAIL() << "Should throw authentication error.";
  } catch (const AuthenticationError& err) {
    // OK
  }
}

TEST_F(CmusClientTest, SetValue_succeeds) {
  ExpectSendAndReply("status\n", kStatusText);
  CmusClient client(std::move(owned_interface_), "");

  ExpectSendAndReply("set repeat=false\n", "\n");
  client.SetValue("repeat", "false");
}

TEST_F(CmusClientTest, SetValue_commandError) {
  ExpectSendAndReply("status\n", kStatusText);
  CmusClient client(std::move(owned_interface_), "");

  ExpectSendAndReply("set repeat=123\n",
                     "Error: name or integer in range 0..1 expected\n");
  try {
    client.SetValue("repeat", "123");
    FAIL() << "Should throw command error.";
  } catch (const CommandError& err) {
    // OK
  }
}

TEST_F(CmusClientTest, GetStatus_parsedProperly) {
  ExpectSendAndReply("status\n", kStatusText);
  CmusClient client(std::move(owned_interface_), "");

  ExpectSendAndReply("status\n", kStatusText);
  Status status = client.GetStatus();

  EXPECT_EQ(Status::PlayerStatus::PLAYING, status.status);
  EXPECT_EQ(208, status.duration);
  EXPECT_EQ(4, status.position);

  EXPECT_EQ("5", status.tags.tracknumber);
  EXPECT_EQ("Papillon", status.tags.title);
  EXPECT_EQ("Secret Garden", status.tags.artist);
  EXPECT_EQ("Songs from a Secret Garden", status.tags.album);
  EXPECT_EQ("Classical", status.tags.genre);
  EXPECT_EQ("Other", status.tags.comment);

  EXPECT_EQ("all", status.settings["aaa_mode"]);
  EXPECT_EQ("true", status.settings["continue"]);
  EXPECT_EQ("true", status.settings["play_library"]);
  EXPECT_EQ("0.000000", status.settings["replaygain_preamp"]);
  EXPECT_EQ("100", status.settings["vol_left"]);
}

TEST_F(CmusClientTest, GetList_empty) {
  ExpectSendAndReply("status\n", kStatusText);
  CmusClient client(std::move(owned_interface_), "");

  ExpectSendAndReply("save -l -e -\n", "");
  std::vector<Metadata> metadata_list =
      client.GetMetadataList(CmusClient::MetadataListSource::LIBRARY);

  EXPECT_EQ(0u, metadata_list.size());
}

TEST_F(CmusClientTest, GetList_returnsTwoRecords) {
  ExpectSendAndReply("status\n", kStatusText);
  CmusClient client(std::move(owned_interface_), "");

  ExpectSendAndReply("save -l -e -\n", kListText);
  std::vector<Metadata> metadata_list =
      client.GetMetadataList(CmusClient::MetadataListSource::LIBRARY);

  EXPECT_EQ(2u, metadata_list.size());
  EXPECT_EQ(318, metadata_list[0].duration);
  EXPECT_EQ("mp3", metadata_list[0].codec);
  EXPECT_EQ(329999, metadata_list[0].bitrate);
  EXPECT_EQ("2", metadata_list[0].tags.tracknumber);

  EXPECT_EQ(210, metadata_list[1].duration);
  EXPECT_EQ("flac", metadata_list[1].codec);
  EXPECT_EQ(988599, metadata_list[1].bitrate);
  EXPECT_EQ("Pressure", metadata_list[1].tags.title);
  EXPECT_EQ("bignic", metadata_list[1].tags.artist);
}

}  // namespace cmusclient
