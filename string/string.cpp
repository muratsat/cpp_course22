#include "string.hpp"

#include <cstring>
#include <vector>

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
  Resize(new_size);
  memcpy(s_, c_string, new_size);
}

String::String(String const& other) {
  Resize(other.Size());
  memcpy((void*)Data(), other.Data(), other.Size());
}

String& String::operator=(const String& other) {
  if (&other != this) {
    Resize(other.Size());
    memcpy((void*)Data(), other.Data(), other.Size());
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
  new_capacity = Capacity();
  if (new_capacity > new_size) {
    memset(s_ + new_size, kStringTerminator, new_capacity - new_size);
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
    if (capacity_ < new_capacity) {
      memset((void*)(Data() + capacity_), 0, new_capacity - capacity_);
    }
  }
  capacity_ = new_capacity;
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    s_ = (char*)realloc(s_, size_);
    capacity_ = size_;
  }
}

void String::Swap(String& other) {
  size_t tmp_size = other.size_;
  size_t tmp_capacity = other.capacity_;
  char* tmp_s = other.s_;
  other.size_ = size_;
  other.capacity_ = capacity_;
  other.s_ = s_;
  size_ = tmp_size;
  capacity_ = tmp_capacity;
  s_ = tmp_s;
}

static int Min(int a, int b) { return a < b ? a : b; }

bool operator<(const String& s1, const String& s2) {
  size_t size = Min(s1.Size(), s2.Size());
  for (size_t i = 0; i < size; i++) {
    if (s1[i] != s2[i]) {
      return s1[i] < s2[i];
    }
  }
  return s1.Size() < s2.Size();
}

bool operator>(const String& s1, const String& s2) { return (s2 < s1); }

bool operator<=(const String& s1, const String& s2) {
  size_t size = Min(s1.Size(), s2.Size());
  for (size_t i = 0; i < size; i++) {
    if (s1[i] > s2[i]) {
      return false;
    }
  }
  return s1.Size() <= s2.Size();
}

bool operator>=(const String& s1, const String& s2) { return (s2 <= s1); }

bool operator==(const String& s1, const String& s2) {
  if (s1.Size() != s2.Size()) {
    return false;
  }

  size_t size = s1.Size();

  for (size_t i = 0; i < size; i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const String& s1, const String& s2) { return !(s1 == s2); }

std::istream& operator>>(std::istream& input, String& s) {
  s.Clear();
  char c = 0;
  while (c == ' ' || c == kStringTerminator) {
    input.get(c);
  }

  if (!input) {
    return input;
  }

  do {
    s.PushBack(c);
    input.get(c);
  } while (input && c != ' ' && c != '\n');

  return input;
}

std::ostream& operator<<(std::ostream& output, const String& s) {
  for (size_t i = 0; i < s.Size(); i++) {
    output << s[i];
  }
  return output;
}

String& String::operator+=(const String& other) {
  size_t old_size = Size();
  size_t other_size = other.Size();
  Resize(old_size + other_size);
  memcpy((void*)(Data() + old_size), other.Data(), other_size);
  return *this;
}

String String::operator+(const String& other) const {
  String res = *this;
  res += other;
  return res;
}

String& String::operator*=(int n) {
  if (n <= 0) {
    Clear();
    return *this;
  }
  size_t old_size = Size();
  size_t new_size = Size() * n;
  Resize(new_size);

  for (size_t i = old_size; i < new_size; i++) {
    s_[i] = s_[i % old_size];
  }
  return *this;
}

String operator*(int n, const String& str) {
  String res = str;
  res *= n;
  return res;
}
String operator*(const String& str, int n) {
  String res = str;
  res *= n;
  return res;
}

// TODO
std::vector<String> String::Split(const String& delim) const {
  std::vector<String> res;
  const String& s = *this;
  if (s.Empty() || delim.Empty()) {
    res.push_back(s);
  }
  return res;
}

String String::Join(const std::vector<String>& strings) const {
  String res;
  if (strings.empty()) {
    res.Clear();
    return res;
  }
  for (size_t i = 0; i < strings.size(); i++) {
    res += strings[i];
    if (i < strings.size() - 1) {
      res += *this;
    }
  }
  return res;
}
