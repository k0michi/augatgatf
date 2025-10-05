#include "web_audio/details/audio_graph.h"

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

std::vector<AudioGraph::Vertex>
AudioGraph::getNextVertices(std::shared_ptr<AudioNode> node) const {
  std::vector<Vertex> nextNodes;

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

std::vector<AudioGraph::Vertex>
AudioGraph::getNextVertices(Vertex vertex) const {
  if (auto node = std::get_if<std::shared_ptr<AudioNode>>(&vertex)) {
    return getNextVertices(*node);
  } else if (auto listener =
                 std::get_if<std::shared_ptr<AudioListener>>(&vertex)) {
    return {};
  }

  std::abort();
}

std::vector<AudioGraph::Vertex>
AudioGraph::getPreviousVertices(Vertex vertex) const {
  // TODO
  return {};
}

std::vector<AudioGraph::Vertex> AudioGraph::getVertices() const {
  std::vector<Vertex> vertices;

  for (const auto &node : nodes_) {
    vertices.push_back(node);
  }

  if (listener_) {
    vertices.push_back(listener_);
  }

  return vertices;
}

bool AudioGraph::isPartOfCycle(std::shared_ptr<AudioNode> node) const {
  enum VisitState { UNVISITED = 0, VISITING = 1, VISITED = 2 };
  std::unordered_map<std::shared_ptr<AudioNode>, VisitState> state;

  std::function<bool(std::shared_ptr<AudioNode>, std::shared_ptr<AudioNode>)>
      dfs = [&](std::shared_ptr<AudioNode> current,
                std::shared_ptr<AudioNode> origin) -> bool {
    state[current] = VISITING;

    for (const auto &next : getNextVertices(current)) {
      if (auto nextNode = std::get_if<std::shared_ptr<AudioNode>>(&next)) {
        auto &nextState = state[*nextNode];

        if (nextState == UNVISITED) {
          if (dfs(*nextNode, origin)) {
            return true;
          }
        } else if (nextState == VISITING) {
          if (*nextNode == origin) {
            return true;
          }
        }
      }
    }

    state[current] = VISITED;
    return false;
  };

  return dfs(node, node);
}

std::vector<std::vector<AudioGraph::Vertex>>
AudioGraph::getConnectedComponents() const {
  std::unordered_set<AudioGraph::Vertex> visited;
  std::vector<AudioGraph::Vertex> order;
  auto vertices = getVertices();

  std::function<void(AudioGraph::Vertex)> dfs = [&](AudioGraph::Vertex v) {
    visited.insert(v);

    auto next = getNextVertices(v);

    for (const auto &u : next) {
      if (!visited.count(u)) {
        dfs(u);
      }
    }

    order.push_back(v);
  };

  std::function<void(AudioGraph::Vertex, std::vector<AudioGraph::Vertex> &)>
      rdfs = [&](AudioGraph::Vertex v, std::vector<AudioGraph::Vertex> &comp) {
        visited.insert(v);
        comp.push_back(v);

        auto prev = getPreviousVertices(v);

        for (const auto &u : prev) {
          if (!visited.count(u)) {
            rdfs(u, comp);
          }
        }
      };

  for (const auto &v : vertices) {
    if (!visited.count(v)) {
      dfs(v);
    }
  }

  visited.clear();
  std::reverse(order.begin(), order.end());

  std::vector<std::vector<AudioGraph::Vertex>> sccs;

  for (const auto &v : order) {
    if (!visited.count(v)) {
      std::vector<AudioGraph::Vertex> comp;
      rdfs(v, comp);
      sccs.push_back(std::move(comp));
    }
  }

  return sccs;
}
} // namespace web_audio::details