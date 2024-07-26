#include "trade.h"

Trade::Trade(std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  char delimiter = ',';

  std::string date;
  std::string valueStr;

  std::getline(file, line);  // первая строка содержит заголовки, они не нужны
  while (std::getline(file, line)) {
    if (std::all_of(line.begin(), line.end(), ::isspace)) {
      continue;
    }
    std::stringstream stream(line);
    std::getline(stream, date, delimiter);
    std::getline(stream, valueStr);
    _map.push_back(
        s21::GraphEntry{.date = ParseDate(date), .cost = std::stod(valueStr)});
  }
  file.close();
  _size = _map.size();
  CreateVectors();
}

Trade::Trade(std::vector<s21::GraphEntry> &map) {
  _map = map;
  _size = _map.size();
  CreateVectors();
}

void Trade::CreateVectors() {
  for (auto &item : _map) {
    _dates.push_back(item.date);
    _values.push_back(item.cost);
  }
}
