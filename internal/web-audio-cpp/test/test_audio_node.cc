#include <gtest/gtest.h>

#include "dummy_node.h"
#include "web_audio/audio_node.h"
#include "web_audio/offline_audio_context.h"

namespace {
std::shared_ptr<web_audio::OfflineAudioContext> createOfflineContext() {
  return web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
}
} // namespace

TEST(AudioNodeTest, ConnectNode) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto connectedNode = node1->connect(node2, 0, 0);
  EXPECT_EQ(connectedNode, node2);
  EXPECT_NO_THROW(node1->disconnect(node2, 0, 0));
}

TEST(AudioNodeTest, ConnectNodeDifferentContext) {
  auto context1 = createOfflineContext();
  auto context2 = createOfflineContext();
  auto node1 = DummyNode::create(context1);
  auto node2 = DummyNode::create(context2);
  EXPECT_THROW(node1->connect(node2, 0, 0), web_audio::DOMException);
}

TEST(AudioNodeTest, ConnectNodeInvalidOutput) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  EXPECT_THROW(node1->connect(node2, 2, 0), web_audio::DOMException);
}

TEST(AudioNodeTest, ConnectNodeInvalidInput) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  EXPECT_THROW(node1->connect(node2, 0, 2), web_audio::DOMException);
}

TEST(AudioNodeTest, ConnectParam) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto param = node2->param_;
  EXPECT_NO_THROW(node1->connect(param, 0));
  EXPECT_NO_THROW(node1->disconnect(param, 0));
}