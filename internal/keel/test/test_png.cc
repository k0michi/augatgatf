#include <gtest/gtest.h>

#include "kl/assets/png.hh"
#include "kl/io/file.hh"

class PngTest : public ::testing::Test {
protected:
  std::shared_ptr<kl::io::Loop> loop = kl::io::Loop::getDefault();
};

TEST_F(PngTest, LoadPngImage) {
  [&]() -> kl::concurrent::Task<void> {
    auto fileExpected = co_await kl::io::File::open(loop, "test_data/test.png",
                                                    kl::io::OpenFlag::eRdOnly);
    EXPECT_TRUE(fileExpected.has_value());
    auto file = *fileExpected;
    auto imageExpected = co_await kl::assets::readPNG(loop, *file);
    EXPECT_TRUE(imageExpected.has_value());
    auto image = *imageExpected;
    EXPECT_EQ(image.format(), kl::assets::ColorFormat::eR8G8B8A8Unorm);
    EXPECT_EQ(image.extent().width, 16u);
    EXPECT_EQ(image.extent().height, 16u);
  }();
  loop->run();
}