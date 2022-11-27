#include "string.hpp"

#include <cstring>
#include <vector>

static int Max(int a, int b) { return a > b ? a : b; }

String::~String() { delete[] s_; }

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
  Reserve(new_size + 1);
  size_ = new_size;
  s_[size_] = '\0';
}

void String::Resize(size_t new_size, char character) {
  Reserve(new_size + 1);
  if (new_size > size_) {
    memset(s_ + size_, character, new_size - size_);
  }
  size_ = new_size;
  s_[size_] = '\0';
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity <= capacity_) {
    return;
  }
  new_capacity = Max(new_capacity, capacity_ * 2);
  char* old = s_;
  s_ = new char[new_capacity + 1]();
  if (size_ != 0) {
    memcpy(s_, old, size_);
  }
  delete[] old;

  capacity_ = new_capacity;
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    char* old = s_;
    s_ = new char[size_ + 1]();
    if (size_ != 0) {
      memcpy(s_, old, size_);
    }
    delete[] old;
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
  while (c == ' ' || c == '\0') {
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
  output << s.Data();
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

std::vector<String> String::Split(const String& delim) const {
  std::vector<String> res;
  const String& s = *this;
  if (s.Empty() || delim.Empty()) {
    res.push_back(s);
  }
  String tmp;
  size_t j = 0;
  for (size_t i = 0; i < s.Size(); i++) {
    tmp.PushBack(s[i]);
    if (s[i] == delim[j]) {
      j++;
      if (j == delim.Size()) {
        size_t tmp_size = tmp.Size();
        tmp.Resize(tmp_size - delim.Size());
        res.push_back(tmp);
        tmp.Clear();
        j = 0;
        if (i == s.Size() - 1) {
          res.push_back(tmp);
        }
      }
    } else {
      j = 0;
    }
  }
  if (!tmp.Empty()) {
    res.push_back(tmp);
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
