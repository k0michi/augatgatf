#include <gtest/gtest.h>

#include "dummy_node.hh"
#include "web_audio.hh"

namespace {
std::shared_ptr<web_audio::OfflineAudioContext> createOfflineContext() {
  return web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
}
} // namespace

TEST(NodeGraph, IsPartOfCycleSimple) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);

  node1->connect(node2);
  node2->connect(node1);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);
  graph->addNode(node2);

  EXPECT_TRUE(graph->isPartOfCycle(node1));
  EXPECT_TRUE(graph->isPartOfCycle(node2));
}

TEST(NodeGraph, IsPartOfCycleNoCycle) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto node3 = DummyNode::create(context);

  node1->connect(node2);
  node2->connect(node3);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);
  graph->addNode(node2);
  graph->addNode(node3);

  EXPECT_FALSE(graph->isPartOfCycle(node1));
  EXPECT_FALSE(graph->isPartOfCycle(node2));
  EXPECT_FALSE(graph->isPartOfCycle(node3));
}

TEST(NodeGraph, IsPartOfCycleComplex) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto node3 = DummyNode::create(context);
  auto node4 = DummyNode::create(context);

  node1->connect(node2);
  node2->connect(node3);
  node3->connect(node4);
  node4->connect(node2);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);
  graph->addNode(node2);
  graph->addNode(node3);
  graph->addNode(node4);

  EXPECT_FALSE(graph->isPartOfCycle(node1));
  EXPECT_TRUE(graph->isPartOfCycle(node2));
  EXPECT_TRUE(graph->isPartOfCycle(node3));
  EXPECT_TRUE(graph->isPartOfCycle(node4));
}

TEST(NodeGraph, IsPartOfCycleSelfLoop) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);

  node1->connect(node1);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);

  EXPECT_TRUE(graph->isPartOfCycle(node1));
}

TEST(NodeGraph, GetNextVertices) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto node3 = DummyNode::create(context);

  node1->connect(node2);
  node1->connect(node3);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);
  graph->addNode(node2);
  graph->addNode(node3);

  auto next = graph->getNextVertices(
      static_cast<std::shared_ptr<web_audio::AudioNode>>(node1));
  EXPECT_EQ(next.size(), 2);
  EXPECT_EQ(next[0], node2);
  EXPECT_EQ(next[1], node3);
}

TEST(NodeGraph, GetPreviousVertices) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto node3 = DummyNode::create(context);

  node1->connect(node2);
  node1->connect(node3);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);
  graph->addNode(node2);
  graph->addNode(node3);

  auto prev = graph->getPreviousVertices(
      static_cast<std::shared_ptr<web_audio::AudioNode>>(node2));
  EXPECT_EQ(prev.size(), 1);
  EXPECT_EQ(prev[0], node1);

  prev = graph->getPreviousVertices(
      static_cast<std::shared_ptr<web_audio::AudioNode>>(node3));
  EXPECT_EQ(prev.size(), 1);
  EXPECT_EQ(prev[0], node1);
}

TEST(NodeGraph, GetStronglyConnectedComponents) {
  auto context = createOfflineContext();
  auto node1 = DummyNode::create(context);
  auto node2 = DummyNode::create(context);
  auto node3 = DummyNode::create(context);
  auto node4 = DummyNode::create(context);

  node1->connect(node2);
  node2->connect(node3);
  node3->connect(node1);
  node4->connect(node1);

  auto graph = context->getAudioGraph();
  graph->addNode(node1);
  graph->addNode(node2);
  graph->addNode(node3);
  graph->addNode(node4);

  auto sccs = graph->getStronglyConnectedComponents();
  EXPECT_EQ(sccs.size(), 4);
  EXPECT_EQ(sccs[1], std::vector<std::shared_ptr<web_audio::AudioNode>>{node4});
  EXPECT_EQ(sccs[2], (std::vector<std::shared_ptr<web_audio::AudioNode>>{
                         node1, node3, node2}));
}