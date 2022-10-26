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

  BigInt& operator*=(int x);
  friend BigInt operator*(const BigInt big_int, const long long x);
  friend BigInt operator*(const long long x, const BigInt big_int);

  size_t Size() const { return digits.size(); }

  // TODO: change implementation after switching to base 2^32
  friend std::ostream& operator<<(std::ostream& output, const BigInt& big_int) {
    if (big_int.digits.empty()) {
      output << "0";
    }
    for (auto digit : big_int.digits) {
      output << digit;
    }
    return output;
  }

 private:
  // const long long unsigned base_ = 4294967296;
  const long long unsigned base_ = 10;
  std::vector<unsigned> digits;
  bool is_negative_ = false;

  unsigned operator[](int i) const { return digits[i]; }
  unsigned& operator[](int i) { return digits[i]; }

  // add absolute values
  void AddAbs(const BigInt& to_add);

  // remove trailling zeros
  void Normalize();
};
