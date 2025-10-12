#include <gtest/gtest.h>
#include <web_audio.hh>

#include <cmath>

TEST(BiquadFilterNodeTest, Create) {
  auto context = web_audio::AudioContext::create();
  auto node = web_audio::BiquadFilterNode::create(context);

  EXPECT_NE(node, nullptr);
  EXPECT_EQ(node->getNumberOfInputs(), 1);
  EXPECT_EQ(node->getNumberOfOutputs(), 1);
  EXPECT_EQ(node->getChannelCount(), 2);
  EXPECT_EQ(node->getChannelCountMode(), web_audio::ChannelCountMode::eMax);
  EXPECT_EQ(node->getChannelInterpretation(),
            web_audio::ChannelInterpretation::eSpeakers);

  EXPECT_EQ(node->getType(), web_audio::BiquadFilterType::eLowpass);
  EXPECT_NE(node->getFrequency(), nullptr);
  EXPECT_NE(node->getDetune(), nullptr);
  EXPECT_NE(node->getQ(), nullptr);
  EXPECT_NE(node->getGain(), nullptr);
}

void testGetFrequencyResponse(const web_audio::BiquadFilterOptions &options,
                              const std::vector<float> &frequencyHz,
                              const std::vector<float> &expectedMagResponse,
                              const std::vector<float> &expectedPhaseResponse) {
  auto context = web_audio::AudioContext::create();
  auto node = web_audio::BiquadFilterNode::create(context, options);
  std::vector<float> magResponse(frequencyHz.size());
  std::vector<float> phaseResponse(frequencyHz.size());
  node->getFrequencyResponse(frequencyHz, magResponse, phaseResponse);
  for (std::size_t i = 0; i < frequencyHz.size(); ++i) {
    EXPECT_NEAR(magResponse[i], expectedMagResponse[i], 0.01)
        << "frequencyHz[" << i << "] = " << frequencyHz[i];
    EXPECT_NEAR(phaseResponse[i], expectedPhaseResponse[i], 0.01)
        << "frequencyHz[" << i << "] = " << frequencyHz[i];
  }
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Lowpass) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.002, 1.004, 1.008, 1.012, 1.018, 1.024, 1.032, 1.040, 1.049, 1.184,
      1.237, 0.940, 0.608, 0.405, 0.286, 0.213, 0.165, 0.131, 0.031, 0.013,
      0.007, 0.005, 0.003, 0.002, 0.002, 0.001, 0.001, 0.000,
  };
  std::vector<float> expectedPhaseResponse = {
      -0.003, -0.005, -0.008, -0.010, -0.013, -0.015, -0.018, -0.020,
      -0.023, -0.025, -0.051, -0.077, -0.103, -0.129, -0.156, -0.184,
      -0.212, -0.241, -0.270, -0.647, -1.236, -1.863, -2.256, -2.475,
      -2.606, -2.693, -2.754, -2.801, -2.983, -3.038, -3.065, -3.081,
      -3.092, -3.100, -3.106, -3.111, -3.115, -3.136,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eLowpass;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Highpass) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.001, 0.001, 0.001,
      0.003, 0.007, 0.013, 0.021, 0.030, 0.041, 0.054, 0.069, 0.086, 0.387,
      0.908, 1.228, 1.241, 1.190, 1.146, 1.114, 1.091, 1.074, 1.018, 1.008,
      1.004, 1.003, 1.002, 1.001, 1.001, 1.001, 1.001, 1.000,
  };
  std::vector<float> expectedPhaseResponse = {
      3.139, 3.137, 3.134, 3.131, 3.129, 3.126, 3.124, 3.121, 3.119, 3.116,
      3.091, 3.065, 3.039, 3.012, 2.985, 2.958, 2.930, 2.901, 2.871, 2.494,
      1.905, 1.279, 0.885, 0.667, 0.536, 0.449, 0.387, 0.341, 0.159, 0.104,
      0.077, 0.060, 0.049, 0.041, 0.035, 0.031, 0.027, 0.005,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eHighpass;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Bandpass) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      0.003, 0.006, 0.009, 0.011, 0.014, 0.017, 0.020, 0.023, 0.026, 0.029,
      0.057, 0.086, 0.115, 0.144, 0.174, 0.204, 0.234, 0.265, 0.297, 0.647,
      0.955, 0.966, 0.808, 0.662, 0.554, 0.475, 0.416, 0.370, 0.177, 0.116,
      0.086, 0.068, 0.055, 0.047, 0.040, 0.034, 0.030, 0.006,
  };
  std::vector<float> expectedPhaseResponse = {
      1.568,  1.565,  1.562,  1.559,  1.557,  1.554,  1.551,  1.548,
      1.545,  1.542,  1.514,  1.485,  1.456,  1.426,  1.396,  1.365,
      1.334,  1.302,  1.269,  0.867,  0.300,  -0.262, -0.630, -0.847,
      -0.983, -1.075, -1.142, -1.192, -1.393, -1.455, -1.485, -1.503,
      -1.515, -1.524, -1.531, -1.536, -1.541, -1.565,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eBandpass;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Lowshelf) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
  };
  std::vector<float> expectedPhaseResponse = {
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eLowshelf;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Highshelf) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
  };
  std::vector<float> expectedPhaseResponse = {
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eHighshelf;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Peaking) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
  };
  std::vector<float> expectedPhaseResponse = {
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
      0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::ePeaking;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Notch) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      0.998, 0.996, 0.993, 0.990, 0.985, 0.979, 0.972, 0.964, 0.955, 0.763,
      0.296, 0.259, 0.589, 0.749, 0.832, 0.880, 0.909, 0.929, 0.984, 0.993,
      0.996, 0.998, 0.998, 0.999, 0.999, 0.999, 1.000, 1.000,
  };
  std::vector<float> expectedPhaseResponse = {
      -0.003, -0.006, -0.009, -0.011, -0.014, -0.017, -0.020, -0.023,
      -0.026, -0.029, -0.057, -0.086, -0.115, -0.145, -0.175, -0.205,
      -0.237, -0.269, -0.302, -0.703, -1.271, 1.309,  0.941,  0.724,
      0.588,  0.495,  0.429,  0.379,  0.178,  0.116,  0.086,  0.068,
      0.055,  0.047,  0.040,  0.034,  0.030,  0.006,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eNotch;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_Allpass) {
  std::vector<float> frequencyHz = {
      1.000,    2.000,     3.000,     4.000,    5.000,    6.000,    7.000,
      8.000,    9.000,     10.000,    20.000,   30.000,   40.000,   50.000,
      60.000,   70.000,    80.000,    90.000,   100.000,  200.000,  300.000,
      400.000,  500.000,   600.000,   700.000,  800.000,  900.000,  1000.000,
      2000.000, 3000.000,  4000.000,  5000.000, 6000.000, 7000.000, 8000.000,
      9000.000, 10000.000, 20000.000,
  };
  // From Firefox
  std::vector<float> expectedMagResponse = {
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
      1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
  };
  std::vector<float> expectedPhaseResponse = {
      -0.006, -0.011, -0.017, -0.023, -0.029, -0.034, -0.040, -0.046,
      -0.051, -0.057, -0.115, -0.172, -0.231, -0.290, -0.350, -0.411,
      -0.473, -0.537, -0.603, -1.407, -2.541, 2.618,  1.882,  1.447,
      1.175,  0.991,  0.858,  0.758,  0.355,  0.232,  0.172,  0.135,
      0.111,  0.093,  0.079,  0.069,  0.060,  0.012,
  };
  web_audio::BiquadFilterOptions options;
  options.type = web_audio::BiquadFilterType::eAllpass;
  testGetFrequencyResponse(options, frequencyHz, expectedMagResponse,
                           expectedPhaseResponse);
}

