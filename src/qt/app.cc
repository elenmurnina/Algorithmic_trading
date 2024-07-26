#include <QPushButton>

#include "buttons.cc"

using namespace s21;

int appExecution(int argc, char **argv) {
  QApplication app(argc, argv);
  QMainWindow window;
  QWidget *centralWidget = new QWidget(&window);
  setlocale(LC_ALL, "en_US.UTF-8");

  // Создаём вертикальный макет для виджета Интерполяции
  QVBoxLayout *interpolationLayout = new QVBoxLayout;

  // Создаём вертикальный макет для кнопок Интерполяции
  QVBoxLayout *buttonInterpolationLayout = new QVBoxLayout;
  QPushButton *loadInterpolationButton =
      new QPushButton("Загрузить CSV для интерполяции");
  QPushButton *plotInterpolationButton =
      new QPushButton("Построить график, полученный методом интерполяции");
  QPushButton *byDateInterpolationButton = new QPushButton(
      "Вывести значение котировок акций на указанную дату и время");

  QPushButton *clearInterpolationButton =
      new QPushButton("Очистить поле интерполяции");

  // Создаём поля для ввода даты и времени для метода интерполяции
  QDateTimeEdit *dateTimeInterpolationEdit = new QDateTimeEdit;
  dateTimeInterpolationEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

  // Создаём область для отображения результатов метода интерполяции
  QLabel *resultInterpolationValueLabel =
      new QLabel("Здесь будет отображаться результат метода интерполяции");

  // Создаём объект графика интерполяции
  QChart *chartInterpolation = new QChart();
  QChartView *chartInterpolationView = new QChartView(chartInterpolation);
  chartInterpolationView->setRenderHint(QPainter::Antialiasing);

  buttonInterpolationLayout->addWidget(loadInterpolationButton);
  buttonInterpolationLayout->addWidget(plotInterpolationButton);
  buttonInterpolationLayout->addWidget(byDateInterpolationButton);
  buttonInterpolationLayout->addWidget(dateTimeInterpolationEdit);
  buttonInterpolationLayout->addWidget(resultInterpolationValueLabel);
  buttonInterpolationLayout->addWidget(clearInterpolationButton);

  interpolationLayout->addLayout(buttonInterpolationLayout);
  interpolationLayout->addWidget(chartInterpolationView);

  // Создаём вертикальный макет для виджета Аппроксимации
  QVBoxLayout *approximationLayout = new QVBoxLayout;

  // Создаём вертикальный макет для кнопок Аппроксимации
  QVBoxLayout *buttonApproximationLayout = new QVBoxLayout;
  QPushButton *loadApproximationButton =
      new QPushButton("Загрузить CSV для аппроксимации");
  QPushButton *plotApproximationButton = new QPushButton(
      "Построить график, полученный методом наименьших квадратов");
  QPushButton *byDateApproximationButton = new QPushButton(
      "Вывести значение котировок акций на указанную дату и время");
  QPushButton *clearApproximationButton =
      new QPushButton("Очистить поле аппроксимации");

  // Создаём горизонтальный макет
  // для полей "Степень полинома" и "Количество дней прогноза"
  QHBoxLayout *polynomialDegreeForecastDayCountLayout = new QHBoxLayout;
  QLabel *polynomialDegreeLabel = new QLabel("Степень полинома:");
  QSpinBox *polynomialDegreeSpinBox = new QSpinBox;
  polynomialDegreeSpinBox->setRange(
      2, 10);  // Настраиваем диапазон степеней полиномов

  QLabel *forecastDayCountLabel = new QLabel("Количество дней прогноза:");
  QSpinBox *forecastDayCountSpinBox = new QSpinBox;
  forecastDayCountSpinBox->setRange(
      0, 10000);  // Настраиваем диапазон количества дней прогноза

  // Добавляем QLabel и QSpinBox в горизонтальный макет
  polynomialDegreeForecastDayCountLayout->addWidget(polynomialDegreeLabel);
  polynomialDegreeForecastDayCountLayout->addWidget(polynomialDegreeSpinBox);
  polynomialDegreeForecastDayCountLayout->addWidget(forecastDayCountLabel);
  polynomialDegreeForecastDayCountLayout->addWidget(forecastDayCountSpinBox);
  int forecastDayCount = 0;

  // Создаём поля для ввода даты и времени для метода аппроксимации
  QDateTimeEdit *dateTimeApproximationEdit = new QDateTimeEdit;
  dateTimeApproximationEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

  // Создаём область для отображения результатов метода интерполяции
  QLabel *resultApproximationValueLabel =
      new QLabel("Здесь будет отображаться результат метода аппроксимации");

  // Создаём объект графика аппроксимации
  QChart *сhartApproximation = new QChart();
  QChartView *сhartApproximationView = new QChartView(сhartApproximation);
  сhartApproximationView->setRenderHint(QPainter::Antialiasing);

  buttonApproximationLayout->addWidget(loadApproximationButton);
  buttonApproximationLayout->addWidget(plotApproximationButton);
  buttonApproximationLayout->addWidget(byDateApproximationButton);
  buttonApproximationLayout->addWidget(dateTimeApproximationEdit);
  buttonApproximationLayout->addWidget(resultApproximationValueLabel);
  buttonApproximationLayout->addLayout(polynomialDegreeForecastDayCountLayout);
  buttonApproximationLayout->addWidget(clearApproximationButton);

  approximationLayout->addLayout(buttonApproximationLayout);
  approximationLayout->addWidget(сhartApproximationView);

  // Создаём горизонтальный макет для размещения двух вертикальных макетов
  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addLayout(interpolationLayout);
  horizontalLayout->addLayout(approximationLayout);

  // Устанавливаем центральный виджет в главное окно
  centralWidget->setLayout(horizontalLayout);
  window.setCentralWidget(centralWidget);

  window.resize(1000, 600);
  window.show();

  //  Создаём массив объектов для данных графика
  QVector<QLineSeries *> seriesInterpolationList;
  QVector<QLineSeries *> seriesApproximationList;

  InterpolationMethod interpolationMethod = CUBIC_SPLINE;

  QScatterSeries *pointSeriesApproximation = nullptr;

  QDateTimeAxis *xInterpolationAxis = nullptr;
  QValueAxis *yInterpolationAxis = nullptr;
  QDateTimeAxis *xApproximationAxis = nullptr;
  QValueAxis *yApproximationAxis = nullptr;

  Trade *dataInterpolation = nullptr;
  Trade *dataApproximation = nullptr;
  int pointPlotApproximationCount;
  std::string filename;

  QObject::connect(loadInterpolationButton, &QPushButton::clicked, [&]() {
    ClearData(dataInterpolation);
    resultInterpolationValueLabel->setText(
        "Здесь будет отображаться результат метода интерполяции");
    dataInterpolation = LoadCSVFile(
        chartInterpolation, seriesInterpolationList, dateTimeInterpolationEdit,
        xInterpolationAxis, yInterpolationAxis, &window, filename);
  });

  QObject::connect(plotInterpolationButton, &QPushButton::clicked, [&]() {
    PlotInterpolation(dataInterpolation, chartInterpolation,
                      seriesInterpolationList, &window, xInterpolationAxis,
                      yInterpolationAxis, interpolationMethod);
  });

  QObject::connect(byDateInterpolationButton, &QPushButton::clicked, [&]() {
    ValueByDateInterpolation(dataInterpolation, dateTimeInterpolationEdit,
                             resultInterpolationValueLabel, &window,
                             interpolationMethod);
  });

  QObject::connect(clearInterpolationButton, &QPushButton::clicked, [&]() {
    ClearData(dataInterpolation);
    ClearAndUpdateXYAxis(xInterpolationAxis, yInterpolationAxis, false);
    ClearField(chartInterpolation, chartInterpolationView, interpolationLayout,
               seriesInterpolationList);
    dateTimeInterpolationEdit->clearMinimumDate();
    dateTimeInterpolationEdit->clearMaximumDate();
    resultInterpolationValueLabel->setText(
        "Здесь будет отображаться результат метода интерполяции");
  });

  QObject::connect(loadApproximationButton, &QPushButton::clicked, [&]() {
    ClearData(dataApproximation);
    if (pointSeriesApproximation) {
      delete pointSeriesApproximation;
      pointSeriesApproximation = nullptr;
    }
    resultApproximationValueLabel->setText(
        "Здесь будет отображаться результат метода аппроксимации");
    dataApproximation = LoadCSVFile(
        сhartApproximation, seriesApproximationList, dateTimeApproximationEdit,
        xApproximationAxis, yApproximationAxis, &window, filename);
  });

  QObject::connect(plotApproximationButton, &QPushButton::clicked, [&]() {
    if (forecastDayCount != forecastDayCountSpinBox->value()) {
      forecastDayCount = forecastDayCountSpinBox->value();
      ClearAndUpdateXYAxis(xApproximationAxis, yApproximationAxis, true);
      ClearField(сhartApproximation, сhartApproximationView,
                 approximationLayout, seriesApproximationList);
      ProcessDate(dateTimeApproximationEdit, dataApproximation,
                  forecastDayCount);
      сhartApproximation->setTitle(
          QString("График для %1").arg(filename.c_str()));
      resultApproximationValueLabel->setText(
          "Здесь будет отображаться результат метода аппроксимации");
    }
    PlotApproximation(dataApproximation, сhartApproximation,
                      seriesApproximationList, pointSeriesApproximation,
                      &window, xApproximationAxis, yApproximationAxis,
                      polynomialDegreeSpinBox, forecastDayCountSpinBox,
                      pointPlotApproximationCount);
  });

  QObject::connect(byDateApproximationButton, &QPushButton::clicked, [&]() {
    ValueByDateApproximation(dataApproximation, dateTimeApproximationEdit,
                             resultApproximationValueLabel, &window,
                             polynomialDegreeSpinBox->value(),
                             forecastDayCountSpinBox->value(),
                             pointPlotApproximationCount);
  });

  QObject::connect(clearApproximationButton, &QPushButton::clicked, [&]() {
    ClearData(dataApproximation);
    ClearAndUpdateXYAxis(xApproximationAxis, yApproximationAxis, false);
    ClearField(сhartApproximation, сhartApproximationView, approximationLayout,
               seriesApproximationList);
    dateTimeApproximationEdit->clearMinimumDate();
    dateTimeApproximationEdit->clearMaximumDate();
    resultApproximationValueLabel->setText(
        "Здесь будет отображаться результат метода аппроксимации");
  });
  return app.exec();
}
