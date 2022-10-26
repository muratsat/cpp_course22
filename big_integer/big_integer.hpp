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

  BigInt& operator=(const BigInt& other);

  BigInt& operator*=(int x);
  friend BigInt operator*(const BigInt big_int, const long long x);
  friend BigInt operator*(const long long x, const BigInt big_int);

  size_t Size() const { return digits.size(); }

 private:
  const long long unsigned base_ = 4294967296;
  // const long long unsigned base_ = 10;
  std::vector<unsigned> digits;
  bool is_negative_ = false;

  // add absolute values
  void AddAbs(const BigInt& to_add);

  // remove trailling zeros
  void Normalize();
};
