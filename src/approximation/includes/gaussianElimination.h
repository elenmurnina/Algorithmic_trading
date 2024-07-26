#ifndef GAUSSIAN_ELIMINATION_H
#define GAUSSIAN_ELIMINATION_H

#include <vector>
using namespace std;

namespace s21 {
class GaussianElimination {
 public:
  ~GaussianElimination();
  vector<double> GetSolution(vector<vector<double>> augmentedMatrix) const;
};
}  // namespace s21

#endif