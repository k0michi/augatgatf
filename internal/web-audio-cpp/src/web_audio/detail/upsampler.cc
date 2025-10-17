#include "web_audio/detail/upsampler.hh"

#include "web_audio/detail/math_helper.hh"
#include "web_audio/detail/wave_processing.hh"

namespace web_audio::detail {
Upsampler::Upsampler(const std::vector<float> &filterCoefficients,
                     std::size_t factor, std::size_t blockSize)
    : factor_(factor), inputBlockSize_(blockSize) {
  filterCoefficients_ = filterCoefficients;

  outputBlockSize_ = inputBlockSize_ * factor_;
  filterSize_ = filterCoefficients_.size();
  fftSize_ = MathHelper::nextPowerOfTwo(outputBlockSize_ + filterSize_ - 1);
  filterCoefficients_.resize(fftSize_, 0.0f);
  fftCoefficientsFrequency_.resize(fftSize_);
  overlapBuffer_.resize(filterSize_ - 1, std::complex<float>(0.0f, 0.0f));

  WaveProcessing::fourierTransform(filterCoefficients_,
                                   fftCoefficientsFrequency_);
}

void Upsampler::process(const std::vector<float> &input,
                        std::vector<float> &output) {
  if (input.size() != inputBlockSize_) {
    throw std::invalid_argument("Input size must be equal to block size.");
  }

  output.resize(outputBlockSize_);

  std::vector<float> inputPadded(fftSize_, 0.0f);

  for (std::size_t i = 0; i < inputBlockSize_; ++i) {
    inputPadded[i * factor_] = input[i];
  }

  std::vector<std::complex<float>> inputFrequency(fftSize_);
  WaveProcessing::fourierTransform(inputPadded, inputFrequency);

  std::vector<std::complex<float>> outputFrequency(fftSize_);

  for (std::size_t i = 0; i < fftSize_; ++i) {
    outputFrequency[i] = inputFrequency[i] * fftCoefficientsFrequency_[i] *
                         static_cast<float>(fftSize_);
  }

  std::vector<std::complex<float>> outputTime(fftSize_);
  WaveProcessing::inverseFourierTransform(outputFrequency, outputTime);

  for (std::size_t i = 0; i < outputBlockSize_; ++i) {
    std::complex<float> sample =
        outputTime[i] + (i < filterSize_ - 1 ? overlapBuffer_[i]
                                             : std::complex<float>(0.0f, 0.0f));
    output[i] = sample.real();
  }

  for (std::size_t i = 0; i < filterSize_ - 1; ++i) {
    overlapBuffer_[i] = outputTime[outputBlockSize_ + i];
  }
}

Upsampler2x::Upsampler2x(std::size_t blockSize)
    : Upsampler(
          {
              -5.4776569282358651e-08f, -1.7959945733991557e-07f,
              4.1240440064676782e-07f,  8.0890573408901516e-07f,
              -1.4439811482027084e-06f, -2.4160987738951988e-06f,
              3.852356105242963e-06f,   5.9141500309856959e-06f,
              -8.8034879317950675e-06f, -1.2769939414362163e-05f,
              1.811821702944141e-05f,   2.5216362559275639e-05f,
              -3.4504503527182013e-05f, -4.6504132912434091e-05f,
              6.1827854147757531e-05f,  8.1189523901447898e-05f,
              -0.000105414717533443f,   -0.00013545143716174956f,
              0.00017238098074639799f,  0.00021742889332693603f,
              -0.00027197592944999671f, -0.000337568969901598f,
              0.00041593185723707828f,  0.00050897614455143404f,
              -0.0006188117919281392f,  -0.00074775789679210438f,
              0.00089835361009840328f,  0.0010733694725623591f,
              -0.0012758195073583041f,  -0.0015089745322770993f,
              0.0017763773110491784f,   0.0020818603581997841f,
              -0.0024295674549812784f,  -0.0028239802401132321f,
              0.0032699516283132344f,   0.0037727483099225037f,
              -0.0043381052357942965f,  -0.0049722958491823449f,
              0.0056822229713967684f,   0.0064755367978850138f,
              -0.0073607885889413411f,  -0.0083476316021607738f,
              0.0094470850020804565f,   0.010671882495974708f,
              -0.012036936222087071f,   -0.013559959454762044f,
              0.015262311428463753f,    0.017170158083929202f,
              -0.019316090753758949f,   -0.021741422952386815f,
              0.02449951569507967f,     0.027660705837698616f,
              -0.031319811380827615f,   -0.035607929557376308f,
              0.040711688273328196f,    0.046906086013730995f,
              -0.054613599387065503f,   -0.064517842630877578f,
              0.077801204348121072f,    0.096699309700867278f,
              -0.12600595270958675f,    -0.17818980703686041f,
              0.29897787003943377f,     0.89993987733099334f,
              0.89993987733099334f,     0.29897787003943377f,
              -0.17818980703686041f,    -0.12600595270958675f,
              0.096699309700867278f,    0.077801204348121072f,
              -0.064517842630877578f,   -0.054613599387065503f,
              0.046906086013730995f,    0.040711688273328196f,
              -0.035607929557376308f,   -0.031319811380827615f,
              0.027660705837698616f,    0.02449951569507967f,
              -0.021741422952386815f,   -0.019316090753758949f,
              0.017170158083929202f,    0.015262311428463753f,
              -0.013559959454762044f,   -0.012036936222087071f,
              0.010671882495974708f,    0.0094470850020804565f,
              -0.0083476316021607738f,  -0.0073607885889413411f,
              0.0064755367978850138f,   0.0056822229713967684f,
              -0.0049722958491823449f,  -0.0043381052357942965f,
              0.0037727483099225037f,   0.0032699516283132344f,
              -0.0028239802401132321f,  -0.0024295674549812784f,
              0.0020818603581997841f,   0.0017763773110491784f,
              -0.0015089745322770993f,  -0.0012758195073583041f,
              0.0010733694725623591f,   0.00089835361009840328f,
              -0.00074775789679210438f, -0.0006188117919281392f,
              0.00050897614455143404f,  0.00041593185723707828f,
              -0.000337568969901598f,   -0.00027197592944999671f,
              0.00021742889332693603f,  0.00017238098074639799f,
              -0.00013545143716174956f, -0.000105414717533443f,
              8.1189523901447898e-05f,  6.1827854147757531e-05f,
              -4.6504132912434091e-05f, -3.4504503527182013e-05f,
              2.5216362559275639e-05f,  1.811821702944141e-05f,
              -1.2769939414362163e-05f, -8.8034879317950675e-06f,
              5.9141500309856959e-06f,  3.852356105242963e-06f,
              -2.4160987738951988e-06f, -1.4439811482027084e-06f,
              8.0890573408901516e-07f,  4.1240440064676782e-07f,
              -1.7959945733991557e-07f, -5.4776569282358651e-08f,
          },
          2, blockSize) {}
} // namespace web_audio::detail