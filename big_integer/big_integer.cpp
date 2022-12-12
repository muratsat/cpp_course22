#include "big_integer.hpp"

#include <iostream>
#include <numeric>
#include <type_traits>

static int Max(int a, int b) { return a > b ? a : b; }

BigInt::BigInt(int64_t n) {
  if (n == 0) {
    return;
  }
  const int64_t kTwoTo63 = 0x8000000000000000;
  if (n == kTwoTo63) {
    is_negative_ = true;
    digits_.push_back(1);
    const size_t kTotalBits = 63;
    for (size_t i = 0; i < kTotalBits; i++) {
      Multiply(2);
    }
    return;
  }
  if (n < 0) {
    is_negative_ = true;
    n = -n;
  }
  while (n != 0) {
    digits_.push_back(n % kBase);
    n /= kBase;
  }
}

BigInt& BigInt::operator=(const BigInt& other) {
  digits_ = other.digits_;
  is_negative_ = other.is_negative_;
  return *this;
}

static int DigitValue(char digit) {
  const int kValueOfA = 10;
  if ('0' <= digit && digit <= '9') {
    return digit - '0';
  }
  if ('a' <= digit && digit <= 'z') {
    return digit - 'a' + kValueOfA;
  }
  return digit - 'A' + kValueOfA;
}

