#pragma once

int* LowerBound(int* first, int* last, int value) {
  if (value <= *first) {
    return first;
  }

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