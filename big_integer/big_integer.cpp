#include "big_integer.hpp"

#include <iostream>

static int Max(int a, int b) { return a > b ? a : b; }

BigInt::BigInt(int64_t n) {
  if (n == 0) {
    return;
  }
  if (n < 0) {
    is_negative_ = true;
  }
  digits.push_back(n % base_);
  if (n / base_) {
    digits.push_back(n / base_);
  }
}

BigInt& BigInt::operator=(const BigInt& other) {
  digits = other.digits;
  is_negative_ = other.is_negative_;
  return *this;
}

BigInt::BigInt(std::string s) {
  BigInt dec(1);

  for (int i = s.size() - 1; i >= 0; i--) {
    if (s[i] == '-') {
      is_negative_ = !digits.empty();
      break;
    }
    BigInt to_add = dec * (s[i] - '0');
    AddAbs(to_add);
    dec *= 10;
  }
}

void BigInt::AddAbs(const BigInt& to_add) {
  size_t old_size = digits.size();
  size_t max_size = Max(old_size, to_add.digits.size());
  digits.resize(max_size);

  int64_t carry = 0;
  for (size_t i = 0; i < max_size; i++) {
    int64_t tmp = carry;
    if (i < old_size) {
      tmp += digits[i];
    }
    if (i < to_add.digits.size()) {
      tmp += to_add[i];
    }

    digits[i] = tmp % base_;
    carry = tmp / base_;
  }

  if (carry) {
    digits.push_back(carry);
  }

  Normalize();
}

void BigInt::Normalize() {
  size_t new_size = digits.size();
  while (new_size > 0 && digits[new_size - 1] == 0) {
    new_size--;
  }
  digits.resize(new_size);
  if (digits.empty()) {
    is_negative_ = false;
  }
}

int BigInt::CompareAbs(const BigInt& other) const {
  if (Size() < other.Size()) {
    return -1;
  }
  if (Size() > other.Size()) {
    return 1;
  }

  for (size_t i = 0; i < Size(); i++) {
    if (digits[i] < other[i]) {
      return -1;
    }
    if (digits[i] > other[i]) {
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
  for (int i = 0; i < digits.size(); i++) {
    int64_t tmp = digits[i] * x + carry;
    digits[i] = tmp % base_;
    carry = tmp / base_;
  }
  if (carry) {
    digits.push_back(carry);
  }

  Normalize();
  return *this;
}

BigInt operator*(const BigInt big_int, const long long x) {
  BigInt res(big_int);
  res *= x;
  return res;
}

BigInt operator*(const long long x, const BigInt big_int) {
  BigInt res(big_int);
  res *= x;
  return res;
}
