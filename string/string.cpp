#include "string.hpp"

#include <cstring>

static const int kMultipler = 2;

static int Max(int a, int b) { return a > b ? a : b; }

String::~String() {
  if (capacity_ != 0) {
    delete[] s_;
  }
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity <= capacity_) {
    return;
  }
  capacity_ = new_capacity;
  s_ = new char[capacity_];
}

String::String(const char* c_string) {
  size_t new_size = Max(1, strlen(c_string));
  // TODO:
  // rezize data array
  Reserve(new_size * kMultipler);
  memcpy(s_, c_string, new_size);
  size_ = new_size;
};

std::istream& operator>>(std::istream& input, String& str) {
  const size_t kMinSize = Max(str.capacity_, 40);
  str.Reserve(kMinSize);
  str.s_ = new char[kMinSize];

  char ch;
  input.get(ch);
  size_t i = 0;
  while (EOF != ch && ch != '\n') {
    str.s_[i++] = ch;
    if (i >= str.capacity_) {
      str.capacity_ *= kMultipler;
      str.s_ = new char[str.capacity_];
    }
    input.get(ch);
  }

  str.size_ = i;
  str.s_[i] = '\0';

  return input;
}
