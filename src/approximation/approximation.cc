#include "approximation.h"

#include <math.h>
#include <time.h>

#include <numeric>
#include <vector>

#include "polinomial.h"
#include "utils.h"

using namespace s21;
using namespace std;

Approximation::Approximation(int polinomialDegree, int approximationDepth,
                             int pointsCount)
    : polinomialDegree_(polinomialDegree),
      approximationDepth_(approximationDepth),
      pointsCount_(pointsCount) {}

Approximation::~Approximation() {}

vector<int64_t> Approximation::GetDateIndicies(
    vector<GraphEntry> const &knownValues, int32_t indexStep) const {
  vector<int64_t> dayIndicies(knownValues.size());
  dayIndicies[0] = indexStep;

  time_t firstDate = knownValues[0].date;
  for (size_t i = 1; i < knownValues.size(); i++) {
    time_t currentDate = knownValues[i].date;

    int64_t secGap = currentDate - firstDate;
    if (secGap <= 0) {
      throw std::invalid_argument("Graph dates are inconsistent");
    }
    int32_t dayGap = secGap / DAYTICKS + 1;
    dayIndicies[i] = dayGap * indexStep;
  }

  return dayIndicies;
}

// Builds a matrix representation of system of linear equations of the form:
// [ a0 * n + a1 * Σxi + ... + ak * Σxi^k = Σyi
// [ a0 * xi + a1 * Σxi^2 + ... + ak * Σxi^k+1 = Σxi * yi
// [ ...
// [ a0 * xi^k + a1 * Σxi^k+1 + ... + ak * Σxi^2k = Σxi^k * yi
//
// a0, ..., ak - Coefficients of the polinomial
// x - close cost (known value)
// y - close time (known value)
// k - Polinomial degree
// i - Index of a known value
// x and y - known values
vector<vector<double>> Approximation::BuildAugmentedMatrix(
    vector<GraphEntry> const &knownValues,
    uint32_t relativeBaseGraphStep) const {
  vector<vector<uint64_t>> poweredXValues(polinomialDegree_ * 2);
  vector<uint64_t> poweredXValuesSumms(polinomialDegree_ * 2);

  vector<int64_t> dateIndicies =
      GetDateIndicies(knownValues, relativeBaseGraphStep);

  for (auto power = 0; power < polinomialDegree_ * 2; power++) {
    vector<uint64_t> allXInPower(knownValues.size());

    for (size_t i = 0; i < knownValues.size(); i++) {
      allXInPower[i] = pow(dateIndicies[i], power);
    }

    poweredXValues[power] = allXInPower;
    poweredXValuesSumms[power] = sum(allXInPower);
  }

  vector<double> yValues(knownValues.size());
  for (size_t i = 0; i < knownValues.size(); i++) {
    yValues[i] = knownValues[i].cost;
  }

  vector<vector<double>> matrixResult(polinomialDegree_);
  for (auto row = 0; row < polinomialDegree_; row++) {
    int augmentedMatrixSize = polinomialDegree_ + 1;
    vector<double> matrixRow(augmentedMatrixSize);

    for (auto col = 0; col < augmentedMatrixSize; col++) {
      if (col == augmentedMatrixSize - 1) {
        matrixRow[col] = sumMultiply(yValues, poweredXValues[row]);
        continue;
      }

      matrixRow[col] = poweredXValuesSumms[col + row];
    }

    matrixResult[row] = matrixRow;
  }

  return matrixResult;
}

vector<GraphEntry> Approximation::GetApproximation(
    vector<GraphEntry> const &knownValues,
    vector<double> const &polinomialCoefficients, uint32_t relativePointStep,
    uint32_t gcd) const {
  uint64_t lastDateIndex = relativePointStep * (pointsCount_ + 1);

  vector<GraphEntry> approximatedVector;
  struct tm *currentDate = localtime(&knownValues[0].date);

  for (uint64_t dateIndex = relativePointStep; dateIndex <= lastDateIndex;
       dateIndex += relativePointStep) {
    double approximatedCost = Polinomial(polinomialCoefficients, dateIndex);

    time_t date = mktime(currentDate);
    approximatedVector.push_back(
        GraphEntry{.date = date, .cost = approximatedCost});

    int32_t recoveredTimespan = relativePointStep * gcd;
    currentDate->tm_sec += recoveredTimespan;
  }

  return approximatedVector;
}

ApproximationResult Approximation::Approximate(Trade &trade) const {
  if (trade.GetSize() <= 1) {
    throw invalid_argument("Graph must be of size of at least > 1");
  }

  vector<GraphEntry> knownValues = trade.GetMap();

  int32_t knownValuesDayCount =
      (knownValues[knownValues.size() - 1].date - knownValues[0].date) /
      DAYTICKS;
  float stepRatio =
      float(knownValuesDayCount + approximationDepth_) / float(pointsCount_);
  uint32_t pointStep = DAYTICKS * stepRatio;

  // GCD - Greatest Common Devider
  int32_t gcd = __gcd(DAYTICKS, pointStep);

  int32_t relativePointStep = pointStep / gcd;
  int32_t relativeBaseGraphStep = DAYTICKS / gcd;

  vector<vector<double>> augmentedMatrix =
      BuildAugmentedMatrix(knownValues, relativeBaseGraphStep);

  vector<double> coeffiecients =
      gaussianElimination_.GetSolution(augmentedMatrix);

  vector<GraphEntry> approximatedGraph =
      GetApproximation(knownValues, coeffiecients, relativePointStep, gcd);

  return ApproximationResult(approximatedGraph, coeffiecients, gcd);
}
