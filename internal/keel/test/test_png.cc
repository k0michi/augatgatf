#include <gtest/gtest.h>

#include "kl/assets/png.hh"
#include "kl/io/file.hh"
#include "kl/io/memory_stream.hh"

class PngTest : public ::testing::Test {
protected:
  std::shared_ptr<kl::io::Loop> loop = kl::io::Loop::create();
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

TEST_F(PngTest, ReadInvalidPngImage) {
  [&]() -> kl::concurrent::Task<void> {
    std::vector<std::byte> data{std::byte{'N'}, std::byte{'o'}, std::byte{'t'},
                                std::byte{' '}, std::byte{'a'}, std::byte{' '},
                                std::byte{'P'}, std::byte{'N'}, std::byte{'G'},
                                std::byte{' '}, std::byte{'f'}, std::byte{'i'},
                                std::byte{'l'}, std::byte{'e'}};
    kl::io::MemoryStream stream;
    co_await stream.write(loop,
                          std::span<const std::byte>(data.data(), data.size()));
    co_await stream.seek(loop, 0, kl::io::SeekDirection::eBegin);
    auto imageExpected = co_await kl::assets::readPNG(loop, stream);
    EXPECT_FALSE(imageExpected.has_value());
  }();
  loop->run();
}