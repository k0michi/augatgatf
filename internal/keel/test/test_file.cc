#include "kl/io/file.hh"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

using namespace kl::io;

namespace {
constexpr std::string_view testFilePath = "test.txt";
void createTestFile(std::string_view path, const std::vector<std::byte> &data) {
  std::ofstream ofs(std::string(path), std::ios::binary);
  ofs.write(reinterpret_cast<const char *>(data.data()), data.size());
}
void removeTestFile(std::string_view path) {
  std::filesystem::remove(std::string(path));
}
} // namespace

class FileTest : public ::testing::Test {
protected:
  std::vector<std::byte> content = {
      std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF},
      std::byte{0x00}, std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
  std::vector<std::byte> writeBuffer = {std::byte{0xAA}, std::byte{0xBB},
                                        std::byte{0xCC}};
  void SetUp() override { createTestFile(testFilePath, content); }
  void TearDown() override { removeTestFile(testFilePath); }
};

TEST_F(FileTest, Open) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult =
        co_await File::open(testFilePath, OpenFlag::eRdWr | OpenFlag::eCreat);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, Read) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdOnly);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    std::vector<std::byte> buffer(1024);
    auto readResult = co_await file->read(loop, buffer);
    EXPECT_TRUE(readResult.has_value());
    std::size_t nread = *readResult;
    std::vector<std::byte> readContent(buffer.begin(), buffer.begin() + nread);
    EXPECT_EQ(readContent, content);
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, Write) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdWr);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto writeResult = co_await file->write(loop, writeBuffer);
    EXPECT_TRUE(writeResult.has_value());
    EXPECT_EQ(*writeResult, writeBuffer.size());
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, Flush) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdWr);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto flushResult = co_await file->flush(loop);
    EXPECT_TRUE(flushResult.has_value());
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, Seek) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdWr);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto seekResult = co_await file->seek(loop, 0, SeekDirection::eBegin);
    EXPECT_TRUE(seekResult.has_value());
    EXPECT_EQ(*seekResult, 0);
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, SeekRead) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdOnly);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto seekResult = co_await file->seek(loop, 4, SeekDirection::eBegin);
    EXPECT_TRUE(seekResult.has_value());
    EXPECT_EQ(*seekResult, 4);
    std::vector<std::byte> buffer(content.size() - 4);
    auto readResult = co_await file->read(loop, buffer);
    EXPECT_TRUE(readResult.has_value());
    std::size_t nread = *readResult;
    std::vector<std::byte> readContent(buffer.begin(), buffer.begin() + nread);
    std::vector<std::byte> expected(content.begin() + 4, content.end());
    EXPECT_EQ(readContent, expected);
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, SeekEndIsFileSize) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdOnly);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto seekResult = co_await file->seek(loop, 0, SeekDirection::eEnd);
    EXPECT_TRUE(seekResult.has_value());
    EXPECT_EQ(*seekResult, content.size());
    auto closeResult = co_await file->close(loop);
    EXPECT_TRUE(closeResult.has_value());
  }();
  loop->run();
}

TEST_F(FileTest, CloseTwice) {
  auto loop = Loop::getDefault();
  [&]() -> kl::concurrent::Task<void> {
    auto openResult = co_await File::open(testFilePath, OpenFlag::eRdOnly);
    EXPECT_TRUE(openResult.has_value());
    auto file = *openResult;
    auto closeResult1 = co_await file->close(loop);
    EXPECT_TRUE(closeResult1.has_value());
    auto closeResult2 = co_await file->close(loop);
    EXPECT_TRUE(closeResult2.has_value());
  }();
  loop->run();
}