#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringList>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include <fstream>
#include <vector>
#include <map>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>

#include "simulator/SimulationView.hpp"

using namespace QtCharts;

namespace sim = simulation;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_queuePolicySelector_currentIndexChanged(const QString &arg1);

    void on_quitButton_clicked();

    void on_runButton_clicked();

private:
    Ui::MainWindow *ui;

    auto insertDataRow(sim::view::DataRow const& dr) -> void;

    auto saveResults(sim::Model const& model) -> void;

    auto newStateAcquired(sim::Model const& model) -> void;

    sim::Model model;
};

#endif // MAINWINDOW_H
