#ifndef TRADE_H
#define TRADE_H

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "graphEntry.h"

#define DAYTICKS (uint32_t)86400

class Trade {
 private:
  std::vector<std::time_t> _dates;
  std::vector<double> _values;
  std::vector<s21::GraphEntry> _map;
  long _size;

  void CreateVectors();

 public:
  Trade(std::string& filename);
  Trade(std::vector<s21::GraphEntry>& map);
  ~Trade() {}
  static std::time_t ParseDate(const std::string& line) {
    std::stringstream stream(line);
    std::tm date{};
    std::string year, month, day;

    std::getline(stream, year, '-');
    std::getline(stream, month, '-');
    std::getline(stream, day);

    date.tm_year = std::stoi(year) - 1900;
    date.tm_mon = std::stoi(month) - 1;
    date.tm_mday = std::stoi(day);

    return std::mktime(&date);
  }

  const std::vector<std::time_t>& GetDates() const { return _dates; }
  const std::vector<double>& GetValues() const { return _values; }
  const std::vector<s21::GraphEntry>& GetMap() const { return _map; }
  long GetSize() const { return _size; }
};

#endif
