#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static const int kDecimalBase = 10;

class BigInt {
 public:
  BigInt() = default;
  BigInt(int64_t n);
  BigInt(int n) : BigInt((int64_t)n){};
  BigInt(unsigned n) : BigInt((int64_t)n){};
  BigInt(double d) : BigInt((int64_t)d){};
  explicit BigInt(const std::string& s, int string_base = kDecimalBase);
  BigInt(const char* c_string) : BigInt(std::string(c_string)){};
  BigInt(const BigInt& other) { *this = other; }
  ~BigInt(){};

  friend bool operator<(const BigInt& left, const BigInt& right);
  friend bool operator>(const BigInt& left, const BigInt& right);
  friend bool operator<=(const BigInt& left, const BigInt& right);
  friend bool operator>=(const BigInt& left, const BigInt& right);
  friend bool operator==(const BigInt& left, const BigInt& right);
  friend bool operator!=(const BigInt& left, const BigInt& right);

  BigInt& operator=(const BigInt& other);

  BigInt& operator-() {
    is_negative_ = !is_negative_ && !IsZero();
    return *this;
  }
  BigInt operator-() const {
    BigInt copy = *this;
    return -copy;
  }

  // prefix increment
  BigInt& operator++() {
    *this += 1;
    return *this;
  }

  // postfix increment
  BigInt operator++(int) {
    BigInt tmp = *this;
    ++*this;
    return tmp;
  }

  // prefix decrement
  BigInt& operator--() {
    *this -= 1;
    return *this;
  }

  // postfix decrement
  BigInt operator--(int) {
    BigInt tmp = *this;
    --*this;
    return tmp;
  }

  // Plus operator
  BigInt& operator+=(const BigInt& other);
  friend BigInt operator+(const BigInt& left, const BigInt& right) {
    BigInt result = left;
    result += right;
    return result;
  }

  // Minus operator
  BigInt& operator-=(const BigInt& other);
  friend BigInt operator-(const BigInt& left, const BigInt& right) {
    BigInt result = left;
    result -= right;
    return result;
  }

  // Multiply operator
  // multiply by small factor
  BigInt& operator*=(const BigInt& factor);
  friend BigInt operator*(const BigInt& left, const BigInt& right);

  // Division operator
  BigInt& operator/=(const BigInt& divisor);
  friend BigInt operator/(const BigInt& left, const BigInt& right);

  // Return number of digits in BigInt base
  size_t Size() const { return digits_.size(); }

  bool IsZero() const { return digits_.empty(); }

  // return string representation
  // in base from 2 to 36 (0 .. 9, a .. z)
  std::string ToString(int base = kDecimalBase) const;

  BigInt& Abs() {
    is_negative_ = false;
    return *this;
  }

  BigInt Abs() const {
    BigInt abs = *this;
    abs.is_negative_ = false;
    return abs;
  }

  friend std::istream& operator>>(std::istream& input, BigInt& big_int) {
    std::string str;
    input >> str;
    big_int = BigInt(str);
    return input;
  }

  friend std::ostream& operator<<(std::ostream& output, const BigInt& big_int) {
    output << big_int.ToString();
    return output;
  }

 private:
  const long long unsigned kBase = 4294967296;
  // const long long unsigned kBase = 10;
  std::vector<unsigned> digits_;
  bool is_negative_ = false;

  unsigned operator[](int i) const { return digits_[i]; }
  unsigned& operator[](int i) { return digits_[i]; }

  // Compare absolute values
  //  returns:
  //  0 if |this| = |other|
  // -1 if |this| < |other|
  //  1 if |this| > |other|
  int CompareAbs(const BigInt& other) const;

  // Compare real values
  //  returns:
  //  0 if this = other
  // -1 if this < other
  //  1 if this > other
  int Compare(const BigInt& other) const;

  // add absolute values
  // |this| := |this| + |to_add|
  void AddAbs(const BigInt& to_add);

  // subtract absolute values
  // |this| := ||this| -  |to_sub||
  void SubAbs(const BigInt& to_sub);

  void Multiply(long long x);

  // Unsigned integer division by a small number
  // returns remainder
  int Divide(long long divisor);

  void DivideAbsolute(const BigInt& divisor, bool is_remainder = false);

  void LongDivision(const BigInt& divisor, bool is_remainder = false);

  void ShiftLeft();

  // when dividend and divisor have same size
  // or their sizes differ by 1
  // the result is a small number
  // that can be found using binary search in O(log(BASE))
  friend unsigned DivideSameSize(const BigInt& dividend, const BigInt& divisor);

  // remove trailling zeros
  void Normalize();
};