TEST(BiquadFilterNodeTest, GetFrequencyResnpose_InvalidLength) {
  auto context = web_audio::AudioContext::create();
  auto node = web_audio::BiquadFilterNode::create(context);
  std::vector<float> frequencyHz = {100.0, 200.0, 300.0};
  std::vector<float> magResponse(2);
  std::vector<float> phaseResponse(3);
  EXPECT_THROW(
      node->getFrequencyResponse(frequencyHz, magResponse, phaseResponse),
      web_audio::DOMException);
  magResponse.resize(3);
  phaseResponse.resize(2);
  EXPECT_THROW(
      node->getFrequencyResponse(frequencyHz, magResponse, phaseResponse),
      web_audio::DOMException);
}

TEST(BiquadFilterNodeTest, GetFrequencyResponse_OverSampleRate) {
  auto context = web_audio::AudioContext::create({.sampleRate = 44100});
  auto node = web_audio::BiquadFilterNode::create(context);
  std::vector<float> frequencyHz = {100.0, 200.0, 300.0, 50000.0};
  std::vector<float> magResponse(4);
  std::vector<float> phaseResponse(4);
  node->getFrequencyResponse(frequencyHz, magResponse, phaseResponse);
  EXPECT_TRUE(std::isfinite(magResponse[0]));
  EXPECT_TRUE(std::isfinite(magResponse[1]));
  EXPECT_TRUE(std::isfinite(magResponse[2]));
  EXPECT_TRUE(std::isnan(magResponse[3]));
}