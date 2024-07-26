#ifndef APPROXIMATION_RESULT
#define APPROXIMATION_RESULT

#include <map>
#include <vector>

#include "graphEntry.h"

using namespace std;
using namespace s21;

namespace s21 {
class ApproximationResult {
 private:
  vector<GraphEntry> approximatedGraph_;
  vector<double> polinomialCoefficients_;
  uint32_t gcd_;

 public:
  ApproximationResult(vector<GraphEntry> approximatiedGraph,
                      vector<double> polinomialCoefficients, int32_t gcd);
  ~ApproximationResult();

  GraphEntry GetSinglePoint(time_t time) const;
  vector<GraphEntry> GetApproximatedGraph() const;
};
}  // namespace s21

#endif
