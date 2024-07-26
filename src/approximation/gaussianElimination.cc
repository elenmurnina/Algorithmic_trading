#include "gaussianElimination.h"

#include <math.h>

#include <iomanip>
#include <iostream>

using namespace std;
using namespace s21;

GaussianElimination::~GaussianElimination() {}

vector<double> GaussianElimination::GetSolution(
    vector<vector<double>> augmentedMatrix) const {
  vector<vector<double>> m(augmentedMatrix.size());
  vector<double> b(augmentedMatrix.size());

  for (size_t i = 0; i < augmentedMatrix.size(); i++) {
    for (size_t j = 0; j < augmentedMatrix.size(); j++) {
      m[i].push_back(augmentedMatrix[i][j]);
    }
  }

  for (size_t i = 0; i < augmentedMatrix.size(); i++) {
    b[i] = augmentedMatrix[i][augmentedMatrix.size()];
  }

  // Forward Elimination
  for (size_t k = 0; k < m.size() - 1; k++) {
    for (size_t i = k + 1; i < m.size(); i++) {
      double fctr = m[i][k] / m[k][k];

      for (size_t j = k; j < m.size(); j++) {
        m[i][j] = m[i][j] - m[k][j] * fctr;
      }

      b[i] = b[i] - b[k] * fctr;
    }
  }

  vector<double> result(m.size());
  // Back-substitution
  result[m.size() - 1] = b[m.size() - 1] / m[m.size() - 1][m.size() - 1];
  for (size_t i = m.size() - 2;; i--) {
    double summ = b[i];
    for (size_t j = i + 1; j < m.size(); j++) {
      summ = summ - m[i][j] * result[j];
    }

    result[i] = summ / m[i][i];
    if (i == 0) {
      break;
    }
  }

  return result;
}