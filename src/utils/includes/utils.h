#ifndef UTILS_H
#define UTILS_H

#include <vector>

using namespace std;

namespace s21 {
template <typename T>
double sum(vector<T> vector) {
  double sum = 0.0;

  for (double i : vector) {
    sum += i;
  }

  return sum;
}

template <typename T, typename K>
double sumMultiply(vector<T> left, vector<K> right) {
  if (left.size() != right.size()) {
    throw new invalid_argument(
        "left vector size is not equal to right vector size");
  }

  double sum = 0.0;

  for (size_t i = 0; i < left.size(); i++) {
    sum += left[i] * right[i];
  }

  return sum;
}
}  // namespace s21

#endif
