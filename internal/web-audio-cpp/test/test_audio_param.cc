#include <gtest/gtest.h>

#include "web_audio.h"

namespace {
std::shared_ptr<web_audio::OfflineAudioContext> createOfflineContext() {
  return web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
}

std::shared_ptr<web_audio::AudioParam>
createAudioParam(std::shared_ptr<web_audio::OfflineAudioContext> context) {
  return web_audio::AudioParam::create(context, 0.0f,
                                       -std::numeric_limits<float>::infinity(),
                                       std::numeric_limits<float>::infinity(),
                                       web_audio::AutomationRate::eARate, true);
}
} // namespace

TEST(AudioParamTest, Create) {
  auto context = createOfflineContext();
  auto param = web_audio::AudioParam::create(
      context, 0.0f, -std::numeric_limits<float>::infinity(),
      std::numeric_limits<float>::infinity(), web_audio::AutomationRate::eARate,
      true);
  EXPECT_EQ(param->getValue(), 0.0f);
  EXPECT_EQ(param->getDefaultValue(), 0.0f);
  EXPECT_EQ(param->getMinValue(), -std::numeric_limits<float>::infinity());
  EXPECT_EQ(param->getMaxValue(), std::numeric_limits<float>::infinity());
  EXPECT_EQ(param->getAutomationRate(), web_audio::AutomationRate::eARate);
}

TEST(AudioParamTest, SetValue) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValue(1.0f);
  EXPECT_EQ(param->getValue(), 1.0f);
}

TEST(AudioParamTest, GetLastValue) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1);
  auto e = param->events_.begin();
  EXPECT_EQ(param->getLastValue(e), 1.0f);
}

TEST(AudioParamTest, GetLastValue_LinearRamp) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1);
  param->linearRampToValueAtTime(2.0f, 2);
  auto e = std::next(param->events_.begin());
  EXPECT_EQ(param->getLastValue(e), 2.0f);
}

TEST(AudioParamTest, GetLastValue_ExponentialRamp) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1);
  param->exponentialRampToValueAtTime(2.0f, 2);
  auto e = std::next(param->events_.begin());
  EXPECT_EQ(param->getLastValue(e), 2.0f);
}

TEST(AudioParamTest, GetLastValue_SetTarget) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setTargetAtTime(1.0f, 0, 1.0f);
  param->setValueAtTime(2.0f, 1);
  auto e = param->events_.begin();
  EXPECT_NEAR(param->getLastValue(e), 1 + (-1.0 / std::exp(1.0)), 0.01);
}

TEST(AudioParamTest, GetLastValue_SetTarget_1) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setTargetAtTime(1.0f, 0, 1.0f);
  param->setValueAtTime(2.0f, 2);
  auto e = param->events_.begin();
  EXPECT_NEAR(param->getLastValue(e), 1 + (-1.0 / std::exp(2.0)), 0.01);
}

TEST(AudioParamTest, GetLastValue_SetValueCurve) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueCurveAtTime({0.0f, 1.0f, 2.0f}, 0, 2);
  auto e = param->events_.begin();
  EXPECT_EQ(param->getLastValue(e), 2.0f);
}