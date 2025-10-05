#include "web_audio/detail/audio_graph.hh"

#include "web_audio/audio_param.hh"

namespace web_audio::detail {
void AudioGraph::initialize(std::shared_ptr<BaseAudioContext> context) {
  listenerNode_ = AudioListenerNode::create(context);
  destinationNode_.reset(new AudioDestinationNode());
  destinationNode_->context_ = context;
  nodes_.push_back(destinationNode_);
}

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

std::vector<std::shared_ptr<AudioNode>>
AudioGraph::getNextVertices(std::shared_ptr<AudioNode> node) const {
  std::vector<std::shared_ptr<AudioNode>> nextNodes;

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
        nextNodes.push_back(owner);
      }
    }
  }

  return nextNodes;
}

std::vector<std::shared_ptr<AudioNode>>
AudioGraph::getPreviousVertices(std::shared_ptr<AudioNode> node) const {
  std::vector<std::shared_ptr<AudioNode>> prevNodes;

  for (const auto &input : node->inputs_) {
    if (auto srcNode = input.source.lock()) {
      prevNodes.push_back(srcNode);
    }
  }

  for (const auto &input : node->inputsIndirect_) {
    if (auto srcNode = input.lock()) {
      prevNodes.push_back(srcNode);
    }
  }

  return prevNodes;
}

std::vector<std::shared_ptr<AudioNode>> AudioGraph::getVertices() const {
  std::vector<std::shared_ptr<AudioNode>> vertices;

  for (const auto &node : nodes_) {
    vertices.push_back(node);
  }

  vertices.push_back(listenerNode_);

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
      auto &nextState = state[next];

      if (nextState == UNVISITED) {
        if (dfs(next, origin)) {
          return true;
        }
      } else if (nextState == VISITING) {
        if (next == origin) {
          return true;
        }
      }
    }

    state[current] = VISITED;
    return false;
  };

  return dfs(node, node);
}

std::vector<std::vector<std::shared_ptr<AudioNode>>>
AudioGraph::getStronglyConnectedComponents() const {
  std::unordered_set<std::shared_ptr<AudioNode>> visited;
  std::vector<std::shared_ptr<AudioNode>> order;
  auto vertices = getVertices();

  std::function<void(std::shared_ptr<AudioNode>)> dfs =
      [&](std::shared_ptr<AudioNode> v) {
        visited.insert(v);

        auto next = getNextVertices(v);

        for (const auto &u : next) {
          if (!visited.count(u)) {
            dfs(u);
          }
        }

        order.push_back(v);
      };

  std::function<void(std::shared_ptr<AudioNode>,
                     std::vector<std::shared_ptr<AudioNode>> &)>
      rdfs = [&](std::shared_ptr<AudioNode> v,
                 std::vector<std::shared_ptr<AudioNode>> &comp) {
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

  std::vector<std::vector<std::shared_ptr<AudioNode>>> sccs;

  for (const auto &v : order) {
    if (!visited.count(v)) {
      std::vector<std::shared_ptr<AudioNode>> comp;
      rdfs(v, comp);
      sccs.push_back(std::move(comp));
    }
  }

  return sccs;
}

std::shared_ptr<AudioListenerNode> AudioGraph::getListenerNode() const {
  return listenerNode_;
}

std::shared_ptr<AudioDestinationNode> AudioGraph::getDestinationNode() const {
  return destinationNode_;
}
} // namespace web_audio::detail