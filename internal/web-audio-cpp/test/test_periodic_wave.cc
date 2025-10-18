#include <gtest/gtest.h>

#include "web_audio/offline_audio_context.hh"
#include "web_audio/periodic_wave.hh"
#include "web_audio/periodic_wave_options.hh"

using namespace web_audio;

namespace {
std::shared_ptr<OfflineAudioContext> createOfflineContext() {
  return OfflineAudioContext::create(2, 128, 44100.0f);
}
} // namespace

TEST(TestPeriodicWave, DefaultConstructor) {
  auto ctx = createOfflineContext();
  auto wave = PeriodicWave::create(ctx);
  EXPECT_NE(wave, nullptr);
}

TEST(TestPeriodicWave, RealImagConstructor) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2, 3};
  opts.imag = std::vector<float>{0, 4, 5};
  auto wave = PeriodicWave::create(ctx, opts);
  EXPECT_NE(wave, nullptr);
}

TEST(TestPeriodicWave, RealOnly) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2, 3};
  auto wave = PeriodicWave::create(ctx, opts);
  EXPECT_NE(wave, nullptr);
}

TEST(TestPeriodicWave, ImagOnly) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.imag = std::vector<float>{0, 4, 5};
  auto wave = PeriodicWave::create(ctx, opts);
  EXPECT_NE(wave, nullptr);
}

TEST(TestPeriodicWave, InvalidLength) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0};
  opts.imag = std::vector<float>{0, 0};
  EXPECT_THROW(PeriodicWave::create(ctx, opts), DOMException);
}

TEST(TestPeriodicWave, InvalidLength2) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 0};
  opts.imag = std::vector<float>{0};
  EXPECT_THROW(PeriodicWave::create(ctx, opts), DOMException);
}

TEST(TestPeriodicWave, InvalidLengthMismatch) {
  auto ctx = createOfflineContext();
  PeriodicWaveOptions opts;
  opts.real = std::vector<float>{0, 2};
  opts.imag = std::vector<float>{0, 4, 5};
  EXPECT_THROW(PeriodicWave::create(ctx, opts), DOMException);
}