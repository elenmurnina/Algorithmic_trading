#ifndef NEWTON_H
#define NEWTON_H

#include <vector>

#include "graphEntry.h"
#include "trade.h"

class Newton {
 private:
  Trade _trade;
  int _size;
  std::vector<std::vector<double>> _divDiff;

  void fillDiffs();
  int findTale(std::time_t &date);

 public:
  Newton(Trade &trade);

  s21::GraphEntry newtPoint(std::time_t &date);
  std::vector<s21::GraphEntry> newtonPlot(int amount);
};

#endif
