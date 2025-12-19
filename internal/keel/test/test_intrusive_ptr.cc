#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "kl/common/intrusive_ptr.hh"

using namespace kl::common;

struct TestObject : public IntrusiveRefCounter<TestObject> {
  TestObject(std::int32_t &notifier)
      : IntrusiveRefCounter<TestObject>(), notifier_(notifier) {}

  ~TestObject() override { notifier_ = 42; }

  std::int32_t &notifier_;
};

TEST(IntrusivePtrTest, IntrusiveRefCounter) {
  std::int32_t destructionNotifier = 0;

  auto obj = new TestObject(destructionNotifier);
  EXPECT_EQ(obj->refCount(), 1u);

  obj->retain();
  EXPECT_EQ(obj->refCount(), 2u);

  obj->retain();
  EXPECT_EQ(obj->refCount(), 3u);

  obj->release();
  EXPECT_EQ(obj->refCount(), 2u);

  obj->release();
  EXPECT_EQ(obj->refCount(), 1u);

  obj->release();
  EXPECT_EQ(destructionNotifier, 42);
}

TEST(IntrusivePtrTest, IntrusivePtr) {
  std::int32_t destructionNotifier = 0;

  IntrusivePtr<TestObject> ptr1 =
      adoptIntrusive(new TestObject(destructionNotifier));
  EXPECT_EQ(ptr1->refCount(), 1u);

  {
    IntrusivePtr<TestObject> ptr2 = ptr1;
    EXPECT_EQ(ptr1->refCount(), 2u);
    EXPECT_EQ(ptr2->refCount(), 2u);

    {
      IntrusivePtr<TestObject> ptr3;
      ptr3 = ptr2;
      EXPECT_EQ(ptr1->refCount(), 3u);
      EXPECT_EQ(ptr2->refCount(), 3u);
      EXPECT_EQ(ptr3->refCount(), 3u);
    }

    EXPECT_EQ(ptr1->refCount(), 2u);
    EXPECT_EQ(ptr2->refCount(), 2u);
  }

  EXPECT_EQ(ptr1->refCount(), 1u);
  ptr1.reset();
  EXPECT_EQ(destructionNotifier, 42);
}

TEST(IntrusivePtrTest, IntrusivePtrMove) {
  std::int32_t destructionNotifier = 0;

  IntrusivePtr<TestObject> ptr1 =
      adoptIntrusive(new TestObject(destructionNotifier));
  EXPECT_EQ(ptr1->refCount(), 1u);

  IntrusivePtr<TestObject> ptr2 = std::move(ptr1);
  EXPECT_EQ(ptr2->refCount(), 1u);
  EXPECT_EQ(ptr1.get(), nullptr);

  IntrusivePtr<TestObject> ptr3;
  ptr3 = std::move(ptr2);
  EXPECT_EQ(ptr3->refCount(), 1u);
  EXPECT_EQ(ptr2.get(), nullptr);

  ptr3.reset();
  EXPECT_EQ(destructionNotifier, 42);
}

TEST(IntrusivePtrTest, MakeIntrusive) {
  std::int32_t destructionNotifier = 0;

  auto ptr = makeIntrusive<TestObject>(destructionNotifier);
  EXPECT_EQ(ptr->refCount(), 1u);

  ptr.reset();
  EXPECT_EQ(destructionNotifier, 42);
}

TEST(IntrusivePtrTest, WeakPtr) {
  std::int32_t destructionNotifier = 0;

  IntrusivePtr<TestObject> strongPtr =
      makeIntrusive<TestObject>(destructionNotifier);
  EXPECT_EQ(strongPtr->refCount(), 1u);

  WeakPtr<TestObject> weakPtr = strongPtr;
  EXPECT_FALSE(weakPtr.isExpired());

  {
    IntrusivePtr<TestObject> lockedPtr = weakPtr.lock();
    EXPECT_NE(lockedPtr.get(), nullptr);
    EXPECT_EQ(lockedPtr->refCount(), 2u);
  }

  EXPECT_EQ(strongPtr->refCount(), 1u);
  strongPtr.reset();
  EXPECT_TRUE(weakPtr.isExpired());
  EXPECT_EQ(destructionNotifier, 42);
  IntrusivePtr<TestObject> lockedPtr = weakPtr.lock();
  EXPECT_EQ(lockedPtr.get(), nullptr);
}

TEST(IntrusivePtrTest, ThreadSafety) {
  std::int32_t destructionNotifier = 0;
  auto ptr = makeIntrusive<TestObject>(destructionNotifier);
  WeakPtr<TestObject> weakPtr = ptr;

  std::atomic<bool> start{false};
  std::atomic<int> ready{0};
  constexpr int numThreads = 8;
  std::vector<std::thread> threads;

  for (int i = 0; i < numThreads; ++i) {
    threads.emplace_back([&] {
      ++ready;
      while (!start) {
        std::this_thread::yield();
      }
      for (int j = 0; j < 10000; ++j) {
        auto locked = weakPtr.lock();
        if (locked) {
          EXPECT_GE(locked->refCount(), 1u);
        }
      }
    });
  }

  threads.emplace_back([&] {
    ++ready;
    while (!start) {
      std::this_thread::yield();
    }
    for (int j = 0; j < 10000; ++j) {
      IntrusivePtr<TestObject> local = ptr;
      std::this_thread::yield();
      local.reset();
    }
  });

  threads.emplace_back([&] {
    ++ready;
    while (!start) {
      std::this_thread::yield();
    }
    for (int j = 0; j < 10000; ++j) {
      ptr->retain();
    }
    for (int j = 0; j < 10000; ++j) {
      ptr->release();
    }
  });

  while (ready < numThreads + 2) {
    std::this_thread::yield();
  }
  start = true;

  for (auto &t : threads)
    t.join();

  ptr.reset();
  EXPECT_EQ(destructionNotifier, 42);
  EXPECT_TRUE(weakPtr.isExpired());
}