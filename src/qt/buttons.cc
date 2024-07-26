#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QScatterSeries>
#include <QSpinBox>
#include <algorithm>

#include "approximation.h"
#include "newton.h"
#include "qt_utils.cc"
#include "spline.h"

using namespace s21;

// Функция для загрузки CSV файла
Trade *LoadCSVFile(QChart *chart, QVector<QLineSeries *> &seriesList,
                   QDateTimeEdit *dateTimeEdit, QDateTimeAxis *&xAxis,
                   QValueAxis *&yAxis, QMainWindow *window,
                   std::string &filename) {
  QString file = QFileDialog::getOpenFileName(nullptr, "Выберите CSV файл", "",
                                              "CSV Files (*.csv)");

  if (!file.isEmpty()) {
    if (IsCSVFileValid(file.toStdString())) {
      ClearSeriesList(chart, seriesList);
      ClearAndUpdateXYAxis(xAxis, yAxis, true);

      filename = file.toStdString();
      Trade *data = new Trade(filename);

      ProcessDate(dateTimeEdit, data, 0);
      chart->setTitle(QString("График для %1").arg(filename.c_str()));
      return data;
    } else {
      QMessageBox::warning(window, "Ошибка", "Неверный формат файла");
    }
  } else {
    QMessageBox::warning(window, "Ошибка", "Файл не выбран");
  }
  return nullptr;
}

// Функция для построения графика интерполяции (кубический сплайн / полином
// Ньютона)
void PlotInterpolation(Trade *data, QChart *chart,
                       QVector<QLineSeries *> &seriesList, QMainWindow *window,
                       QDateTimeAxis *xAxis, QValueAxis *yAxis,
                       InterpolationMethod &interpolationMethod) {
  if (data && data->GetSize() > 0) {
    if (seriesList.size() < 5) {
      int pointCountCsv = data->GetSize();

      // Создаем список вариантов выбора графика
      QStringList items;
      items << "Интерполяция кубическим сплайном"
            << "Интерполяция полиномом Ньютона";

      bool ok;
      QString item =
          QInputDialog::getItem(window, "Выберите метод интерполяции",
                                "Метод:", items, 0, false, &ok);

      int pointCount = QInputDialog::getInt(window, "Введите количество точек",
                                            "Количество точек:", pointCountCsv,
                                            pointCountCsv, INT_MAX, 1, &ok);

      if (ok) {
        if (pointCount >= pointCountCsv) {
          QLineSeries *series = new QLineSeries();
          seriesList.push_back(series);

          QString chartName;

          std::time_t minDate = std::numeric_limits<std::time_t>::max();
          std::time_t maxDate = std::numeric_limits<std::time_t>::min();
          double minCost = INT_MAX;
          double maxCost = INT_MIN;

          if (item == "Интерполяция кубическим сплайном") {
            interpolationMethod = CUBIC_SPLINE;
            Spline spline(*data);
            spline.CubSplain();
            std::map<std::time_t, double> splinesData =
                spline.SplinePlot(pointCount);

            for (auto splinePoint : splinesData) {
              minDate = std::min(minDate, splinePoint.first);
              maxDate = std::max(maxDate, splinePoint.first);
              minCost = std::min(minCost, splinePoint.second);
              maxCost = std::max(maxCost, splinePoint.second);
              QPointF dataPoint(QDateTime::fromSecsSinceEpoch(splinePoint.first)
                                    .toMSecsSinceEpoch(),
                                splinePoint.second);
              seriesList.back()->append(dataPoint);
            }
            chartName = QString("Сплайн. Точек: %1").arg(pointCount);
          } else if (item == "Интерполяция полиномом Ньютона") {
            interpolationMethod = NEWTON_POLYNOMIAL;
            Newton newton(*data);
            std::vector<s21::GraphEntry> newtonData =
                newton.newtonPlot(pointCount);

            for (auto newtonPoint : newtonData) {
              minDate = std::min(minDate, newtonPoint.date);
              maxDate = std::max(maxDate, newtonPoint.date);
              minCost = std::min(minCost, newtonPoint.cost);
              maxCost = std::max(maxCost, newtonPoint.cost);
              QPointF dataPoint(QDateTime::fromSecsSinceEpoch(newtonPoint.date)
                                    .toMSecsSinceEpoch(),
                                newtonPoint.cost);
              seriesList.back()->append(dataPoint);
            }
            chartName = QString("Полином Ньютона. Точек: %1").arg(pointCount);
          }
          seriesList.back()->setName(chartName);
          chart->addSeries(seriesList.back());

          if (!chart->axes(Qt::Horizontal).contains(xAxis)) {
            // Увеличиваем диапазон оси Y на 10% с каждой стороны
            double increaseFactor = 1.1;
            double newYMin =
                minCost - (maxCost - minCost) * (increaseFactor - 1) / 2;
            double newYMax =
                maxCost + (maxCost - minCost) * (increaseFactor - 1) / 2;

            // Устанавливаем новые значения диапазона осей X и Y
            xAxis->setRange(QDateTime::fromSecsSinceEpoch(minDate),
                            QDateTime::fromSecsSinceEpoch(maxDate));
            yAxis->setRange(newYMin, newYMax);
            chart->addAxis(xAxis, Qt::AlignBottom);
            chart->addAxis(yAxis, Qt::AlignLeft);
          }
          seriesList.back()->attachAxis(xAxis);
          seriesList.back()->attachAxis(yAxis);
        } else {
          QMessageBox::warning(window, "Ошибка",
                               "Количество точек должно быть не меньше, чем в "
                               "загруженном csv файле!");
        }
      }
    } else {
      QMessageBox::warning(
          window, "Ошибка",
          "На поле может быть одновременно изображено не более 5 графиков");
    }
  } else {
    QMessageBox::warning(window, "Ошибка", "Сначала загрузите файл с данными");
  }
}

