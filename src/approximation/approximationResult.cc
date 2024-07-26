#include "approximationResult.h"

#include <iostream>

#include "polinomial.h"
#include "trade.h"

using namespace std;
using namespace s21;

ApproximationResult::ApproximationResult(vector<GraphEntry> approximatedGraph,
                                         vector<double> polinomialCoefficients,
                                         int32_t gcd)
    : approximatedGraph_(approximatedGraph),
      polinomialCoefficients_(polinomialCoefficients),
      gcd_(gcd) {}

ApproximationResult::~ApproximationResult() {}

GraphEntry ApproximationResult::GetSinglePoint(time_t time) const {
  int64_t timeSinceBeginning = time - approximatedGraph_[0].date + DAYTICKS;

  if (timeSinceBeginning < DAYTICKS) {
    throw std::invalid_argument(
        "Введеная дата меньше крайней левой границы графа!");
  }

  double approximatedCost = Polinomial(
      polinomialCoefficients_, double(timeSinceBeginning) / double(gcd_));

  return GraphEntry{.cost = approximatedCost, .date = time};
}

vector<GraphEntry> ApproximationResult::GetApproximatedGraph() const {
  return approximatedGraph_;
}