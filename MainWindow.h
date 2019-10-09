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

using namespace QtCharts;

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
    void on_cbDistribution_currentIndexChanged(const QString &arg1);

    void on_btGenerate_clicked();
    void on_btCloseApp_clicked();

private:
    Ui::MainWindow *ui;

    QChartView *chartView;
    QString activeDir;

    auto formatTableColumns() -> void;
    auto updateTableRows(std::vector<QString> headers) -> void;

    auto fetchParams() -> std::map<QString, double>;

    auto saveAs(QString filename, std::vector<double> const& dst) -> void;
    auto saveMapAs(QString filename, std::map<double, size_t> const& map) -> void;

    auto formatChartView() -> void;
    auto formatChart(std::map<double, size_t> histogram) -> void;
};

#endif // MAINWINDOW_H
