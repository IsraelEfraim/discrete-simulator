#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "simulator/Simulation.hpp"
#include "simulator/Export.hpp"

#include "DistributionRegex.hpp"

#include <chrono>
#include <thread>
#include <iostream>

#include "ParallelSimulator.hpp"

auto makeCell(QString const& name) -> QTableWidgetItem* {
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(name);

    return item;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

auto MainWindow::insertDataRow(sim::view::DataRow const& dr) -> void {
    auto index = this->ui->simulationTableView->rowCount();
    this->ui->simulationTableView->insertRow(index);

    auto entity = makeCell(QString::number(dr.entityId));
    auto clock = makeCell(QString::number(dr.clock));
    auto event = makeCell(QString::fromStdString(sim::view::getEventTypeName(dr.eventType)));
    auto queueSize = makeCell(QString::number(dr.queueSize));
    auto tc = makeCell(dr.tc.first ? QString::number(dr.tc.second) : QString("-"));
    auto calendar = makeCell(QString::fromStdString(sim::view::getCalendar(dr.calendar)));

    auto avgQueueSize = makeCell(QString::number(dr.generalStatistics.averageQueueSize));
    auto avgResourceBusy = makeCell(QString::number(dr.generalStatistics.averageResourceBusy));
    auto avgQueueTime = makeCell(QString::number(dr.generalStatistics.averageQueueTime));
    auto avgTotalTime = makeCell(QString::number(dr.generalStatistics.averageTotalTime));
    auto entityOut = makeCell(QString::number(dr.generalStatistics.entityOut));
    auto entityMax = makeCell(QString::number(dr.generalStatistics.entityMax));

    /* state */
    this->ui->simulationTableView->setItem(index, 0, entity);
    this->ui->simulationTableView->setItem(index, 1, clock);
    this->ui->simulationTableView->setItem(index, 2, event);
    this->ui->simulationTableView->setItem(index, 3, queueSize);
    this->ui->simulationTableView->setItem(index, 4, tc);
    this->ui->simulationTableView->setItem(index, 5, calendar);

    /* statistics */
    this->ui->simulationTableView->setItem(index, 6, avgQueueSize);
    this->ui->simulationTableView->setItem(index, 7, avgResourceBusy);
    this->ui->simulationTableView->setItem(index, 8, avgQueueTime);
    this->ui->simulationTableView->setItem(index, 9, avgTotalTime);
    this->ui->simulationTableView->setItem(index, 10, entityOut);
    this->ui->simulationTableView->setItem(index, 11, entityMax);
}

void MainWindow::on_queuePolicySelector_currentIndexChanged(const QString &arg1)
{
    this->ui->queuePolicyValue->setEnabled(arg1 == "Limited");
}

void MainWindow::on_quitButton_clicked()
{
    QApplication::quit();
}

auto MainWindow::newStateAcquired(sim::Model const& model) -> void {
    this->insertDataRow(sim::view::makeDataRow(model.state));
    if (this->ui->simulationSpeed->value() > 0) {
        this->ui->simulationTableView->scrollToBottom();
    }
}

auto MainWindow::saveResults(sim::Model const& model) -> void {
    if (QMessageBox::question(this, "Save results", "Do you wish to save the simulation results?") == QMessageBox::Yes) {
        auto saveName = QFileDialog::getSaveFileName(this, "Save results");

        sim::persist::toFile(model.state, saveName.toStdString());
    }
}

void MainWindow::on_runButton_clicked()
{
    this->ui->simulationTableView->clearContents();
    this->ui->simulationTableView->setRowCount(0);

    namespace dst = distribution;

    sim::QueuePolicy queuePolicy =
        this->ui->queuePolicySelector->currentText().toStdString() == "Limited"
        ? sim::limitedQueue(this->ui->queuePolicyValue->text().toULongLong())
        : sim::unlimitedQueue();

    this->model = {
        sim::State {
            sim::cleanState(0.0, this->ui->simulationTimeLimit->text().toDouble())
        },
        sim::SimulationController {
            dst::regex::match(this->ui->expressionTec->currentText().toStdString()),
            dst::regex::match(this->ui->expressionTs->currentText().toStdString()),
            queuePolicy
        }
    };

    qRegisterMetaType<sim::Model>();

    ParallelSimulator* sim = new ParallelSimulator(model, static_cast<size_t>(this->ui->simulationSpeed->value()));
    connect(sim, &ParallelSimulator::newState, this, &MainWindow::newStateAcquired);
    connect(sim, &ParallelSimulator::finished, this, &MainWindow::saveResults);
    sim->start(QThread::Priority::HighPriority);
}
