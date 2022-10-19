#pragma once

#include <cstddef>

class RingBuffer {
  int* item_;
  int capacity_;
  int size_ = 0;
  int begin_ = 0;
  int end_ = 0;

 public:
  explicit RingBuffer(size_t capacity) : capacity_(capacity) {
    item_ = new int[capacity];
  }

  ~RingBuffer() { delete[] item_; }

  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  bool TryPush(int element) {
    int end = (end_ + 1) % capacity_;
    if (end == begin_) {
      return false;
    }
    item_[end] = element;
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