#include <gtest/gtest.h>

#include "web_audio.hh"

TEST(TestMessageQueue, PushPop) {
  web_audio::detail::MessageQueue mq;
  web_audio::detail::MessageAudioScheduledSourceNodeStart msg1{0.0, 0.0, 1.0,
                                                               nullptr};
  web_audio::detail::MessageAudioScheduledSourceNodeStop msg2{1.0, nullptr};

  mq.push(msg1);
  mq.push(msg2);

  auto popped1 = mq.pop();
  EXPECT_TRUE(
      std::holds_alternative<
          web_audio::detail::MessageAudioScheduledSourceNodeStart>(popped1));
  auto &startMsg =
      std::get<web_audio::detail::MessageAudioScheduledSourceNodeStart>(
          popped1);
  EXPECT_EQ(startMsg.when, 0.0);
  EXPECT_EQ(startMsg.offset, 0.0);
  EXPECT_EQ(startMsg.duration, 1.0);
  EXPECT_EQ(startMsg.node, nullptr);

  auto popped2 = mq.pop();
  EXPECT_TRUE(std::holds_alternative<
              web_audio::detail::MessageAudioScheduledSourceNodeStop>(popped2));
  auto &stopMsg =
      std::get<web_audio::detail::MessageAudioScheduledSourceNodeStop>(popped2);
  EXPECT_EQ(stopMsg.when, 1.0);
  EXPECT_EQ(stopMsg.node, nullptr);
}