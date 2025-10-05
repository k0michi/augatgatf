#pragma once

#include <stdexcept>

#include "audio_node.hh"
#include "audio_param.hh"
#include "delay_options.hh"
#include "details/common.hh"
#include "details/param_collection.hh"
#include "details/render_quantum.hh"

namespace web_audio {
class DelayNode : public AudioNode {
  WEB_AUDIO_PRIVATE : DelayNode() = default;

public:
  static std::shared_ptr<DelayNode>
  create(std::shared_ptr<BaseAudioContext> context,
         const DelayOptions &options = {});

  std::shared_ptr<AudioParam> getDelayTime() const;

  void process(const std::vector<details::RenderQuantum> &inputs,
               std::vector<details::RenderQuantum> &outputs,
               const details::ParamCollection &params) override;

public:
  class DelayNodeReader : public AudioNode {
    WEB_AUDIO_PRIVATE : DelayNodeReader() = default;

  public:
    void process(const std::vector<details::RenderQuantum> &inputs,
                 std::vector<details::RenderQuantum> &outputs,
                 const details::ParamCollection &params) override;

    static std::shared_ptr<DelayNodeReader>
    create(std::shared_ptr<DelayNode> delayNode);

    WEB_AUDIO_PRIVATE : std::weak_ptr<DelayNode> delayNode_;
  };

  class DelayNodeWriter : public AudioNode {
    WEB_AUDIO_PRIVATE : DelayNodeWriter() = default;

  public:
    void process(const std::vector<details::RenderQuantum> &inputs,
                 std::vector<details::RenderQuantum> &outputs,
                 const details::ParamCollection &params) override;

    static std::shared_ptr<DelayNodeWriter>
    create(std::shared_ptr<DelayNode> delayNode);

    WEB_AUDIO_PRIVATE : std::weak_ptr<DelayNode> delayNode_;
  };

  WEB_AUDIO_PRIVATE : std::shared_ptr<AudioParam> delayTime_;
  double maxDelayTime_;
  std::vector<details::RenderQuantum> delayBuffers_;
  size_t writeIndex_ = 0;
  std::shared_ptr<DelayNodeReader> reader_;
  std::shared_ptr<DelayNodeWriter> writer_;

  friend class DelayNodeReader;
  friend class DelayNodeWriter;
};
} // namespace web_audio