// Функция для возврата значения акций на выбранную дату и время (для
// интерполяции)
void ValueByDateInterpolation(Trade *data, QDateTimeEdit *dateTimeEdit,
                              QLabel *resultValueLabel, QMainWindow *window,
                              InterpolationMethod &interpolationMethod) {
  if (data && data->GetSize() > 0) {
    // Получаем выбранную пользователем дату и время из dateTimeEdit
    QDateTime selectedDateTime = dateTimeEdit->dateTime();
    std::time_t timeT = selectedDateTime.toSecsSinceEpoch();

    double value = 0.0;
    QString formattedDateTime =
        selectedDateTime.toString("yyyy-MM-dd HH:mm:ss");

    if (interpolationMethod == CUBIC_SPLINE) {
      Spline spline(*data);
      spline.CubSplain();
      std::pair<std::time_t, double> point = spline.CubPoint(timeT);
      value = point.second;
    } else if (interpolationMethod == NEWTON_POLYNOMIAL) {
      Newton newton(*data);
      s21::GraphEntry newtonPoint = newton.newtPoint(timeT);
      value = newtonPoint.cost;
    }

    resultValueLabel->setText(
        QString("Стоимость акций на %1: %2").arg(formattedDateTime).arg(value));
  } else {
    QMessageBox::warning(window, "Ошибка", "Сначала загрузите файл с данными");
  }
}

