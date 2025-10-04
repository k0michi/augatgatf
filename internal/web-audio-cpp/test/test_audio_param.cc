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

TEST(AudioParamTest, EventInValueCurve) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueCurveAtTime({0.0f, 1.0f, 2.0f}, 0, 2);
  EXPECT_THROW(param->setValueAtTime(1.0f, 1), web_audio::DOMException);
  EXPECT_THROW(param->linearRampToValueAtTime(1.0f, 1),
               web_audio::DOMException);
  EXPECT_THROW(param->exponentialRampToValueAtTime(1.0f, 1),
               web_audio::DOMException);
  EXPECT_THROW(param->setTargetAtTime(1.0f, 1, 1.0f), web_audio::DOMException);
  EXPECT_THROW(param->setValueCurveAtTime({0.0f, 1.0f}, 1, 2),
               web_audio::DOMException);
}

TEST(AudioParamTest, EventOverlapValueCurve) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1.0);
  EXPECT_THROW(param->setValueCurveAtTime({0.0f, 1.0f, 2.0f}, 0.0, 2.0),
               web_audio::DOMException);
  EXPECT_NO_THROW(param->setValueCurveAtTime({0.0f, 1.0f, 2.0f}, 0.0, 1.0));
  EXPECT_NO_THROW(param->setValueCurveAtTime({0.0f, 1.0f, 2.0f}, 1.0, 1.0));
}

TEST(AudioParamTest, ComputeIntrinsicValues_SetValue) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 0.0);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (auto v : outputs) {
    EXPECT_FLOAT_EQ(v, 1.0f);
  }
}

TEST(AudioParamTest, ComputeIntrinsicValues_LinearRamp) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 0.0);
  param->linearRampToValueAtTime(2.0f, 1.0);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (std::size_t i = 0; i < outputs.size(); ++i) {
    auto expected =
        1.0f + (2.0f - 1.0f) * (static_cast<float>(i) / outputs.size());
    EXPECT_NEAR(outputs[i], expected, 0.01f);
  }
}

TEST(AudioParamTest, ComputeIntrinsicValues_ExponentialRamp) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 0.0);
  param->exponentialRampToValueAtTime(2.0f, 1.0);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (std::size_t i = 0; i < outputs.size(); ++i) {
    auto expected =
        1.0f * std::pow(2.0f / 1.0f, static_cast<float>(i) / outputs.size());
    EXPECT_NEAR(outputs[i], expected, 0.01f);
  }
}