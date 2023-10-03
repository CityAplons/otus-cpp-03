#pragma once

#include <iterator>

namespace otus {

template <typename T, typename Allocator = std::allocator<T>>
class Vector {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef Allocator allocator_type;
  typedef typename Allocator::reference reference;
  typedef typename Allocator::const_reference const_reference;
  typedef typename Allocator::difference_type difference_type;
  typedef typename Allocator::size_type size_type;

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    Iterator(pointer ptr) : m_ptr_(ptr) {}

    reference operator*() const { return *m_ptr_; }
    pointer operator->() { return m_ptr_; }
    Iterator& operator++() {
      m_ptr_++;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ == b.m_ptr_;
    };
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.m_ptr_ != b.m_ptr_;
    };

   private:
    pointer m_ptr_;
  };

  Iterator begin() { return Iterator(data_); }
  Iterator end() { return Iterator(data_ + size_); }
  size_type size() { return size_; }

  void push_back(const T& val) {
    if (size_ == capacity_) {
      size_type newCapacity = capacity_ * 2 + 1;
      pointer newData =
          std::allocator_traits<Allocator>::allocate(allocator_, newCapacity);

      for (size_t i = 0; i < capacity_; ++i) {
        newData[i] = std::move(data_[i]);
      }

      capacity_ = newCapacity;
      std::swap(newData, data_);
    }

    data_[size_++] = val;
  }

 private:
  size_type size_ = 0;
  size_type capacity_ = 0;
  pointer data_ = nullptr;
  Allocator allocator_;
};

}  // namespace otus
