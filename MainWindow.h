#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include <fstream>
#include <vector>
#include <map>

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

    QString activeDir;

    auto formatTableColumns() -> void;
    auto updateTableRows(std::vector<QString> headers) -> void;

    auto fetchParams() -> std::map<QString, double>;

    auto saveAs(QString filename, std::vector<double> const& dst) -> void;
    auto saveMapAs(QString filename, std::map<double, size_t> const& map) -> void;
};

#endif // MAINWINDOW_H
