#ifndef APPROXIMATION_H
#define APPROXIMATION_H

#include "approximation.h"
#include "approximationResult.h"
#include "gaussianElimination.h"
#include "graphEntry.h"
#include "map"
#include "trade.h"
#include "vector"

using namespace std;

namespace s21 {
class Approximation {
 private:
  int polinomialDegree_;
  int approximationDepth_;
  int pointsCount_;

  GaussianElimination gaussianElimination_;

  vector<int64_t> GetDateIndicies(vector<GraphEntry> const &knownValues,
                                  int32_t indexStep) const;
  vector<vector<double>> BuildAugmentedMatrix(
      vector<GraphEntry> const &knownValues,
      uint32_t relativeBaseGraphStep) const;
  vector<GraphEntry> GetApproximation(
      vector<GraphEntry> const &knownValues,
      vector<double> const &polinomialCoefficients, uint32_t relativePointStep,
      uint32_t gcd) const;

 public:
  Approximation(int polinomialDegree, int approximationDepth, int pointsCount);
  ~Approximation();

  ApproximationResult Approximate(Trade &trade) const;
};
}  // namespace s21

#endif
