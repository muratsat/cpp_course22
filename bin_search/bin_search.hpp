#pragma once

int* LowerBound(int* first, int* last, int value) {
  while (last > first + 1) {
    int* mid = first + (last - first) / 2;

    if (*mid < value) {
      first = mid;
    } else {
      last = mid;
    }
  }

  return last;
}