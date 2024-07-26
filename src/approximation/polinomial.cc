#include "polinomial.h"

using namespace std;

double s21::Polinomial(vector<double> const &coefficients, double xValue) {
  double result = 0.0;

  for (size_t i = 0; i < coefficients.size(); i++) {
    result += coefficients[i] * pow(xValue, i);
  }

  return result;
}
