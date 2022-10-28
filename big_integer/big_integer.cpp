#include "big_integer.hpp"

#include <iostream>

static int Max(int a, int b) { return a > b ? a : b; }

BigInt::BigInt(int64_t n) {
  if (n == 0) {
    return;
  }
  if (n < 0) {
    is_negative_ = true;
    // TODO: handle case n = -2^63
    n = -n;
  }
  digits_.push_back(n % kBase);
  if (n / kBase) {
    digits_.push_back(n / kBase);
  }
}

BigInt& BigInt::operator=(const BigInt& other) {
  digits_ = other.digits_;
  is_negative_ = other.is_negative_;
  return *this;
}

static int DigitValue(char digit) {
  if ('0' <= digit && digit <= '9') {
    return digit - '0';
  }
  if ('a' <= digit && digit <= 'z') {
    return digit - 'a' + 10;
  }
  return digit - 'A' + 10;
}

BigInt::BigInt(std::string s, int string_base) {
  BigInt dec(1);

  for (int i = s.size() - 1; i >= 0; i--) {
    if (s[i] == '-') {
      is_negative_ = !digits_.empty();
      break;
    }
    BigInt to_add = dec * DigitValue(s[i]);
    AddAbs(to_add);
    dec *= string_base;
  }
}

void BigInt::AddAbs(const BigInt& to_add) {
  size_t old_size = digits_.size();
  size_t max_size = Max(old_size, to_add.digits_.size());
  digits_.resize(max_size);

  int64_t carry = 0;
  for (size_t i = 0; i < max_size; i++) {
    int64_t tmp = carry;
    if (i < old_size) {
      tmp += digits_[i];
    }
    if (i < to_add.digits_.size()) {
      tmp += to_add[i];
    }

    digits_[i] = tmp % kBase;
    carry = tmp / kBase;
  }

  if (carry) {
    digits_.push_back(carry);
  }

  Normalize();
}

void BigInt::SubAbs(const BigInt& to_sub) {
  int cmp = CompareAbs(to_sub);
  size_t old_size = Size();
  size_t size = Max(old_size, to_sub.Size());
  digits_.resize(size);

  bool borrowed = false;
  for (size_t i = 0; i < size; i++) {
    long long tmp = borrowed ? -1 : 0;
    borrowed = false;
    if (i < old_size) {
      tmp += cmp * (long long)digits_[i];
    }
    if (i < to_sub.Size()) {
      tmp -= cmp * (long long)to_sub[i];
    }
    if (tmp < 0) {
      borrowed = true;
      tmp += kBase;
    }
    digits_[i] = tmp;
  }

  Normalize();
}

void BigInt::Normalize() {
  size_t new_size = digits_.size();
  while (new_size > 0 && digits_[new_size - 1] == 0) {
    new_size--;
  }
  digits_.resize(new_size);
  if (digits_.empty()) {
    is_negative_ = false;
  }
}

BigInt& BigInt::operator+=(const BigInt& other) {
  if (is_negative_ != other.is_negative_) {
    if (CompareAbs(other) < 0) {
      is_negative_ = !is_negative_;
    }
    SubAbs(other);
  } else {
    AddAbs(other);
  }

  return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
  if (is_negative_ == other.is_negative_) {
    if (CompareAbs(other) < 0) {
      is_negative_ = !is_negative_;
    }
    SubAbs(other);
  } else {
    AddAbs(other);
  }

  return *this;
}

int BigInt::CompareAbs(const BigInt& other) const {
  if (Size() < other.Size()) {
    return -1;
  }
  if (Size() > other.Size()) {
    return 1;
  }

  for (size_t i = 0; i < Size(); i++) {
    if (digits_[i] < other[i]) {
      return -1;
    }
    if (digits_[i] > other[i]) {
      return 1;
    }
  }

  return 0;
}

int BigInt::Compare(const BigInt& other) const {
  if (is_negative_ != other.is_negative_) {
    return is_negative_ ? -1 : 1;
  }
  int cmp = CompareAbs(other);
  cmp *= is_negative_ ? -1 : 1;
  return cmp;
}

bool operator<(const BigInt& left, const BigInt& right) {
  return left.Compare(right) < 0;
}

bool operator>(const BigInt& left, const BigInt& right) {
  return left.Compare(right) > 0;
}

bool operator<=(const BigInt& left, const BigInt& right) {
  return left.Compare(right) <= 0;
}

bool operator>=(const BigInt& left, const BigInt& right) {
  return left.Compare(right) >= 0;
}

bool operator==(const BigInt& left, const BigInt& right) {
  return left.Compare(right) == 0;
}

bool operator!=(const BigInt& left, const BigInt& right) {
  return left.Compare(right) != 0;
}

BigInt& BigInt::operator*=(int x) {
  if (x < 0) {
    is_negative_ = !is_negative_;
    x = -x;
  }

  int64_t carry = 0;
  for (int i = 0; i < digits_.size(); i++) {
    int64_t tmp = (int64_t)digits_[i] * (int64_t)x + carry;
    digits_[i] = tmp % kBase;
    carry = tmp / kBase;
  }
  if (carry) {
    digits_.push_back(carry);
  }

  Normalize();
  return *this;
}

BigInt operator*(const BigInt& big_int, long long x) {
  BigInt res(big_int);
  res *= x;
  return res;
}

BigInt operator*(long long x, const BigInt& big_int) {
  BigInt res(big_int);
  res *= x;
  return res;
}
