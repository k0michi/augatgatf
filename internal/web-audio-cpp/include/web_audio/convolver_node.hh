#pragma once

#include <complex>
#include <memory>
#include <optional>
#include <vector>

#include "audio_node.hh"
#include "convolver_options.hh"
#include "detail/convolver.hh"

namespace web_audio {
class ConvolverNode : public AudioNode {
public:
  // SPEC: constructor (BaseAudioContext context, optional ConvolverOptions
  // options = {});
  static std::shared_ptr<ConvolverNode>
  create(std::shared_ptr<BaseAudioContext> context,
         ConvolverOptions options = {});

  // SPEC: attribute AudioBuffer? buffer;
  std::shared_ptr<AudioBuffer> getBuffer() const;

  void setBuffer(const std::shared_ptr<AudioBuffer> &buffer);

  // SPEC: attribute boolean normalize;
  bool getNormalize() const;

  void setNormalize(bool normalize);

  void process(const std::vector<detail::RenderQuantum> &inputs,
               std::vector<detail::RenderQuantum> &outputs,
               const detail::ParamCollection &params) override;

  static double
  calculateNormalizationScale(std::shared_ptr<AudioBuffer> buffer);

  WEB_AUDIO_PRIVATE : std::shared_ptr<AudioBuffer> buffer_;
  bool normalize_;
  std::shared_ptr<AudioBuffer> bufferCopy_;
  std::vector<std::vector<std::complex<float>>> bufferFrequency_;
  std::vector<std::unique_ptr<detail::Convolver<float>>> convolvers_;
};
} // namespace web_audio