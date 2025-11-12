#include "kl/io/loop.hh"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

using namespace kl::io;

TEST(LoopTest, CreateAndDestroy) {
  auto loop = Loop::create();
  ASSERT_NE(loop, nullptr);
  ASSERT_NE(loop->getUVLoop(), nullptr);
}

TEST(LoopTest, GetDefaultLoopSingleton) {
  auto loop1 = Loop::getDefault();
  auto loop2 = Loop::getDefault();
  ASSERT_NE(loop1, nullptr);
  ASSERT_NE(loop2, nullptr);
  ASSERT_EQ(loop1->getUVLoop(), loop2->getUVLoop());
  ASSERT_EQ(loop1, loop2);
}

TEST(LoopTest, ThreadLocalDefaultLoopIsolated) {
  constexpr int kNumThreads = 8;
  std::vector<std::shared_ptr<Loop>> loops(kNumThreads);
  std::vector<std::thread> threads;

  for (int i = 0; i < kNumThreads; ++i) {
    threads.emplace_back([&, i] { loops[i] = Loop::getDefault(); });
  }

  for (auto &t : threads)
    t.join();

  for (int i = 0; i < kNumThreads; ++i) {
    for (int j = i + 1; j < kNumThreads; ++j) {
      EXPECT_NE(loops[i], loops[j]);
      EXPECT_NE(loops[i]->getUVLoop(), loops[j]->getUVLoop());
    }
  }
}