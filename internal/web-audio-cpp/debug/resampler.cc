#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include <web_audio/detail/downsampler.hh>
#include <web_audio/detail/upsampler.hh>

#define M_PI 3.14159265358979323846

int main() {
  using namespace web_audio::detail;
  constexpr std::size_t blockSize = 1024;
  constexpr float freq = 2.0f * static_cast<float>(M_PI) / blockSize;

  std::vector<float> input(blockSize);
  for (std::size_t i = 0; i < blockSize; ++i) {
    // input[i] = std::sin(freq * i);
    // triangle
    // input[i] =
    //     1.0f - 4.0f * std::fabs(std::round((i /
    //     static_cast<float>(blockSize)) -
    //                                        0.25f) -
    //                             ((i / static_cast<float>(blockSize)) -
    //                             0.25f));
    // square
    input[i] = (i % (blockSize / 4) < (blockSize / 8)) ? 1.0f : -1.0f;
  }

  // Upsampler4x upsampler(blockSize);
  Downsampler4x downsampler(blockSize);
  constexpr std::uint32_t factor = 4;

  std::vector<float> output;
  // upsampler.process(input, output);
  downsampler.process(input, output);

  std::ofstream csv("upsample_wave.csv");
  // csv << "input,output\n";
  csv << "output\n";
  for (std::size_t i = 0; i < output.size(); ++i) {
    // float in = (i % factor == 0 && i / factor < input.size())
    //                ? input[i / factor]
    //                : 0.0f;
    // csv << in << "," << output[i] << "\n";
    csv << output[i] << "\n";
  }
  csv.close();
  std::cout << "upsample_wave.csv written.\n";
  return 0;
}