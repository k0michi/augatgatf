#ifndef KL_IO_PATH_HH
#define KL_IO_PATH_HH

#include <cstring>

#include "kl/io/async_stream.hh"

namespace kl::io {
class BufferedReader final {
public:
  static constexpr std::size_t kDefaultBufferSize = 8192;

  template <AsyncReadable T>
  explicit BufferedReader(T &readable,
                          std::size_t bufferSize = kDefaultBufferSize)
      : readable_(std::make_unique<Wrapper<T>>(readable)), buffer_(bufferSize),
        head_(0), tail_(0) {}

  kl::concurrent::Task<Expected<std::size_t>>
  read(std::shared_ptr<Loop> loop, std::span<std::byte> buffer) {
    std::size_t totalRead = 0;

    while (totalRead < buffer.size()) {
      if (head_ == tail_) {
        // Buffer is empty
        head_ = 0;
        tail_ = 0;
        auto nread = co_await readable_->read(
            loop, std::span{buffer_.data(), buffer_.size()});
        if (!nread) {
          co_return std::unexpected(nread.error());
        }
        if (*nread == 0) {
          // EOF
          break;
        }
        tail_ += *nread;
      }

      std::size_t toCopy = std::min(buffer.size() - totalRead, tail_ - head_);
      std::memcpy(buffer.data() + totalRead, buffer_.data() + head_, toCopy);
      head_ += toCopy;
      totalRead += toCopy;
    }

    co_return totalRead;
  }

private:
  template <AsyncReadable T> class Wrapper : public IAsyncReadable {
  public:
    explicit Wrapper(T &inner) : inner_(inner) {}

    kl::concurrent::Task<Expected<std::size_t>>
    read(std::shared_ptr<kl::io::Loop> loop,
         std::span<std::byte> buffer) override {
      co_return co_await inner_.read(loop, buffer);
    }

  private:
    T &inner_;
  };

  std::unique_ptr<IAsyncReadable> readable_;
  std::vector<std::byte> buffer_;
  std::size_t head_;
  std::size_t tail_;
};

class BufferedWriter final {
public:
  static constexpr std::size_t kDefaultBufferSize = 8192;

  template <AsyncWritable T>
  explicit BufferedWriter(T &writable,
                          std::size_t bufferSize = kDefaultBufferSize)
      : writable_(std::make_unique<Wrapper<T>>(writable)), buffer_(bufferSize),
        position_(0) {}

  kl::concurrent::Task<Expected<std::size_t>>
  write(std::shared_ptr<Loop> loop, std::span<const std::byte> buffer) {
    std::size_t totalWritten = 0;

    while (totalWritten < buffer.size()) {
      std::size_t spaceLeft = buffer_.size() - position_;

      if (spaceLeft == 0) {
        // Buffer is full
        auto flushResult = co_await flushInternal(loop);
        if (!flushResult) {
          co_return std::unexpected(flushResult.error());
        }
        spaceLeft = buffer_.size();
      }

      std::size_t toCopy = std::min(buffer.size() - totalWritten, spaceLeft);
      std::memcpy(buffer_.data() + position_, buffer.data() + totalWritten,
                  toCopy);
      position_ += toCopy;
      totalWritten += toCopy;
    }

    co_return totalWritten;
  }

  kl::concurrent::Task<Expected<void>> flush(std::shared_ptr<Loop> loop) {
    auto flushResult = co_await flushInternal(loop);

    if (!flushResult) {
      co_return std::unexpected(flushResult.error());
    }

    co_return co_await writable_->flush(loop);
  }

private:
  kl::concurrent::Task<Expected<void>>
  flushInternal(std::shared_ptr<Loop> loop) {
    std::size_t totalWritten = 0;

    while (totalWritten < position_) {
      auto nwritten = co_await writable_->write(
          loop,
          std::span{buffer_.data() + totalWritten, position_ - totalWritten});

      if (!nwritten) {
        co_return std::unexpected(nwritten.error());
      }

      totalWritten += *nwritten;
    }

    position_ = 0;
    co_return {};
  }

  template <AsyncWritable T> class Wrapper : public IAsyncWritable {
  public:
    explicit Wrapper(T &inner) : inner_(inner) {}

    kl::concurrent::Task<Expected<std::size_t>>
    write(std::shared_ptr<kl::io::Loop> loop,
          std::span<const std::byte> buffer) override {
      co_return co_await inner_.write(loop, buffer);
    }

    kl::concurrent::Task<Expected<void>>
    flush(std::shared_ptr<kl::io::Loop> loop) override {
      co_return co_await inner_.flush(loop);
    }

  private:
    T &inner_;
  };

  std::unique_ptr<IAsyncWritable> writable_;
  std::vector<std::byte> buffer_;
  std::size_t position_;
};
} // namespace kl::io
#endif