#include "string.hpp"

#include <cstring>

// Оказыватся, что, если при каждом переполнении массива увеличивать его
// вместимость в константное число раз, то можно добиться линейной зависимости
// числа операций над массивом от числа добавляемых элементов
// (https://en.wikipedia.org/wiki/Dynamic_array#Geometric_expansion_and_amortized_cost).
// В вашем решении будет проверяться корректность работы данной схемы при
// множителе равном
// 2 (то есть, если фактический размер изменяется так:
// 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> ...,
// то вместимость меняется следующим образом:
// 0 -> 1 -> 2 -> 4 -> 4 -> 8 -> ...).
static const int kMultipler = 2;

static const char kStringTerminator = '\0';

static int Max(int a, int b) { return a > b ? a : b; }

String::~String() {
  if (s_ != nullptr) {
    free(s_);
  }
}

String::String(size_t size, char character) { Resize(size, character); }

String::String(const char* c_string) {
  size_t new_size = strlen(c_string);
  Reserve(new_size * kMultipler);
  memcpy(s_, c_string, new_size);
  size_ = new_size;
};

String::String(String const& other) : String(other.s_) {
  Resize(other.size_);
  Reserve(other.capacity_);
}

String& String::operator=(const String& other) {
  if (&other == this) {
    *this = String(other);
  }
  return *this;
}

void String::Clear() { Resize(0); }

void String::PushBack(char character) {
  Resize(size_ + 1);
  s_[size_ - 1] = character;
}

void String::PopBack() { Resize(Max(size_ - 1, 0)); }

void String::Resize(size_t new_size) {
  size_t new_capacity = Max(new_size + 1, size_ * kMultipler);
  Reserve(new_capacity);
  if (new_size > size_) {
    memset(s_ + size_, kStringTerminator, new_size - size_);
  }
  size_ = new_size;
  s_[size_] = kStringTerminator;
}

void String::Resize(size_t new_size, char character) {
  Reserve(new_size + 1);
  if (new_size > size_) {
    memset(s_ + size_, character, new_size - size_);
  }
  size_ = new_size;
  s_[size_] = kStringTerminator;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity <= capacity_) {
    return;
  }
  if (capacity_ == 0) {
    s_ = (char*)malloc(new_capacity);
  } else {
    s_ = (char*)realloc(s_, new_capacity);
  }
  capacity_ = new_capacity;
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    s_ = (char*)realloc(s_, size_);
    capacity_ = size_;
  }
}

const char& String::operator[](size_t id) const { return s_[id]; }

char& String::operator[](size_t id) { return s_[id]; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

const char* String::Data() const { return s_; }
