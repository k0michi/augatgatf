#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

constexpr double kPi = std::numbers::pi;

#include <web_audio.hh>

using namespace web_audio;

// TEST(TestOscillatorNode, OutputOffline) {
//   OfflineAudioContextOptions options;
//   options.numberOfChannels = 2;
//   options.length = 44100;
//   options.sampleRate = 44100.0f;

//   auto context = OfflineAudioContext::create(options);
//   auto oscillatorOptions = OscillatorOptions{
//       .type = OscillatorType::eSine,
//       .frequency = 1,
//   };
//   oscillatorOptions.channelCount = 2;
//   auto oscillator = OscillatorNode::create(context, oscillatorOptions);
//   oscillator->connect(context->getDestination());
//   oscillator->start(0.0);
//   auto promise = context->startRendering();
//   bool called = false;
//   promise.then([&](std::shared_ptr<AudioBuffer> buffer) {
//     for (std::uint32_t channel = 0; channel < buffer->getNumberOfChannels();
//          ++channel) {
//       auto data = buffer->getChannelData(channel);
//       for (std::uint32_t i = 0; i < buffer->getLength(); ++i) {
//         float expected = std::sin(
//             2 * kPi * (static_cast<float>(i) / buffer->getSampleRate()));
//         EXPECT_NEAR(data[i], expected, 0.01f);
//       }
//     }
//     called = true;
//   });
//   promise.catch_([&](std::exception_ptr exception) {
//     called = true;
//     FAIL() << "Promise rejected";
//   });

//   while (!called) {
//     context->processEvents();
//   }
// }