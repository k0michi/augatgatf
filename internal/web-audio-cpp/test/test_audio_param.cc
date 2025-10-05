#include <gtest/gtest.h>

#include "dummy_node.h"
#include "web_audio.h"

#include <unordered_set>

namespace {
std::shared_ptr<web_audio::OfflineAudioContext> context;

std::shared_ptr<web_audio::OfflineAudioContext> createOfflineContext() {
  return context = web_audio::OfflineAudioContext::create(2, 128, 44100.0f);
}

// FIXME:
std::unordered_set<std::shared_ptr<DummyNode>> nodes;

std::shared_ptr<web_audio::AudioParam>
createAudioParam(std::shared_ptr<web_audio::OfflineAudioContext> context) {
  std::shared_ptr<DummyNode> dummyNode = DummyNode::create(context);
  nodes.insert(dummyNode);
  return dummyNode->param_;
}

std::shared_ptr<web_audio::AudioParam>
createAudioParam1(std::shared_ptr<web_audio::OfflineAudioContext> context) {
  std::shared_ptr<DummyNode> dummyNode = DummyNode::create(context);
  nodes.insert(dummyNode);
  return dummyNode->param1_;
}
} // namespace

TEST(AudioParamTest, Create) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
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

TEST(AudioParamTest, getEndValue) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1);
  auto e = param->events_.begin();
  EXPECT_EQ(param->getEndValue(e), 1.0f);
}

TEST(AudioParamTest, getEndValue_LinearRamp) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1);
  param->linearRampToValueAtTime(2.0f, 2);
  auto e = std::next(param->events_.begin());
  EXPECT_EQ(param->getEndValue(e), 2.0f);
}

TEST(AudioParamTest, getEndValue_ExponentialRamp) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 1);
  param->exponentialRampToValueAtTime(2.0f, 2);
  auto e = std::next(param->events_.begin());
  EXPECT_EQ(param->getEndValue(e), 2.0f);
}

TEST(AudioParamTest, getEndValue_SetTarget) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setTargetAtTime(1.0f, 0, 1.0f);
  param->setValueAtTime(2.0f, 1);
  auto e = param->events_.begin();
  EXPECT_NEAR(param->getEndValue(e), 1 + (-1.0 / std::exp(1.0)), 0.01);
}

TEST(AudioParamTest, getEndValue_SetTarget_1) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setTargetAtTime(1.0f, 0, 1.0f);
  param->setValueAtTime(2.0f, 2);
  auto e = param->events_.begin();
  EXPECT_NEAR(param->getEndValue(e), 1 + (-1.0 / std::exp(2.0)), 0.01);
}

TEST(AudioParamTest, getEndValue_SetValueCurve) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueCurveAtTime({0.0f, 1.0f, 2.0f}, 0, 2);
  auto e = param->events_.begin();
  EXPECT_EQ(param->getEndValue(e), 2.0f);
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

TEST(AudioParamTest, ComputeIntrinsicValues_SetTarget) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setTargetAtTime(1.0f, 0.0, 0.5f);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (std::size_t i = 0; i < 200; ++i) {
    auto t = static_cast<double>(i) / context->getSampleRate();
    auto expected = 1.0f + (0.0f - 1.0f) * std::exp(-t / 0.5f);
    EXPECT_NEAR(outputs[i], expected, 0.01f);
  }
}

TEST(AudioParamTest, ComputeIntrinsicValues_SetValueCurve) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueCurveAtTime({0.0f, 1.0f, 0.0f}, 0.0, 1.0);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (std::size_t i = 0; i < outputs.size(); ++i) {
    auto t = static_cast<double>(i) / context->getSampleRate();
    auto expected = (t < 0.5) ? (2.0f * t) : (-2.0f * t + 2.0f);
    EXPECT_NEAR(outputs[i], expected, 0.01f);
  }
}

TEST(AudioParamTest, ComputeIntrinsicValues_DuplicateEvents) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueAtTime(1.0f, 0.0);
  param->setValueAtTime(2.0f, 0.0);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (auto v : outputs) {
    EXPECT_FLOAT_EQ(v, 2.0f);
  }
}

TEST(AudioParamTest, ComputeIntrinsicValues_NoEvents) {
  auto context = createOfflineContext();
  auto param = createAudioParam1(context);
  std::vector<float> outputs(context->getSampleRate(), 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (auto v : outputs) {
    EXPECT_FLOAT_EQ(v, 1.0f);
  }
}

TEST(AudioParamTest, ComputeIntrinsicValues_LinearAfterCurve) {
  auto context = createOfflineContext();
  auto param = createAudioParam(context);
  param->setValueCurveAtTime({0.0f, 1.0f, 0.0f}, 0.0, 1.0);
  param->linearRampToValueAtTime(2.0f, 2.0);
  std::vector<float> outputs(context->getSampleRate() * 2, 0.0f);
  param->computeIntrinsicValues(0.0, outputs);

  for (std::size_t i = 0; i < outputs.size(); ++i) {
    auto t = static_cast<double>(i) / context->getSampleRate();
    if (t < 1.0) {
      auto expected = (t < 0.5) ? (2.0f * t) : (-2.0f * t + 2.0f);
      EXPECT_NEAR(outputs[i], expected, 0.01f);
    } else {
      auto expected = 0.0f + (2.0f - 0.0f) * (t - 1.0f) / (2.0f - 1.0f);
      EXPECT_NEAR(outputs[i], expected, 0.01f);
    }
  }
}

TEST(AudioParamTest, CurrentValue) {
  auto context = createOfflineContext();
  auto param = createAudioParam1(context);
  EXPECT_EQ(param->getValue(), 1.0f);
}