#include <QApplication>
#include <QChartView>
#include <QComboBox>
#include <QDateTimeAxis>
#include <QDateTimeEdit>
#include <QInputDialog>
#include <QLabel>
#include <QLegendMarker>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QValueAxis>
#include <ctime>
#include <sstream>

#include "trade.h"

using namespace s21;

enum InterpolationMethod { CUBIC_SPLINE, NEWTON_POLYNOMIAL };

// Проверка корректности даты
bool IsValidDate(const std::string &dateStr) {
  if (dateStr.size() != 10 || dateStr[4] != '-' || dateStr[7] != '-') {
    return false;
  }

  int year, month, day;
  if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
    return false;
  }

  if (year < 1900 || year > 2023 || month < 1 || month > 12 || day < 1 ||
      day > 31) {
    return false;
  }

  // Проверка на корректное количество дней в месяце
  bool validDay =
      (day >= 1 && day <= 31) && (month >= 1 && month <= 12) &&
      (
          // Месяцы с 31 днем
          (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 ||
           month == 10 || month == 12)
              ? (day <= 31)
              :

              // Месяцы с 30 днями
              (month == 4 || month == 6 || month == 9 || month == 11)
              ? (day <= 30)
              :

              // Февраль
              (day <= ((year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
                           ? 29
                           : 28)));

  return validDay;
}

// Функция проверки файла на валидность
bool IsCSVFileValid(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Failed to open the file: " << filePath << std::endl;
    return false;
  }

  std::string line;
  std::getline(file, line);

  // Проверяем, что заголовок соответствует ожидаемому формату
  if (line != "Date,Close") {
    std::cerr << "Invalid CSV file format: " << filePath << std::endl;
    file.close();
    return false;
  }

  while (std::getline(file, line) && !line.empty()) {
    std::istringstream iss(line);
    std::string date, value;

    if (!(std::getline(iss, date, ',') && std::getline(iss, value, ','))) {
      std::cerr << "Invalid CSV line format: " << line << std::endl;
      file.close();
      return false;
    }
    // Проверяем, что дата имеет правильный формат ("2021-03-01")
    if (!IsValidDate(date)) {
      std::cerr << "Invalid date format in CSV file: " << line << std::endl;
      file.close();
      return false;
    }

    // Проверяем, что вся строка value была успешно преобразована в число
    std::istringstream iss2(value);
    double numericValue;
    char remainingChar;

    if (!(iss2 >> numericValue) || iss2 >> remainingChar) {
      std::cerr << "Invalid double format in CSV file: " << value << std::endl;
      file.close();
      return false;
    }
    line.clear();
  }

  file.close();
  return true;
}

// Функция для очистки списка QLineSeries
void ClearSeriesList(QChart *&chart, QVector<QLineSeries *> &seriesList) {
  for (QLineSeries *series : seriesList) {
    if (chart->series().contains(series)) {
      chart->removeSeries(series);
      delete series;
    }
  }
  seriesList.clear();
}

void ClearData(Trade *&data) {
  if (data) {
    delete data;
    data = nullptr;
  }
}

// Функция для очистки поля для отрисовки графика
void ClearField(QChart *&chart, QChartView *&chartView,
                QVBoxLayout *centralLayout,
                QVector<QLineSeries *> &seriesList) {
  ClearSeriesList(chart, seriesList);

  QChart *newChart = new QChart();
  delete chart;
  chart = newChart;
  QChartView *newChartView = new QChartView(newChart);
  delete chartView;
  chartView = newChartView;
  newChartView->setRenderHint(QPainter::Antialiasing);

  centralLayout->removeWidget(chartView);
  centralLayout->addWidget(newChartView);
}

// Функция для очистки и обновления осей
void ClearAndUpdateXYAxis(QDateTimeAxis *&xAxis, QValueAxis *&yAxis,
                          bool isUpdate) {
  if (xAxis && yAxis) {
    delete xAxis;
    xAxis = nullptr;
    delete yAxis;
    yAxis = nullptr;
  }
  if (isUpdate) {
    xAxis = new QDateTimeAxis;
    yAxis = new QValueAxis;
    xAxis->setFormat("yyyy-MM-dd");
  }
}

// Функция для обработки даты в поле dateTimeEdit
void ProcessDate(QDateTimeEdit *dateTimeEdit, Trade *data,
                 int forecastDayCount) {
  dateTimeEdit->clearMinimumDate();
  dateTimeEdit->clearMaximumDate();
  dateTimeEdit->clear();

  vector<time_t> dates = data->GetDates();
  QDateTime minDateTime =
      QDateTime::fromSecsSinceEpoch(*min_element(dates.begin(), dates.end()));
  QDateTime maxDateTime =
      QDateTime::fromSecsSinceEpoch(*max_element(dates.begin(), dates.end()));
  maxDateTime = maxDateTime.addDays(forecastDayCount);

  // Устанавливаем ограничения для виджета dateTimeEdit
  dateTimeEdit->setMinimumDateTime(minDateTime);
  dateTimeEdit->setMaximumDateTime(maxDateTime);
}
