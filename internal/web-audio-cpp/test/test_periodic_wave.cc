#include <gtest/gtest.h>

#include "web_audio/offline_audio_context.h"
#include "web_audio/periodic_wave.h"
#include "web_audio/periodic_wave_options.h"

using namespace web_audio;

namespace {
std::shared_ptr<OfflineAudioContext> createOfflineContext() {
  return OfflineAudioContext::create(2, 128, 44100.0f);
}
} // namespace

TEST(PeriodicWaveTest, DefaultConstructor) {
  auto ctx = createOfflineContext();
  auto wave = PeriodicWave::create(ctx);
  EXPECT_NE(wave, nullptr);
}

TEST(PeriodicWaveTest, RealImagConstructor) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2, 3};
  opts.imag = std::vector<float>{0, 4, 5};
  auto wave = PeriodicWave::create(ctx, opts);
  EXPECT_NE(wave, nullptr);
}

TEST(PeriodicWaveTest, RealOnly) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2, 3};
  auto wave = PeriodicWave::create(ctx, opts);
  EXPECT_NE(wave, nullptr);
}

TEST(PeriodicWaveTest, ImagOnly) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.imag = std::vector<float>{0, 4, 5};
  auto wave = PeriodicWave::create(ctx, opts);
  EXPECT_NE(wave, nullptr);
}

TEST(PeriodicWaveTest, InvalidLength) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0};
  opts.imag = std::vector<float>{0, 0};
  EXPECT_THROW(PeriodicWave::create(ctx, opts), DOMException);
}

TEST(PeriodicWaveTest, InvalidLength2) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 0};
  opts.imag = std::vector<float>{0};
  EXPECT_THROW(PeriodicWave::create(ctx, opts), DOMException);
}

TEST(PeriodicWaveTest, InvalidLengthMismatch) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2};
  opts.imag = std::vector<float>{0, 4, 5};
  EXPECT_THROW(PeriodicWave::create(ctx, opts), DOMException);
}