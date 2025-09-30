#define WEB_AUDIO_IMPLEMENTATION
#include "web_audio/periodic_wave.h"
#include "web_audio/periodic_wave_options.h"
#include <gtest/gtest.h>
#include <memory>

using namespace web_audio;

class web_audio::BaseAudioContext {};

class DummyContext : public BaseAudioContext {};

TEST(PeriodicWaveTest, DefaultConstructor) {
  auto ctx = std::make_shared<DummyContext>();
  PeriodicWave wave(ctx);
}

TEST(PeriodicWaveTest, RealImagConstructor) {
  auto ctx = std::make_shared<DummyContext>();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2, 3};
  opts.imag = std::vector<float>{0, 4, 5};
  PeriodicWave wave(ctx, opts);
}

TEST(PeriodicWaveTest, RealOnly) {
  auto ctx = std::make_shared<DummyContext>();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2, 3};
  PeriodicWave wave(ctx, opts);
}

TEST(PeriodicWaveTest, ImagOnly) {
  auto ctx = std::make_shared<DummyContext>();
  PeriodicWaveOptions opts;
  opts.imag = std::vector<float>{0, 4, 5};
  PeriodicWave wave(ctx, opts);
}

TEST(PeriodicWaveTest, InvalidLength) {
  auto ctx = std::make_shared<DummyContext>();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0};
  opts.imag = std::vector<float>{0};
  EXPECT_THROW(PeriodicWave wave(ctx, opts), DOMException);
}

TEST(PeriodicWaveTest, DisableNormalization) {
  auto ctx = std::make_shared<DummyContext>();
  PeriodicWaveOptions opts;
  opts.disableNormalization = true;
  PeriodicWave wave(ctx, opts);
}
