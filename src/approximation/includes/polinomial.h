#ifndef POLINOMIAL_H
#define POLINOMIAL_H

#include <vector>

// Uses φ(x) = a0 + a1*x + ... + ak*x^k as approximation function.
// a - coefficients,
// x - close date,
// k - polinomial degree.
//
// Returns approximated close cost
namespace s21 {
double Polinomial(std::vector<double> const &coefficients, double xValue);
}

#endif