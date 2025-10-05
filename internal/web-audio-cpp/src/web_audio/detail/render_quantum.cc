#include "web_audio/detail/render_quantum.hh"

namespace web_audio::details {
RenderQuantum::RenderQuantum(std::uint32_t numberOfChannels,
                             std::uint32_t length)
    : length_(length),
      channelData_(numberOfChannels, std::vector<float>(length, 0.0f)) {}

std::uint32_t RenderQuantum::getLength() const { return length_; }

std::uint32_t RenderQuantum::getNumberOfChannels() const {
  return static_cast<std::uint32_t>(channelData_.size());
}

std::size_t RenderQuantum::size() const { return channelData_.size(); }

std::vector<float> &RenderQuantum::operator[](std::uint32_t channel) {
  return channelData_[channel];
}

const std::vector<float> &
RenderQuantum::operator[](std::uint32_t channel) const {
  return channelData_[channel];
}

void RenderQuantum::mix(std::uint32_t computedNumberOfChannels,
                        ChannelInterpretation channelInterpretation) {
  if (computedNumberOfChannels == channelData_.size()) {
    return;
  }

  if (channelInterpretation == ChannelInterpretation::eSpeakers) {
    switch (channelData_.size()) {
    case 1:
      switch (computedNumberOfChannels) {
      case 2:
        /*
         * output.L = input;
         * output.R = input;
         */
        channelData_.emplace_back(channelData_[0]);
        return;
      case 4:
        /*
         * output.L = input;
         * output.R = input;
         * output.SL = 0;
         * output.SR = 0;
         */
        channelData_.emplace_back(channelData_[0]);
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        return;
      case 6:
        /*
         * output.L = 0;
         * output.R = 0;
         * output.C = input; // put in center channel
         * output.LFE = 0;
         * output.SL = 0;
         * output.SR = 0;
         */
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        std::swap(channelData_[2], channelData_[0]);
        return;
      }
    case 2:
      switch (computedNumberOfChannels) {
      case 1:
        /*
         * output = 0.5 * (input.L + input.R);
         */
        for (std::uint32_t i = 0; i < length_; ++i) {
          channelData_[0][i] = 0.5f * (channelData_[0][i] + channelData_[1][i]);
        }
        channelData_.resize(1);
        return;
      case 4:
        /*
         * output.L = input.L;
         * output.R = input.R;
         * output.SL = 0;
         * output.SR = 0;
         */
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        return;
      case 6:
        /*
         * output.L = input.L;
         * output.R = input.R;
         * output.C = 0;
         * output.LFE = 0;
         * output.SL = 0;
         * output.SR = 0;
         */
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        return;
      }
    case 4:
      switch (computedNumberOfChannels) {
      case 1:
        /*
         * output = 0.25 * (input.L + input.R + input.SL + input.SR);
         */
        for (std::uint32_t i = 0; i < length_; ++i) {
          channelData_[0][i] =
              0.25f * (channelData_[0][i] + channelData_[1][i] +
                       channelData_[2][i] + channelData_[3][i]);
        }

        channelData_.resize(1);
        return;
      case 2:
        /*
         * output.L = 0.5 * (input.L + input.SL);
         * output.R = 0.5 * (input.R + input.SR);
         */
        for (std::uint32_t i = 0; i < length_; ++i) {
          channelData_[0][i] = 0.5f * (channelData_[0][i] + channelData_[2][i]);
          channelData_[1][i] = 0.5f * (channelData_[1][i] + channelData_[3][i]);
        }

        channelData_.resize(2);
        return;
      case 6:
        /*
         * output.L = input.L;
         * output.R = input.R;
         * output.C = 0;
         * output.LFE = 0;
         * output.SL = input.SL;
         * output.SR = input.SR;
         */
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        channelData_.emplace_back(std::vector<float>(length_, 0.0f));
        std::swap(channelData_[4], channelData_[2]);
        std::swap(channelData_[5], channelData_[3]);
        return;
      }
    case 6:
      switch (computedNumberOfChannels) {
      case 1:
        /*
         * output = sqrt(0.5) * (input.L + input.R) + input.C + 0.5 *
         * (input.SL + input.SR)
         */
        for (std::uint32_t i = 0; i < length_; ++i) {
          channelData_[0][i] =
              std::sqrt(0.5f) * (channelData_[0][i] + channelData_[1][i]) +
              channelData_[2][i] +
              0.5f * (channelData_[4][i] + channelData_[5][i]);
        }
        channelData_.resize(1);
        return;
      case 2:
        /*
         * output.L = L + sqrt(0.5) * (input.C + input.SL)
         * output.R = R + sqrt(0.5) * (input.C + input.SR)
         */
        for (std::uint32_t i = 0; i < length_; ++i) {
          channelData_[0][i] =
              channelData_[0][i] +
              std::sqrt(0.5f) * (channelData_[2][i] + channelData_[4][i]);
          channelData_[1][i] =
              channelData_[1][i] +
              std::sqrt(0.5f) * (channelData_[2][i] + channelData_[5][i]);
        }

        channelData_.resize(2);
        return;
      case 4:
        /*
         * output.L = L + sqrt(0.5) * input.C
         * output.R = R + sqrt(0.5) * input.C
         * output.SL = input.SL
         * output.SR = input.SR
         */
        for (std::uint32_t i = 0; i < length_; ++i) {
          channelData_[0][i] =
              channelData_[0][i] + std::sqrt(0.5f) * channelData_[2][i];
          channelData_[1][i] =
              channelData_[1][i] + std::sqrt(0.5f) * channelData_[2][i];
        }

        channelData_.erase(channelData_.begin() + 2, channelData_.begin() + 4);
        return;
      }
    }
  }

  if (computedNumberOfChannels > channelData_.size()) {
    channelData_.resize(computedNumberOfChannels,
                        std::vector<float>(length_, 0.0f));
  } else {
    channelData_.resize(computedNumberOfChannels);
  }
}
} // namespace web_audio::details