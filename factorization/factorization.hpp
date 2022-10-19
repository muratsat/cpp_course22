#pragma once
#include <vector>

std::vector<int> Factorize(int n) {
  std::vector<int> factors;

  for (int i = 2; i <= n; i++) {
    if (n % i == 0) {
      while (n % i == 0) {
        factors.push_back(i);
        n /= i;
      }
    }
  }

  return factors;
}