BigInt::BigInt(const std::string& s) {
  BigInt dec = 1;
  const int kDecimalBase = 10;

  for (int i = s.size() - 1; i >= 0; i--) {
    if (s[i] == '-') {
      is_negative_ = !digits_.empty();
      break;
    }
    BigInt to_add = dec;
    to_add.Multiply(DigitValue(s[i]));
    AddAbs(to_add);
    dec.Multiply(kDecimalBase);
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

void BigInt::Multiply(long long x) {
  if (x < 0) {
    is_negative_ = !is_negative_;
    x = -x;
  }

  int64_t carry = 0;
  for (size_t i = 0; i < digits_.size(); i++) {
    int64_t tmp = (int64_t)digits_[i] * (int64_t)x + carry;
    digits_[i] = tmp % kBase;
    carry = tmp / kBase;
  }
  while (carry != 0) {
    digits_.push_back(carry % kBase);
    carry /= kBase;
  }

  Normalize();
}

int BigInt::Divide(long long divisor) {
  if (divisor == 0) {
    throw std::invalid_argument("Division by zero");
  }

  int remainder = 0;
  for (int i = digits_.size() - 1; i >= 0; i--) {
    int64_t tmp = kBase * (int64_t)remainder + (int64_t)digits_[i];
    digits_[i] = tmp / divisor;
    remainder = tmp % divisor;
  }

  Normalize();
  if (remainder < 0) {
    remainder += divisor;
  }
  return remainder;
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

  while (carry != 0) {
    digits_.push_back(carry % kBase);
    carry /= kBase;
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

int BigInt::CompareAbs(const BigInt& other) const {
  if (Size() < other.Size()) {
    return -1;
  }
  if (Size() > other.Size()) {
    return 1;
  }

  for (int i = Size() - 1; i >= 0; i--) {
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

static void Reverse(std::string& s) {
  size_t size = s.size();
  for (size_t i = 0; i < size / 2; i++) {
    char t = s[size - 1 - i];
    s[size - 1 - i] = s[i];
    s[i] = t;
  }
}

std::string BigInt::ToString(int base) const {
  const int kMaxBase = 36;
  if (base < 2 || base >= kMaxBase) {
    throw std::invalid_argument("Invalid base");
  }
  char char_value[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                       '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                       'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                       'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  std::string s;
  BigInt copy = *this;

  int remainder = 0;
  do {
    remainder = copy.Divide(base);
    s.push_back(char_value[remainder]);
  } while (!copy.IsZero());

  if (is_negative_) {
    s.push_back('-');
  }

  Reverse(s);

  return s;
}

BigInt& BigInt::operator-() {
  is_negative_ = !is_negative_ && !digits_.empty();
  return *this;
}

BigInt BigInt::operator-() const {
  BigInt result = *this;
  result.is_negative_ = !is_negative_ && !digits_.empty();
  return result;
}

BigInt& BigInt::operator+=(const BigInt& other) {
  if (is_negative_ == other.is_negative_) {
    AddAbs(other);
  } else {
    if (CompareAbs(other) < 0) {
      is_negative_ = !is_negative_;
    }
    SubAbs(other);
  }
  return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
  if (is_negative_ != other.is_negative_) {
    AddAbs(other);
  } else {
    if (CompareAbs(other) < 0) {
      is_negative_ = !is_negative_;
    }
    SubAbs(other);
  }
  return *this;
}

BigInt operator+(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result += right;
  return result;
}

BigInt operator-(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result -= right;
  return result;
}

BigInt& BigInt::operator++() {
  *this += 1;
  return *this;
}

BigInt& BigInt::operator--() {
  *this -= 1;
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt copy = *this;
  ++(*this);
  return copy;
}

BigInt BigInt::operator--(int) {
  BigInt copy = *this;
  --(*this);
  return copy;
}

BigInt operator*(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result *= right;
  return result;
}

BigInt operator/(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result /= right;
  return result;
}

BigInt operator%(const BigInt& left, const BigInt& right) {
  BigInt result = left;
  result %= right;
  return result;
}

BigInt& BigInt::operator*=(const BigInt& factor) {
  size_t old_size = Size();
  std::vector<unsigned> old_digits = digits_;

  digits_.clear();
  digits_.resize(old_size + factor.Size());

  is_negative_ = is_negative_ ^ factor.is_negative_;

  for (size_t i = 0; i < factor.Size(); i++) {
    long long carry = 0;
    for (size_t j = 0; j < old_size; j++) {
      long long tmp = carry;
      tmp += (long long)digits_[i + j];
      tmp += (long long)old_digits[j] * (long long)factor[i];
      digits_[i + j] = tmp % kBase;
      carry = tmp / kBase;
    }
    if (carry != 0) {
      digits_[i + old_size] = carry;
    }
  }

  Normalize();
  return *this;
}

static unsigned DivideBinSearch(const BigInt& dividend, const BigInt& divisor) {
  long long left = 0;
  long long right = BigInt::kBase;

  while (left + 1 < right) {
    long long mid = left + (right - left) / 2;
    BigInt tmp = divisor * mid;
    if (tmp.CompareAbs(dividend) <= 0) {
      left = mid;
    } else {
      right = mid;
    }
  }

  return left;
}

void BigInt::DivideAbs(const BigInt& divisor, bool is_remainder) {
  if (divisor.IsZero()) {
    throw std::invalid_argument("Division by zero");
  }
  if (CompareAbs(divisor) < 0) {
    if (!is_remainder) {
      digits_.clear();
    }
    return;
  }

  BigInt remainder;
  BigInt quotient;
  quotient.digits_.resize(Size());
  for (int i = Size() - 1; i >= 0; i--) {
    remainder.ShiftLeft();
    remainder.digits_[0] = digits_[i];
    remainder.Normalize();
    unsigned digit = DivideBinSearch(remainder, divisor);
    quotient[i] = digit;
    remainder -= divisor * digit;
  }

  quotient.Normalize();
  remainder.Normalize();
  if (is_remainder) {
    *this = remainder;
  } else {
    *this = quotient;
  }
}

BigInt& BigInt::operator/=(const BigInt& divisor) {
  if (divisor.IsZero()) {
    throw std::invalid_argument("Division by zero");
  }

  bool is_result_negative = is_negative_ ^ divisor.is_negative_;
  BigInt div = divisor.Abs();
  DivideAbs(div);
  is_negative_ = is_result_negative && !IsZero();

  return *this;
}

BigInt& BigInt::operator%=(const BigInt& divisor) {
  BigInt quotient = *this / divisor;
  *this -= quotient * divisor;
  return *this;
}