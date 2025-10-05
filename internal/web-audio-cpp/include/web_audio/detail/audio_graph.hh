#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "../audio_destination_node.hh"
#include "../audio_listener.hh"
#include "../audio_node.hh"
#include "audio_listener_node.hh"
#include "common.hh"

namespace web_audio::detail {
class AudioGraph {
public:
  AudioGraph() = default;
  virtual ~AudioGraph() noexcept = default;

  void initialize(std::shared_ptr<BaseAudioContext> context);

  void addNode(std::shared_ptr<AudioNode> node);

  void removeNode(std::shared_ptr<AudioNode> node);

  bool hasNode(std::shared_ptr<AudioNode> node) const;

  void clear();

  /**
   * Returns the destination nodes connected to this node.
   */
  std::vector<std::shared_ptr<AudioNode>>
  getNextVertices(std::shared_ptr<AudioNode> node) const;

  std::vector<std::shared_ptr<AudioNode>>
  getPreviousVertices(std::shared_ptr<AudioNode> node) const;

  std::vector<std::shared_ptr<AudioNode>> getVertices() const;

  /**
   * Returns true if the given node is part of a cycle.
   */
  bool isPartOfCycle(std::shared_ptr<AudioNode> node) const;

  /**
   * Returns the strongly connected components of the graph.
   */
  std::vector<std::vector<std::shared_ptr<AudioNode>>>
  getStronglyConnectedComponents() const;

  std::shared_ptr<AudioListenerNode> getListenerNode() const;
  std::shared_ptr<AudioDestinationNode> getDestinationNode() const;

  WEB_AUDIO_PRIVATE : std::vector<std::shared_ptr<AudioNode>> nodes_;
  std::shared_ptr<detail::AudioListenerNode> listenerNode_;
  std::shared_ptr<AudioDestinationNode> destinationNode_;
};
} // namespace web_audio::detail