#include <gtest/gtest.h>

#include "kl/concurrent/task.hh"

kl::concurrent::Task<void> fTaskVoid() { co_return; }

kl::concurrent::Task<int> fTaskInt() { co_return 42; }

kl::concurrent::Task<std::string> fTaskString() { co_return "Hello, World!"; }

kl::concurrent::Task<int> fTaskChain() {
  int value = co_await fTaskInt();
  co_await fTaskString();
  co_await fTaskVoid();
  co_return value + 1;
}

TEST(TaskTest, ImmediateReadyVoid) {
  auto task = fTaskVoid();
  EXPECT_TRUE(task.await_ready());
}

TEST(TaskTest, ImmediateReadyInt) {
  auto task = fTaskInt();
  EXPECT_TRUE(task.await_ready());
  EXPECT_EQ(task.await_resume(), 42);
}

TEST(TaskTest, ChainTasks) {
  auto task = fTaskChain();
  EXPECT_TRUE(task.await_ready());
  EXPECT_EQ(task.await_resume(), 43);
}

kl::concurrent::Task<int> fAwaitMultipleTimes() {
  auto task1 = fTaskInt();
  co_await task1;
  auto value1 = co_await task1;
  co_return value1;
}

TEST(TaskTest, AwaitMultipleTimes) {
  auto task = fAwaitMultipleTimes();
  EXPECT_TRUE(task.await_ready());
  EXPECT_EQ(task.await_resume(), 42);
}