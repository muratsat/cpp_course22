#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
  std::vector<int> item_;
  size_t capacity_;
  size_t size_ = 0;
  int begin_ = 0;
  int end_ = 0;

 public:
  explicit RingBuffer(size_t capacity) : capacity_(capacity) {
    item_.resize(capacity);
  }

  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  bool TryPush(int element) {
    if (size_ == capacity_) {
      return false;
    }
    item_[end_] = element;
    end_ = (end_ + 1) % capacity_;
    size_++;
    return true;
  }

  bool TryPop(int* element) {
    if (size_ == 0) {
      return false;
    }
    *element = item_[begin_];
    begin_ = (begin_ + 1) % capacity_;
    size_--;
    return true;
  }
};
