#pragma once

#include <memory>
#include <stack>
#include <vector>

namespace otus {

template <class T, size_t pool_size>
struct Allocator {
  using value_type = T;
  std::shared_ptr<T> pool_ = nullptr;
  std::stack<T*, std::vector<T*>> slots_;

  Allocator() noexcept : pool_(new T[pool_size * sizeof(T)]) {
    T* begin = pool_.get();
    const T* end = begin + pool_size;

    while (begin != end) slots_.push(begin++);
  };

  template <class U, size_t sz>
  Allocator(const Allocator<U, sz>& a) noexcept {
    pool_ = a.pool_;
    slots_ = a.slots_;
  }

  Allocator select_on_container_copy_construction() const {
    return Allocator(pool_size);
  }

  T* allocate(std::size_t n) {
    if (n > max_size()) throw std::invalid_argument("Not enough memory");

    if (slots_.empty()) throw std::bad_alloc();

    auto p = slots_.top();
    slots_.pop();
    return p;
  }

  void deallocate(T* p, std::size_t n) {
    if (n > max_size()) throw std::invalid_argument("Not enough memory");

    if (slots_.size() == pool_size)
      throw std::runtime_error("Pool is fully free");

    slots_.push(p);
  }

  std::size_t max_size() { return pool_size; }

  template <class U>
  struct rebind {
    typedef Allocator<U, pool_size> other;
  };

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;
};

template <class T, class U, size_t sz>
constexpr bool operator==(const Allocator<T, sz>& a1,
                          const Allocator<U, sz>& a2) noexcept {
  return a1.pool == a2.pool;
}

template <class T, class U, size_t sz>
constexpr bool operator!=(const Allocator<T, sz>& a1,
                          const Allocator<U, sz>& a2) noexcept {
  return a1.pool != a2.pool;
}

}  // namespace otus
