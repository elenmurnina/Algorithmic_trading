#include "newton.h"

Newton::Newton(Trade &trade) : _trade(trade) {
  _size = _trade.GetMap().size();
  fillDiffs();
}

void Newton::fillDiffs() {
  std::vector<std::time_t> dates = _trade.GetDates();
  std::vector<double> values = _trade.GetValues();

  for (int k = 0; k < _size && k < 4; ++k) {
    std::vector<double> diff;
    for (int i = 0; i < _size - k; ++i) {
      if (k == 0) {
        diff.push_back(values[i]);
      } else {
        double tmp;
        tmp = _divDiff.at(k - 1).at(i + 1) - _divDiff.at(k - 1).at(i);
        tmp = tmp / ((dates[i + k] - dates[i]) / 3600);
        diff.push_back(tmp);
      }
    }
    _divDiff.push_back(diff);
  }
}

int Newton::findTale(std::time_t &date) {
  if (_size <= 4) {
    return _size - 1;
  }

  std::vector<std::time_t> dates = _trade.GetDates();
  int i = 0;
  while (dates[i] <= date && i < _size) {
    ++i;
  }
  --i;

  if (i >= _size - 2) {
    return _size - 1;
  } else if (i == 0) {
    return 3;
  } else {
    return i + 2;
  }
}

s21::GraphEntry Newton::newtPoint(std::time_t &date) {
  std::vector<std::time_t> dates = _trade.GetDates();
  double value;
  int i = findTale(date);
  int head;
  if (_size < 4) {
    head = 0;
  } else {
    head = i - 3;
  }

  value = _divDiff.at(i - head).at(head) * (int)((date - dates[i - 1]) / 3600);
  for (i = i - 1; i - head > 0; --i) {
    value = (_divDiff.at(i - head).at(head) + value) *
            (int)((date - dates[i - 1]) / 3600);
  }
  value += _divDiff.at(0).at(head);

  return s21::GraphEntry{.date = date, .cost = value};
}

std::vector<s21::GraphEntry> Newton::newtonPlot(int amount) {
  time_t step, rest, currDate;
  std::vector<s21::GraphEntry> plot;

  std::vector<std::time_t> dates = _trade.GetDates();
  step = (dates[_size - 1] - dates[0]) / (amount - 1);
  rest = (dates[_size - 1] - dates[0]) % (amount - 1);

  currDate = dates[0];
  while (currDate <= dates[_size - 1]) {
    plot.push_back(newtPoint(currDate));
    currDate += step;
    if (rest > 0) {
      ++currDate;
      --rest;
    }
  }

  return plot;
}