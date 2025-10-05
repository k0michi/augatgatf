#include "web_audio/details/node_graph.h"

#include "web_audio/audio_param.h"

namespace web_audio::details {
void AudioGraph::addNode(std::shared_ptr<AudioNode> node) {
  if (!hasNode(node)) {
    nodes_.push_back(node);
  }
}

void AudioGraph::removeNode(std::shared_ptr<AudioNode> node) {
  nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), node), nodes_.end());
}

bool AudioGraph::hasNode(std::shared_ptr<AudioNode> node) const {
  return std::find(nodes_.begin(), nodes_.end(), node) != nodes_.end();
}

void AudioGraph::clear() { nodes_.clear(); }

std::vector<
    std::variant<std::shared_ptr<AudioNode>, std::shared_ptr<AudioListener>>>
AudioGraph::getNextNodes(std::shared_ptr<AudioNode> node) const {
  std::vector<
      std::variant<std::shared_ptr<AudioNode>, std::shared_ptr<AudioListener>>>
      nextNodes;

  for (const auto &output : node->outputs_) {
    auto destination = output.destination;

    if (auto destNode = std::get_if<std::weak_ptr<AudioNode>>(&destination)) {
      if (auto sharedDestNode = destNode->lock()) {
        nextNodes.push_back(sharedDestNode);
      }
    } else if (auto destParam =
                   std::get_if<std::weak_ptr<AudioParam>>(&destination)) {
      if (auto sharedDestParam = destParam->lock()) {
        auto owner = sharedDestParam->getOwner();

        if (auto ownerNode = std::get_if<std::shared_ptr<AudioNode>>(&owner)) {
          nextNodes.push_back(*ownerNode);
        } else if (auto ownerListener =
                       std::get_if<std::shared_ptr<AudioListener>>(&owner)) {
          nextNodes.push_back(*ownerListener);
        }
      }
    }
  }

  return nextNodes;
}

bool AudioGraph::isPartOfCycle(std::shared_ptr<AudioNode> node) const {
  std::unordered_map<std::shared_ptr<AudioNode>, std::uint32_t> state;

  std::function<bool(std::shared_ptr<AudioNode>)> dfs =
      [&](std::shared_ptr<AudioNode> current) {
        state[current] |= 1;

        for (const auto &next : getNextNodes(current)) {
          if (auto nextNode = std::get_if<std::shared_ptr<AudioNode>>(&next)) {
            if (state[*nextNode] & 1) {
              return true;
            } else if (!(state[*nextNode] & 2)) {
              if (dfs(*nextNode)) {
                return true;
              }
            }
          }
        }

        state[current] |= 2;
        return false;
      };

  return dfs(node);
}
} // namespace web_audio::details