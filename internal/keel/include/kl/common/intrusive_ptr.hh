#ifndef KL_COMMON_INTRUSIVE_PTR_HH
#define KL_COMMON_INTRUSIVE_PTR_HH

#include <atomic>
#include <mutex>
#include <utility>

namespace kl::common {
template <typename T> class WeakReference final {
private:
  std::mutex mutex_;
  T *ptr_ = nullptr;
  std::atomic<std::size_t> refCount_;

public:
  WeakReference(T *ptr) : ptr_(ptr), refCount_(1) {}

  void retain() { refCount_.fetch_add(1, std::memory_order_relaxed); }

  void release() {
    if (refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      delete this;
    }
  }

  void clear() {
    std::lock_guard lock(mutex_);
    ptr_ = nullptr;
  }

  T *lockAndRetain() {
    std::lock_guard lock(mutex_);

    if (!ptr_) {
      return nullptr;
    }

    // Try to retain the pointer
    if (ptr_->tryRetain()) {
      return ptr_;
    }

    return nullptr;
  }

  bool isExpired() {
    std::lock_guard lock(mutex_);
    return ptr_ == nullptr;
  }
};

template <typename T> class IntrusiveRefCounter {
private:
  mutable std::atomic<std::size_t> refCount_;
  mutable std::atomic<WeakReference<T> *> weakRef_;

public:
  IntrusiveRefCounter() : refCount_(1), weakRef_(nullptr) {}

  virtual ~IntrusiveRefCounter() {
    auto weakRef = weakRef_.load(std::memory_order_acquire);

    if (weakRef) {
      weakRef->clear();
      weakRef->release();
    }
  }

  /**
   * @brief Retain the object by incrementing the reference count. This method must not be called unless the object is known to be alive.
   */
  void retain() const { refCount_.fetch_add(1, std::memory_order_relaxed); }

  /**
   * @brief Try to retain the object. Returns false if the object is already
   * being destroyed, that is, refCount_ is 0. Otherwise, increments the refCount_
   * and returns true.
   * @return true 
   * @return false 
   */
  bool tryRetain() {
    std::size_t count = refCount_.load(std::memory_order_acquire);

    do {
      if (count == 0) {
        return false;
      }
    } while (!refCount_.compare_exchange_weak(count, count + 1,
                                              std::memory_order_acquire,
                                              std::memory_order_relaxed));

    return true;
  }

  /**
   * @brief Release the object by decrementing the reference count. If the reference count reaches zero, the object is deleted.
   * 
   */
  void release() const {
    if (refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      delete static_cast<const T *>(this);
    }
  }

  WeakReference<T> *getWeakReference() const {
    WeakReference<T> *ref = weakRef_.load(std::memory_order_acquire);

    if (!ref) {
      WeakReference<T> *newRef = new WeakReference<T>(
          static_cast<T *>(const_cast<IntrusiveRefCounter<T> *>(this)));

      WeakReference<T> *expected = nullptr;
      if (weakRef_.compare_exchange_strong(expected, newRef,
                                           std::memory_order_release,
                                           std::memory_order_acquire)) {
        ref = newRef;
      } else {
        delete newRef;
        ref = expected;
      }
    }

    return ref;
  }

  std::size_t refCount() const {
    return refCount_.load(std::memory_order_relaxed);
  }
};

template <typename T> class IntrusivePtr;
template <typename T, typename... Args>
IntrusivePtr<T> makeIntrusive(Args &&...args);
template <typename T> IntrusivePtr<T> adoptIntrusive(T *ptr) noexcept;

template <typename T> class IntrusivePtr final {
private:
  T *ptr_ = nullptr;

public:
  IntrusivePtr() noexcept : ptr_(nullptr) {}

  IntrusivePtr(std::nullptr_t) noexcept : ptr_(nullptr) {}

  /**
   * @brief Constructs an IntrusivePtr that takes ownership of the provided raw pointer.
   * 
   * @param ptr 
   */
  explicit IntrusivePtr(T *ptr) noexcept : ptr_(ptr) {
    if (ptr_) {
      ptr_->retain();
    }
  }

  IntrusivePtr(const IntrusivePtr &other) noexcept : ptr_(other.ptr_) {
    if (ptr_) {
      ptr_->retain();
    }
  }

  IntrusivePtr &operator=(const IntrusivePtr &other) noexcept {
    IntrusivePtr temp(other);
    swap(*this, temp);
    return *this;
  }

  IntrusivePtr(IntrusivePtr &&other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  IntrusivePtr &operator=(IntrusivePtr &&other) noexcept {
    IntrusivePtr temp(std::move(other));
    swap(*this, temp);
    return *this;
  }

  ~IntrusivePtr() noexcept {
    if (ptr_) {
      ptr_->release();
    }
  }

  friend void swap(IntrusivePtr &first, IntrusivePtr &second) noexcept {
    using std::swap;
    swap(first.ptr_, second.ptr_);
  }

  T *get() const noexcept { return ptr_; }

  T &operator*() const noexcept { return *ptr_; }

  T *operator->() const noexcept { return ptr_; }

  explicit operator bool() const noexcept { return ptr_ != nullptr; }

  void reset() noexcept {
    IntrusivePtr temp;
    swap(*this, temp);
  }

  template <typename U, typename... Args>
  friend IntrusivePtr<U> makeIntrusive(Args &&...args);

  template <typename U> friend IntrusivePtr<U> adoptIntrusive(U *ptr) noexcept;
};

template <typename T> IntrusivePtr<T> adoptIntrusive(T *ptr) noexcept {
  IntrusivePtr<T> intrusivePtr;
  intrusivePtr.ptr_ = ptr;
  return intrusivePtr;
}

template <typename T, typename... Args>
IntrusivePtr<T> makeIntrusive(Args &&...args) {
  return adoptIntrusive(new T(std::forward<Args>(args)...));
}

template <typename T> class WeakPtr final {
private:
  WeakReference<T> *weakRef_ = nullptr;

public:
  WeakPtr() noexcept : weakRef_(nullptr) {}

  WeakPtr(std::nullptr_t) noexcept : weakRef_(nullptr) {}

  WeakPtr(T *ptr) noexcept {
    if (ptr) {
      weakRef_ = ptr->getWeakReference();
      weakRef_->retain();
    } else {
      weakRef_ = nullptr;
    }
  }

  WeakPtr(const IntrusivePtr<T> &ptr) noexcept {
    if (ptr.get()) {
      weakRef_ = ptr.get()->getWeakReference();
      weakRef_->retain();
    } else {
      weakRef_ = nullptr;
    }
  }

  WeakPtr(const WeakPtr &other) noexcept : weakRef_(other.weakRef_) {
    if (weakRef_) {
      weakRef_->retain();
    }
  }

  WeakPtr &operator=(const WeakPtr &other) noexcept {
    WeakPtr temp(other);
    swap(*this, temp);
    return *this;
  }

  WeakPtr(WeakPtr &&other) noexcept : weakRef_(other.weakRef_) {
    other.weakRef_ = nullptr;
  }

  WeakPtr &operator=(WeakPtr &&other) noexcept {
    WeakPtr temp(std::move(other));
    swap(*this, temp);
    return *this;
  }

  ~WeakPtr() noexcept {
    if (weakRef_) {
      weakRef_->release();
    }
  }

  friend void swap(WeakPtr &first, WeakPtr &second) noexcept {
    using std::swap;
    swap(first.weakRef_, second.weakRef_);
  }

  IntrusivePtr<T> lock() const noexcept {
    if (!weakRef_) {
      return nullptr;
    }

    T *ptr = weakRef_->lockAndRetain();

    if (ptr) {
      return adoptIntrusive(ptr);
    } else {
      return nullptr;
    }
  }

  bool isExpired() const noexcept { return !weakRef_ || weakRef_->isExpired(); }

  void reset() noexcept {
    WeakPtr temp;
    swap(*this, temp);
  }
};
} // namespace kl::common
#endif