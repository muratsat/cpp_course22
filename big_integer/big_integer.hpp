#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static const int kDecimalBase = 10;

class BigInt {
 public:
  static const long long kBase = 1000000000;

  BigInt() = default;
  BigInt(int64_t n);
  BigInt(int n) : BigInt((int64_t)n) {}
  BigInt(long long n) : BigInt((int64_t)n) {}
  BigInt(unsigned n) : BigInt((int64_t)n) {}
  BigInt(double n) : BigInt((int64_t)n) {}
  BigInt(const std::string& s);
  BigInt(const char* c_string) : BigInt(std::string(c_string)){};
  BigInt(const BigInt& other) { *this = other; }
  ~BigInt(){};

  // assignment operator
  BigInt& operator=(const BigInt& other);

  // unary minus
  BigInt& operator-();
  BigInt operator-() const;

  // Plus operator
  BigInt& operator+=(const BigInt& other);
  friend BigInt operator+(const BigInt& left, const BigInt& right);

  // Minus operator
  BigInt& operator-=(const BigInt& other);
  friend BigInt operator-(const BigInt& left, const BigInt& right);

  // Multiply operator
  BigInt& operator*=(const BigInt& factor);
  friend BigInt operator*(const BigInt& left, const BigInt& right);

  // Division operator
  BigInt& operator/=(const BigInt& divisor);
  friend BigInt operator/(const BigInt& left, const BigInt& right);

  // Module operator
  BigInt& operator%=(const BigInt& divisor);
  friend BigInt operator%(const BigInt& left, const BigInt& right);

  friend bool operator<(const BigInt& left, const BigInt& right);
  friend bool operator>(const BigInt& left, const BigInt& right);
  friend bool operator<=(const BigInt& left, const BigInt& right);
  friend bool operator>=(const BigInt& left, const BigInt& right);
  friend bool operator==(const BigInt& left, const BigInt& right);
  friend bool operator!=(const BigInt& left, const BigInt& right);

  // prefix increment
  BigInt& operator++();

  // postfix increment
  BigInt operator++(int);

  // prefix decrement
  BigInt& operator--();

  // postfix decrement
  BigInt operator--(int);

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

  // return string representation
  // in base from 2 to 36 (0 .. 9, a .. z)
  std::string ToString(int base = kDecimalBase) const;

  // Return number of digits in BigInt base
  size_t Size() const { return digits_.size(); }

  bool IsZero() const { return digits_.empty(); }

  bool IsNegative() const { return is_negative_; }

  // absolute value
  BigInt& Abs() {
    is_negative_ = false;
    return *this;
  }
  BigInt Abs() const {
    BigInt abs(*this);
    abs.is_negative_ = false;
    return abs;
  }

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

 private:
  // static const long long unsigned kBase = 10;
  std::vector<unsigned> digits_;
  bool is_negative_ = false;

  unsigned operator[](int i) const { return digits_[i]; }
  unsigned& operator[](int i) { return digits_[i]; }

  // remove trailling zeros
  void Normalize();

  // add absolute values
  // |this| := |this| + |to_add|
  void AddAbs(const BigInt& to_add);

  // subtract absolute values
  // |this| := ||this| -  |to_sub||
  void SubAbs(const BigInt& to_sub);

  // Multiply by a small number
  void Multiply(long long x);

  // Unsigned integer division by a small number
  // returns remainder
  int Divide(long long divisor);

  // Absolute division
  // |this| := |this| / |divisor|
  // if is_remainder is true, then
  // |this| := |this| % |divisor|
  void DivideAbs(const BigInt& divisor, bool is_remainder = false);

  void ShiftLeft() { digits_.insert(digits_.begin(), 0); }
};
