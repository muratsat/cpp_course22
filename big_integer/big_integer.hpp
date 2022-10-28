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
  BigInt& operator*=(int factor);
  friend BigInt operator*(const BigInt& big_int, long long factor);
  friend BigInt operator*(long long factor, const BigInt& big_int);

  // Division operator
  // Divide by a small divisor
  BigInt& operator/=(int divisor);

  // Return number of digits in BigInt base
  size_t Size() const { return digits_.size(); }

  bool IsZero() const { return digits_.empty(); }

  // return string representation
  // in base from 2 to 36 (0 .. 9, a .. z)
  std::string ToString(int base = kDecimalBase) const;

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
  // const long long unsigned base_ = 10;
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

  // Unsigned integer division by a small number
  // returns remainder
  int Divide(int divisor);

  // remove trailling zeros
  void Normalize();
};
