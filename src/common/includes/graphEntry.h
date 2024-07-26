#ifndef GRPAPH_ENTRY_H
#define GRPAPH_ENTRY_H

#include <ctime>

using namespace std;

namespace s21 {
typedef struct GraphEntry {
  time_t date;
  double cost;
} GraphEntry;
}  // namespace s21

#endif
