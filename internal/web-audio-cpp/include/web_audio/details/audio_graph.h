#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "../audio_listener.h"
#include "../audio_node.h"
#include "common.h"

namespace web_audio::details {
class AudioGraph {
public:
  using Vertex =
      std::variant<std::shared_ptr<AudioNode>, std::shared_ptr<AudioListener>>;

  AudioGraph() = default;
  virtual ~AudioGraph() noexcept = default;

  void addNode(std::shared_ptr<AudioNode> node);

  void removeNode(std::shared_ptr<AudioNode> node);

  bool hasNode(std::shared_ptr<AudioNode> node) const;

  void clear();

  /**
   * Returns the destination nodes connected to this node.
   */
  std::vector<Vertex> getNextVertices(std::shared_ptr<AudioNode> node) const;

  std::vector<Vertex> getNextVertices(Vertex vertex) const;

  std::vector<Vertex> getPreviousVertices(Vertex vertex) const;

  std::vector<Vertex> getVertices() const;

  /**
   * Returns true if the given node is part of a cycle.
   */
  bool isPartOfCycle(std::shared_ptr<AudioNode> node) const;

  /**
   * Returns the strongly connected components of the graph.
   */
  std::vector<std::vector<Vertex>> getStronglyConnectedComponents() const;

  WEB_AUDIO_PRIVATE : std::vector<std::shared_ptr<AudioNode>> nodes_;
  std::shared_ptr<AudioListener> listener_;
};
} // namespace web_audio::details