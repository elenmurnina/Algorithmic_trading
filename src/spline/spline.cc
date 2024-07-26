#include "includes/spline.h"

#include <cmath>

Spline::Spline(Trade &trade) : _trade(trade) {
  _size = _trade.GetMap().size();
  _a = _trade.GetValues();
}

void Spline::CreateSteps() {
  std::vector<std::time_t> dates = _trade.GetDates();
  for (int i = 0; i < _size - 1; ++i) {
    _h.push_back((int)(dates[i + 1] - dates[i]));
  }
}

void Spline::CreateMatrix() {
  int i, j, n;
  _matrix.resize(_size * _size);
  for (j = 0; j < _size; ++j) {
    for (i = 0; i < _size; ++i) {
      n = j * (_size) + i;
      if (i + j == 0 || n == _size * _size - 1) {
        _matrix[n] = 1;
      } else if (i == j) {
        _matrix[n] = 2 * (_h[i - 1] + _h[i]);
      } else if (j != 0 && j == i - 1) {
        _matrix[n] = _h[j];
      } else if (j != _size - 1 && j == i + 1) {
        _matrix[n] = _h[i];
      } else {
        _matrix[n] = 0;
      }
    }
  }
}

void Spline::CreateRes() {
  for (int i = 0; i < _size; ++i) {
    if (i == 0 || i == _size - 1) {
      _res.push_back(0);
    } else {
      double num;
      num = (_a[i + 1] - _a[i]) / _h[i] - (_a[i] - _a[i - 1]) / _h[i - 1];
      _res.push_back(6 * num);
    }
  }
}

void Spline::Tridiagonal() {
  int i;
  _m.resize(_size);
  std::vector<double> a(_size, 0), b(_size, 0), c(_size, 0);
  for (i = 0; i < _size - 1; ++i) {
    c[i] = _matrix[i * _size + i + 1];
  }
  for (i = 1; i < _size; ++i) {
    a[i] = _matrix[i * _size + i - 1];
  }
  for (i = 0; i < _size; ++i) {
    b[i] = _matrix[i * _size + i];
  }

  std::vector<double> alfa(_size, 0), beta(_size, 0);
  alfa[0] = -c[0] / b[0];
  beta[0] = _res[0] / b[0];

  for (i = 0; i < _size - 1; ++i) {
    alfa[i] = -c[i] / (b[i] + a[i] * alfa[i - 1]);
    beta[i] = (_res[i] - a[i] * beta[i - 1]) / (b[i] + a[i] * alfa[i - 1]);
  }
  _m[i] = (_res[i] - a[i] * beta[i - 1]) / (b[i] + a[i] * alfa[i - 1]);

  --i;
  for (; i >= 0; --i) {
    _m[i] = alfa[i] * _m[i + 1] + beta[i];
  }
}

void Spline::Coefficients() {
  for (int i = 0; i < _size - 1; ++i) {
    _b.push_back((_a[i + 1] - _a[i]) / _h[i] - _h[i] * _m[i] / 2 -
                 _h[i] * (_m[i + 1] - _m[i]) / 6);
    _c.push_back(_m[i] / 2);
    _d.push_back((_m[i + 1] - _m[i]) / (6 * _h[i]));
  }
}

void Spline::CubSplain() {
  CreateSteps();
  CreateMatrix();
  CreateRes();
  Tridiagonal();
  Coefficients();

  //    std::cout << "\ncoefficients\n";
  //    for (int i = 0; i < _size; ++i) {
  //        std::cout << _a[i] << "   "
  //                                << _b[i] << "   "
  //                                << _c[i] << "   "
  //                                << _d[i] << "   "
  //                                << std::endl;
  //    }
}

std::pair<std::time_t, double> Spline::CubPoint(std::time_t &date) {
  int number = 0;
  std::vector<std::time_t> dates = _trade.GetDates();
  number = FindInterval(date, dates, number);
  return std::make_pair(date, Polynom(date, dates[number], number));
}

int Spline::FindInterval(std::time_t &currDate, std::vector<std::time_t> &dates,
                         int number) {
  while (currDate >= dates[number] && number < _size) {
    ++number;
  }
  --number;
  return number;
}

double Spline::Polynom(std::time_t &currDate, std::time_t &startDate,
                       int number) {
  int x;
  x = (int)(currDate - startDate);
  double value = _a[number] + _b[number] * x + _c[number] * pow(x, 2) +
                 _d[number] * pow(x, 3);
  return value;
}

std::map<std::time_t, double> Spline::SplinePlot(int amount) {
  std::time_t step, rest, currDate;
  std::map<std::time_t, double> plot;

  std::vector<std::time_t> dates = _trade.GetDates();
  step = (dates[_size - 1] - dates[0]) / (amount - 1);
  rest = (dates[_size - 1] - dates[0]) % (amount - 1);

  int number = 0;
  currDate = dates[0];
  while (currDate <= dates[_size - 1]) {
    number = FindInterval(currDate, dates, number);
    plot.emplace(
        std::make_pair(currDate, Polynom(currDate, dates[number], number)));

    currDate += step;
    if (rest > 0) {
      ++currDate;
      --rest;
    }
  }

  return plot;
}
