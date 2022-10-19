#pragma once
#include <vector>

std::vector<int> Factorize(int n) {
  std::vector<int> factors;

  for (int i = 2; i <= n; i++) {
    if (n % i == 0) {
      factors.push_back(i);
      while (n % i == 0) {
        n /= i;
      }
    }
  }

  return factors;
}