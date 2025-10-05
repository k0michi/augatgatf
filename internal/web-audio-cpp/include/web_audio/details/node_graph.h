#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "../audio_listener.h"
#include "../audio_node.h"

namespace web_audio::details {
class AudioGraph {
public:
  AudioGraph() = default;
  virtual ~AudioGraph() noexcept = default;

  void addNode(std::shared_ptr<AudioNode> node);

  void removeNode(std::shared_ptr<AudioNode> node);

  bool hasNode(std::shared_ptr<AudioNode> node) const;

  void clear();

  /**
   * Returns the destination nodes connected to this node.
   */
  std::vector<
      std::variant<std::shared_ptr<AudioNode>, std::shared_ptr<AudioListener>>>
  getNextNodes(std::shared_ptr<AudioNode> node) const;

  bool isPartOfCycle(std::shared_ptr<AudioNode> node) const;

private:
  std::vector<std::shared_ptr<AudioNode>> nodes_;
  std::shared_ptr<AudioListener> listener_;
};
} // namespace web_audio::details