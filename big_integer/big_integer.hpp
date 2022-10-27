#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class BigInt {
 public:
  BigInt() = default;
  BigInt(int64_t n);
  BigInt(std::string s);
  BigInt(const BigInt& other) { *this = other; }
  ~BigInt(){};

  friend bool operator<(const BigInt& left, const BigInt& right);
  friend bool operator>(const BigInt& left, const BigInt& right);
  friend bool operator<=(const BigInt& left, const BigInt& right);
  friend bool operator>=(const BigInt& left, const BigInt& right);
  friend bool operator==(const BigInt& left, const BigInt& right);
  friend bool operator!=(const BigInt& left, const BigInt& right);

  BigInt& operator=(const BigInt& other);

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
  BigInt& operator*=(int x);
  friend BigInt operator*(const BigInt& big_int, const long long x);
  friend BigInt operator*(const long long x, const BigInt& big_int);

  // Return number of digits in BigInt base
  size_t Size() const { return digits.size(); }

  // TODO: change implementation after switching to base 2^32
  friend std::ostream& operator<<(std::ostream& output, const BigInt& big_int) {
    if (big_int.digits.empty()) {
      output << '0';
    }
    if (big_int.is_negative_) {
      output << '-';
    }
    for (int i = big_int.Size() - 1; i >= 0; i--) {
      output << big_int[i];
    }
    return output;
  }

 private:
  const long long unsigned base_ = 4294967296;
  // const long long unsigned base_ = 10;
  std::vector<unsigned> digits;
  bool is_negative_ = false;

  unsigned operator[](int i) const { return digits[i]; }
  unsigned& operator[](int i) { return digits[i]; }

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

  // remove trailling zeros
  void Normalize();
};
