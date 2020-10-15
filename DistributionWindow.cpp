#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "distribution.hpp"

auto makeParamRow(QString const& name) -> std::pair<QTableWidgetItem*, QTableWidgetItem*> {
    QTableWidgetItem* param = new QTableWidgetItem();
    param->setFlags(param->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable ^ Qt::ItemIsEnabled);
    param->setText(name);

    QTableWidgetItem* value = new QTableWidgetItem();

    return std::make_pair(param, value);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->activeDir = QDir::currentPath();

    this->setWindowIcon(QIcon("icon.ico"));
    this->formatTableColumns();
    this->formatChartView();
}

auto MainWindow::formatTableColumns() -> void {
    ui->params->horizontalHeader()->setStretchLastSection(true);
    ui->params->setColumnWidth(0, 115);
}

auto MainWindow::updateTableRows(std::vector<QString> headers) -> void {
    int count = static_cast<int>(headers.size());

    ui->params->clearContents();
    ui->params->setRowCount(count);

    for (int i = 0; i < count; i++) {
        auto row = makeParamRow(headers.at(static_cast<size_t>(i)));
        ui->params->setItem(i, 0, row.first);
        ui->params->setItem(i, 1, row.second);
    }
}

auto MainWindow::fetchParams() -> std::map<QString, double> {
    std::map<QString, double> params;

    for (int i = 0; i < ui->params->rowCount(); i++) {
        QString param = ui->params->item(i, 0)->text();
        double value = ui->params->item(i, 1)->text().toDouble();

        params.insert({param, value});
    }

    return params;
}

auto MainWindow::saveAs(QString filename, std::vector<double> const& dst) -> void {
    std::ofstream file(filename.toStdString());

    for (auto num : dst) {
        file << num << std::endl;
    }

    file.close();
}

auto MainWindow::saveMapAs(QString filename, std::map<double, size_t> const& map) -> void {
    std::ofstream file(filename.toStdString());
    file << "bucket,count," << std::endl;

    for (auto bucket : map) {
        file << bucket.first << "," << bucket.second << "," << std::endl;
    }

    file.close();
}

auto MainWindow::on_cbDistribution_currentIndexChanged(const QString &dst) -> void {
    if (dst == "Uniform") {
        this->updateTableRows({"Minimum", "Maximum"});
    }
    else if (dst == "Triangular") {
        this->updateTableRows({"Minimum", "Maximum", "Mode"});
    }
    else if (dst == "Exponential") {
        this->updateTableRows({"μ", "x₀"});
    }
    else if (dst == "Normal") {
        this->updateTableRows({"σ", "μ"});
    }
}

auto MainWindow::on_btGenerate_clicked() -> void {
    if (ui->cbDistribution->currentIndex() >= 0) {
        std::function<double()> function;

        auto quantity = this->ui->sbQuantity->text().toULongLong();
        auto params = this->fetchParams();

        switch (ui->cbDistribution->currentIndex()) {
            case 0:
                function = distribution::uniform(params.at("Minimum"), params.at("Maximum"));
                break;
            case 1:
                function = distribution::triangular(params.at("Minimum"), params.at("Maximum"), params.at("Mode"));
                break;
            case 2:
                function = distribution::exponential(params.at("μ"), params.at("x₀"));
                break;
            case 3:
                function = distribution::normal(params.at("σ"), params.at("μ"));
                break;
        }

        auto dst = distribution::random_interval(function, quantity);
        auto histogram = distribution::histogram(dst);

        auto dstName = QFileDialog::getSaveFileName(this, "Generate As", this->activeDir);
        this->saveAs(dstName, dst);

        //auto path = dstName.mid(0, dstName.lastIndexOf("/") + 1);
        //auto name = dstName.mid(dstName.lastIndexOf("/") + 1, dstName.lastIndexOf(".") - path.size()).append("-histogram.csv");
        //this->saveMapAs(path.append(name), histogram);

        QMessageBox::information(this, "Interval Generated", "The interval was successfully generated at the target location 👍.", QMessageBox::Ok);

        this->activeDir = dstName.mid(0, dstName.lastIndexOf("/"));

        this->formatChart(histogram);
    }

}

auto MainWindow::on_btCloseApp_clicked() -> void {
    QApplication::quit();
}

auto MainWindow::formatChartView() -> void {
    this->chartView = new QChartView();
    this->chartView->setBackgroundBrush(QBrush(Qt::white));
    this->chartView->setFrameStyle(QFrame::StyledPanel);
    this->chartView->setMinimumSize(QSize(271, 228));

    this->ui->centerHorizontalLayout->addWidget(chartView);
}

auto MainWindow::formatChart(std::map<double, size_t> histogram) -> void {
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    QBarSeries *s = new QBarSeries();
    QBarSet *set = new QBarSet("h");

    for (auto const& pair : histogram) {
        axis->append(QString::number(pair.first));
        set->append(pair.second);
    }

    s->append(set);

    QChart *c = new QChart();
    c->setAnimationOptions(QChart::AllAnimations);
    c->legend()->hide();

    c->addSeries(s);
    c->createDefaultAxes();

    c->setAxisX(axis, s);

    chartView->setChart(c);
}

MainWindow::~MainWindow()
{
    delete ui;
}
