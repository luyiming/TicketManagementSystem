#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "importdialog.h"
#include "ui_importdialog.h"
#include "selldialog.h"
#include "ui_selldialog.h"
#include "magalgorithm.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QInputDialog>
#include <QDebug>
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDialog>
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connect(ui->modifyButton, &QPushButton::clicked, this, &MainWindow::modifyData);
    connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::loadFromFile);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::saveToFile);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshTable);
    //connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeItem);
    connect(ui->sellButton, &QPushButton::clicked, this, &MainWindow::sellTicket);

    QMenu *fileMenu = ui->menuBar->addMenu(tr("&File"));
    QMenu *helpMenu = ui->menuBar->addMenu(tr("&Help"));

    //火车Table初始化

    trainTable = new QTableWidget(this);
    trainTable->setColumnCount(9);
    trainTable->setRowCount(1);

    QStringList headers;
    headers << QString::fromLocal8Bit("班次号") << QString::fromLocal8Bit("始发站")
            << QString::fromLocal8Bit("终点站") << QString::fromLocal8Bit("发车时间")
            << QString::fromLocal8Bit("行车时间") << QString::fromLocal8Bit("票价")
            << QString::fromLocal8Bit("额定载客量") << QString::fromLocal8Bit("余票数")
            << QString::fromLocal8Bit("路线");
    trainTable->setHorizontalHeaderLabels(headers);
    trainTable->setEditTriggers(QAbstractItemView::NoEditTriggers);// 不可编辑
    trainTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    //trainTable->setSelectionMode(QAbstractItemView::ExtendedSelection);  //设置为可以选中多个目标
    trainTable->verticalHeader()->setVisible(false); //隐藏行表头

    trainTable->resizeColumnsToContents();
    trainTable->resizeRowsToContents();
    trainTable->setColumnWidth(0, 65);
    trainTable->setColumnWidth(1, 75);
    trainTable->setColumnWidth(2, 75);
    trainTable->setColumnWidth(3, 135);
    trainTable->setColumnWidth(5, 60);
    trainTable->setColumnWidth(8, 95);


    //订单table初始化

    ticketTable = new QTableWidget(this);
    ticketTable->setColumnCount(8);
    ticketTable->setRowCount(2);
    ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);// 不可编辑
    ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    //trainTable->setSelectionMode(QAbstractItemView::ExtendedSelection);  //设置为可以选中多个目标

    ticketTable->verticalHeader()->setVisible(false); //隐藏行表头

    headers.clear();
    headers << QString::fromLocal8Bit("订单号") << QString::fromLocal8Bit("订单人")
            << QString::fromLocal8Bit("车次号") << QString::fromLocal8Bit("始发站")
            << QString::fromLocal8Bit("终点站") << QString::fromLocal8Bit("票价")
            << QString::fromLocal8Bit("票数") << QString::fromLocal8Bit("总价");
    ticketTable->setHorizontalHeaderLabels(headers);

    ticketTable->resizeColumnsToContents();
    ticketTable->resizeRowsToContents();
    ticketTable->setColumnWidth(0, 100 * 1.25);
    ticketTable->setColumnWidth(1, 72 * 1.25);
    ticketTable->setColumnWidth(2, 68 * 1.25);
    ticketTable->setColumnWidth(3, 78 * 1.25);
    ticketTable->setColumnWidth(4, 78 * 1.25);
    ticketTable->setColumnWidth(5, 62 * 1.25);
    ticketTable->setColumnWidth(6, 60 * 1.25);
    ticketTable->setColumnWidth(7, 70 * 1.25);

    //添加火车和订单table
    ui->tabWidget->addTab(trainTable, QString::fromLocal8Bit("火车"));
    ui->tabWidget->addTab(ticketTable, QString::fromLocal8Bit("订单"));

    //显示时间label
    timeLabel = new QLabel(this);
    timeLabel->setGeometry(660, 451, 181, 16);
    QTimer *timer = new QTimer(this);
    displayTime();
    connect(timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    timer->start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::displayTime()
{
    QDateTime dt;
    QTime time;
    QDate date;

    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());

    QString currentDate = dt.toString("yyyy-MM-dd hh:mm:ss");
    timeLabel->setText(currentDate);
}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::modifyData()
{
    ImportDialog *importdlg = new ImportDialog(this);
    connect(importdlg, &ImportDialog::valueChanged, this, &MainWindow::setTrainInfo);
    importdlg->exec();

}
void MainWindow::setTrainInfo(QStringList data)
{
    int curRow = trainTable->rowCount();
    trainTable->insertRow(curRow);
    trainTable->setItem(curRow, 0, new QTableWidgetItem(data.at(0)));   //班次号
    trainTable->setItem(curRow, 1, new QTableWidgetItem(data.at(1)));   //始发站
    trainTable->setItem(curRow, 2, new QTableWidgetItem(data.at(2)));   //终点站
    trainTable->setItem(curRow, 3, new QTableWidgetItem(data.at(3)));   //发车时间
    trainTable->setItem(curRow, 4, new QTableWidgetItem(data.at(4)));   //行车时间
    trainTable->setItem(curRow, 5, new QTableWidgetItem(data.at(5)));   //票价
    trainTable->setItem(curRow, 6, new QTableWidgetItem(data.at(6)));   //额定载客量
    trainTable->setItem(curRow, 7, new QTableWidgetItem(data.at(6)));   //余票数就是额定载客数

    QComboBox *comBox = new QComboBox();
    QString routeData = data.at(7);
    std::vector<string> stations = splitString(routeData.toStdString(), "|");
    for(int i = 0; i < stations.size(); ++i)
        comBox->addItem(QString::fromStdString(stations[i]));

    trainTable->setCellWidget(curRow, 8, comBox);
    trainTable->resizeRowToContents(curRow);
}

void MainWindow::loadFromFile()
{
    trainRoutes.loadFromFile(this, trainTable);
}

void MainWindow::saveToFile()
{
    trainRoutes.saveToFile(this);
}

void MainWindow::refreshTable()
{
    //如果trainTable有焦点
    trainRoutes.refreshTrainInfo(trainTable);

    //如果orderTable有焦点
    //
    //remain update
    //****************************************
}

void MainWindow::sellTicket()
{
    int rowNum = trainTable->currentRow();
    if(rowNum >= 0)
    {
        QString trainID = trainTable->item(rowNum, 0)->text();
        QString departureTime = trainTable->item(rowNum, 3)->text();
        int index = trainRoutes.findRoute(trainID.toStdString(), departureTime.toStdString());

        if(index != -1)
        {
            trainTickets.sellTicket(index, trainRoutes, this);
        }

    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("购票失败"), QString::fromLocal8Bit("请选择要购买的车次"));
    }
}
//void MainWindow::removeItem()
//{
//    int rowNum = trainTable->currentRow();
//    //qDebug() << rowNum;
//    if(rowNum >= 0)
//    {
//        if(QMessage::Yes == QMessageBox::question(this,QString::fromLocal8Bit("删除确认"),QString::fromLocal8Bit("你确定要删除%1车次列车吗?").arg(trainTable->item(rowNum, 0)->text())))


//}