// Функция для построения графика аппроксимации
void PlotApproximation(Trade *data, QChart *chart,
                       QVector<QLineSeries *> &seriesList,
                       QScatterSeries *&pointSeries, QMainWindow *window,
                       QDateTimeAxis *xAxis, QValueAxis *yAxis,
                       QSpinBox *polynomialDegreeSpinBox,
                       QSpinBox *forecastDayCountSpinBox, int &pointPlotCount) {
  if (data && data->GetSize() > 0) {
    if (seriesList.size() < 5) {
      int pointCountCsv = data->GetSize();
      bool ok;
      int pointCount = QInputDialog::getInt(window, "Введите количество точек",
                                            "Количество точек:", pointCountCsv,
                                            pointCountCsv, INT_MAX, 1, &ok);
      if (ok) {
        if (pointCount >= pointCountCsv) {
          pointPlotCount = pointCount;
          QLineSeries *series = new QLineSeries();
          seriesList.push_back(series);

          if (chart->series().contains(pointSeries)) {
            chart->removeSeries(pointSeries);
            delete pointSeries;
            pointSeries = nullptr;
          }

          pointSeries = new QScatterSeries();

          // Настраиваем радиус и стиль маркера для точек из файла
          pointSeries->setMarkerSize(8);  // Размер маркера
          pointSeries->setMarkerShape(
              QScatterSeries::MarkerShapeCircle);  // Форма маркера
          pointSeries->setColor(Qt::red);          // Цвет маркера

          int polynomialDegree = polynomialDegreeSpinBox->value();
          int forecastDayCount = forecastDayCountSpinBox->value();

          std::time_t minDate = std::numeric_limits<std::time_t>::max();
          std::time_t maxDate = std::numeric_limits<std::time_t>::min();
          double minCost = INT_MAX;
          double maxCost = INT_MIN;

          std::vector<s21::GraphEntry> mapFromFile = data->GetMap();
          for (const auto &pointFromFile : mapFromFile) {
            minDate = std::min(minDate, pointFromFile.date);
            maxDate = std::max(maxDate, pointFromFile.date);
            minCost = std::min(minCost, pointFromFile.cost);
            maxCost = std::max(maxCost, pointFromFile.cost);
            QPointF dataPoint(QDateTime::fromSecsSinceEpoch(pointFromFile.date)
                                  .toMSecsSinceEpoch(),
                              pointFromFile.cost);
            pointSeries->append(dataPoint);
          }

          Approximation approximation(polynomialDegree, forecastDayCount,
                                      pointCount);
          ApproximationResult apprRes = approximation.Approximate(*data);
          vector<GraphEntry> appr = apprRes.GetApproximatedGraph();

          for (auto apprPoint : appr) {
            minDate = std::min(minDate, apprPoint.date);
            maxDate = std::max(maxDate, apprPoint.date);
            minCost = std::min(minCost, apprPoint.cost);
            maxCost = std::max(maxCost, apprPoint.cost);
            QPointF dataPoint(QDateTime::fromSecsSinceEpoch(apprPoint.date)
                                  .toMSecsSinceEpoch(),
                              apprPoint.cost);
            seriesList.back()->append(dataPoint);
          }

          chart->addSeries(seriesList.back());
          chart->addSeries(pointSeries);

          QString chartName = QString("Точек: %1. Полином: %2.")
                                  .arg(pointCount)
                                  .arg(polynomialDegree);
          seriesList.back()->setName(chartName);

          if (!chart->axes(Qt::Horizontal).contains(xAxis)) {
            // Увеличиваем диапазон оси Y на 10% с каждой стороны
            double increaseFactor = 1.1;
            double newYMin =
                minCost - (maxCost - minCost) * (increaseFactor - 1) / 2;
            double newYMax =
                maxCost + (maxCost - minCost) * (increaseFactor - 1) / 2;

            // Устанавливаем новые значения диапазона осей X и Y
            xAxis->setRange(QDateTime::fromSecsSinceEpoch(minDate),
                            QDateTime::fromSecsSinceEpoch(maxDate));
            yAxis->setRange(newYMin, newYMax);
            chart->addAxis(xAxis, Qt::AlignBottom);
            chart->addAxis(yAxis, Qt::AlignLeft);
          }

          seriesList.back()->attachAxis(xAxis);
          seriesList.back()->attachAxis(yAxis);
          pointSeries->attachAxis(xAxis);
          pointSeries->attachAxis(yAxis);
          chart->legend()->markers(pointSeries).at(0)->setVisible(false);
        } else {
          QMessageBox::warning(window, "Ошибка",
                               "Количество точек должно быть не меньше, чем в "
                               "загруженном csv файле!");
        }
      }
    } else {
      QMessageBox::warning(
          window, "Ошибка",
          "На поле может быть одновременно изображено не более 5 графиков");
    }
  } else {
    QMessageBox::warning(window, "Ошибка", "Сначала загрузите файл с данными");
  }
}

// Функция для возврата значения акций на выбранную дату и время (для
// аппроксимации)
void ValueByDateApproximation(Trade *data, QDateTimeEdit *dateTimeEdit,
                              QLabel *resultValueLabel, QMainWindow *window,
                              int polynomialDegree, int forecastDayCount,
                              int pointCount) {
  if (data && data->GetSize() > 0) {
    // Получаем выбранную пользователем дату и время из dateTimeEdit
    QDateTime selectedDateTime = dateTimeEdit->dateTime();
    std::time_t timeT = selectedDateTime.toSecsSinceEpoch();

    QString formattedDateTime =
        selectedDateTime.toString("yyyy-MM-dd HH:mm:ss");

    Approximation approximation(polynomialDegree, forecastDayCount, pointCount);
    ApproximationResult apprRes = approximation.Approximate(*data);

    resultValueLabel->setText(QString("Стоимость акций на %1: %2")
                                  .arg(formattedDateTime)
                                  .arg(apprRes.GetSinglePoint(timeT).cost));
  } else {
    QMessageBox::warning(window, "Ошибка", "Сначала загрузите файл с данными");
  }
}
