#include <gtest/gtest.h>

#include "web_audio.hh"

TEST(TestPromise, Then) {
  web_audio::detail::EventQueue queue;
  web_audio::Promise<int> promise(queue);
  bool called = false;

  promise.then([&](int value) {
    called = true;
    EXPECT_EQ(value, 42);
  });

  promise.getInternal()->resolve(42);
  queue.poll();
  EXPECT_TRUE(called);
}

TEST(TestPromise, Catch) {
  web_audio::detail::EventQueue queue;
  web_audio::Promise<int> promise(queue);
  bool called = false;

  promise.catch_([&](std::exception_ptr exception) {
    called = true;
    try {
      if (exception) {
        std::rethrow_exception(exception);
      }
    } catch (const std::runtime_error &e) {
      EXPECT_STREQ(e.what(), "Test error");
    }
  });

  promise.getInternal()->reject(
      std::make_exception_ptr(std::runtime_error("Test error")));
  queue.poll();
  EXPECT_TRUE(called);
}

TEST(TestPromise, ThenAfterResolve) {
  web_audio::detail::EventQueue queue;
  web_audio::Promise<int> promise(queue);
  bool called = false;

  promise.getInternal()->resolve(42);

  promise.then([&](int value) {
    called = true;
    EXPECT_EQ(value, 42);
  });

  EXPECT_TRUE(called);
}

TEST(TestPromise, CatchAfterReject) {
  web_audio::detail::EventQueue queue;
  web_audio::Promise<int> promise(queue);
  bool called = false;

  promise.getInternal()->reject(
      std::make_exception_ptr(std::runtime_error("Test error")));

  promise.catch_([&](std::exception_ptr exception) {
    called = true;
    try {
      if (exception) {
        std::rethrow_exception(exception);
      }
    } catch (const std::runtime_error &e) {
      EXPECT_STREQ(e.what(), "Test error");
    }
  });

  EXPECT_TRUE(called);
}