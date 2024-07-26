#ifndef SPLINE_H
#define SPLINE_H

#include <iostream>
#include <vector>

#include "trade.h"

class Spline {
 private:
  Trade _trade;
  std::vector<int> _h;
  std::vector<double> _matrix;
  std::vector<double> _res;
  std::vector<double> _a, _b, _c, _d, _m;
  int _size;

  void CreateSteps();
  void CreateMatrix();
  void CreateRes();
  void Tridiagonal();
  void Coefficients();

 public:
  Spline(Trade &trade);
  ~Spline() {}

  const std::vector<int> &GetH() const { return _h; }

  void CubSplain();
  std::pair<std::time_t, double> CubPoint(std::time_t &date);
  std::map<std::time_t, double> SplinePlot(int amount);

 private:
  int FindInterval(std::time_t &currDate, std::vector<std::time_t> &dates,
                   int number);
  double Polynom(std::time_t &currDate, std::time_t &startDate, int number);
};

#endif