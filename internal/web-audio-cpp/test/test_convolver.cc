#include <gtest/gtest.h>

#include <web_audio.hh>

#include "test_helper.hh"

std::vector<float> naiveConvolve(const std::vector<float> &input,
                                 const std::vector<float> &impulseResponse) {
  std::size_t inputSize = input.size();
  std::size_t impulseSize = impulseResponse.size();
  std::size_t outputSize = inputSize + impulseSize - 1;

  std::vector<float> output(outputSize, 0.0f);

  for (std::size_t n = 0; n < outputSize; ++n) {
    for (std::size_t k = 0; k < impulseSize; ++k) {
      auto inputIndex = n - k;
      if (inputIndex >= 0 && inputIndex < inputSize) {
        output[n] += input[inputIndex] * impulseResponse[k];
      }
    }
  }

  return output;
}

std::vector<float> convolve(const std::vector<float> &input,
                            const std::vector<float> &impulseResponse,
                            std::size_t blockSize = 8) {
  web_audio::detail::Convolver<float> convolver(impulseResponse, blockSize);
  std::size_t outputSize = input.size() + impulseResponse.size() - 1;
  std::vector<float> output(outputSize, 0.0f);

  for (std::size_t offset = 0; offset < outputSize; offset += blockSize) {
    std::vector<float> inputBlock(blockSize, 0.0f);
    std::size_t copySize = std::clamp(
        static_cast<std::ptrdiff_t>(input.size()) -
            static_cast<std::ptrdiff_t>(offset),
        static_cast<std::ptrdiff_t>(0), static_cast<std::ptrdiff_t>(blockSize));
    std::copy_n(input.data() + offset, copySize, inputBlock.data());

    std::vector<float> outputBlock;
    convolver.process(inputBlock, outputBlock);

    for (std::size_t i = 0; i < outputBlock.size(); ++i) {
      if (offset + i < output.size()) {
        output[offset + i] += outputBlock[i];
      }
    }
  }

  return output;
}

template <std::floating_point T>
void compareVectors(const std::vector<T> &a, const std::vector<T> &b,
                    T epsilon) {
  ASSERT_EQ(a.size(), b.size()) << "Vectors are of different sizes.";
  for (std::size_t i = 0; i < a.size(); ++i) {
    EXPECT_NEAR(a[i], b[i], epsilon) << "Vectors differ at index " << i;
  }
}

// TEST(ConvolverTest, Identity) {
//   std::vector<float> impulseResponse = {1.0f, 0.0f, 0.0f, 0.0f};
//   std::vector<float> input = {0.5f,  -0.5f,  0.25f, -0.25f,
//                               0.75f, -0.75f, 1.0f,  -1.0f};
//   auto expectedOutput = naiveConvolve(input, impulseResponse);
//   auto output = convolve(input, impulseResponse, 8);
//   compareVectors(expectedOutput, output, 1e-6f);
// }
//
// TEST(ConvolverTest, Delay) {
//   std::vector<float> impulseResponse = {0.0f, 0.0f, 1.0f};
//   std::vector<float> input = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//   0.0f};
//
//   auto expectedOutput = naiveConvolve(input, impulseResponse);
//   auto output = convolve(input, impulseResponse, 8);
//   compareVectors(expectedOutput, output, 1e-6f);
// }

TEST(ConvolverTest, LongerImpulse) {
  std::vector<float> impulseResponse = {0.2f, 0.4f, 0.6f, 0.8f, 1.0f,
                                        0.8f, 0.6f, 0.4f, 0.2f};
  std::vector<float> input = {1.0f, 1.0f, 1.0f, 1.0f};

  auto expectedOutput = naiveConvolve(input, impulseResponse);
  auto output = convolve(input, impulseResponse, 4);
  compareVectors(expectedOutput, output, 1e-6f);
}