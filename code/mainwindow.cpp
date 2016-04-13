#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trainAMDialog.h"
#include "ui_trainAMDialog.h"
#include "selldialog.h"
#include "ui_selldialog.h"
#include "querydialog.h"
#include "ui_querydialog.h"
#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
#include "routes.h"
#include "RouteQueryDialog.h"
#include "ui_RouteQueryDialog.h"
#include "ExportDialog.h"
#include "ui_ExportDialog.h"
#include "OnlineLoadingDialog.h"
#include "ui_OnlineLoadingDialog.h"
#include "ImportDialog.h"
#include "ui_ImportDialog.h"
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
#include <QMenuBar>
#include <QAction>
#include <QProcess>
#include <QStringList>
#include <QDesktopServices>
#include <QIcon>
#include <QMimeData>
#include <QList>
#include <QEvent>

//----------------------------------------------------------------//
//                                                                //
//       To understand recursion, see the bottom of this file     //
//                                                                //
//----------------------------------------------------------------//

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/train"));

//------------------------------* Drag and Drop *--------------------------------------
    this->setAcceptDrops(true);
    ui->tabWidget->setAcceptDrops(false);
//----------------------------------* config *-----------------------------------------
    visitorMode = false;
    if(visitorMode == true)
        this->setWindowTitle(QString::fromLocal8Bit("车票管理系统(访客模式)"));
    else
        this->setWindowTitle(QString::fromLocal8Bit("车票管理系统(管理员模式)"));
    this->configInfo << "admin"
                     << "admin"
                     << "C:\\Users\\Magnolias\\Desktop\\trainExport.txt"
                     << "C:\\Users\\Magnolias\\Desktop\\ticketExport.txt"
                     << "C:\\Users\\Magnolias\\Desktop\\train.txt"
                     << "C:\\Users\\Magnolias\\Desktop\\ticket.txt"
                     << "true"
                     << "false";

    QFile configFile("train.conf");
    if(configFile.exists())
    {
        bool flag = true;
        if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            flag = false;
        }
        if(flag == true)
        {
            this->configInfo.clear();
            QTextStream in(&configFile);
            while(!in.atEnd())
                this->configInfo << in.readLine();
            configFile.close();
        }
    }

    if(this->configInfo.at(6) == "true")
        trainRoutes.setAutoExport(true);
    else
        trainRoutes.setAutoExport(false);
    if(this->configInfo.at(7) == "true")
        trainRoutes.setAutoImport(true);
    else
        trainRoutes.setAutoImport(false);
    trainRoutes.setTrainExportPath (this->configInfo.at(2));
    trainRoutes.setTicketExportPath(this->configInfo.at(3));
    trainRoutes.setTrainImportPath (this->configInfo.at(4));
    trainRoutes.setTicketImportPath(this->configInfo.at(5));

//--------------------------------* signal-slots *----------------------------------------
    connect(ui->importButton,    &QPushButton::clicked, this, &MainWindow::importDialog);
    connect(ui->exportButton,    &QPushButton::clicked, this, &MainWindow::exportDialog);
    connect(ui->refreshButton,   &QPushButton::clicked, this, &MainWindow::refreshTable);
    connect(ui->removeButton,    &QPushButton::clicked, this, &MainWindow::deleteRoute);
    connect(ui->sellButton,      &QPushButton::clicked, this, &MainWindow::sellTicketDialog);
    connect(ui->refundButton,    &QPushButton::clicked, this, &MainWindow::refundTicketDialog);
    connect(ui->addButton,       &QPushButton::clicked, this, &MainWindow::addTrainDialog);
    connect(ui->modifyButton,    &QPushButton::clicked, this, &MainWindow::modifyTrainDialog);
    connect(ui->queryButton,     &QPushButton::clicked, this, &MainWindow::queryDialog);
    connect(ui->routeQueryButton,&QPushButton::clicked, this, &MainWindow::openRouteQueryDialog);
//-----------------------------------* menu *--------------------------------------------
    QMenu *fileMenu   = ui->menuBar->addMenu(QString::fromLocal8Bit("文件"));
    QMenu *configMenu = ui->menuBar->addMenu(QString::fromLocal8Bit("选项"));
    QMenu *helpMenu   = ui->menuBar->addMenu(QString::fromLocal8Bit("帮助"));

    QAction *configAction = new QAction(QIcon(":/icon/config.png"), QString::fromLocal8Bit("首选项"), this);
    connect(configAction, &QAction::triggered, this, &MainWindow::configDialog);
    configMenu->addAction(configAction);

    QAction *exportAction = new QAction(QIcon(":/icon/export.png"), QString::fromLocal8Bit("导出车次"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::saveToFile);
    fileMenu->addAction(exportAction);

    QAction *ticketsExportAction = new QAction(QIcon(":/icon/export.png"), QString::fromLocal8Bit("导出票价"), this);
    connect(ticketsExportAction, &QAction::triggered, this, &MainWindow::ticketsSaveToFile);
    fileMenu->addAction(ticketsExportAction);

    QAction *importAction = new QAction(QIcon(":/icon/import.png"), QString::fromLocal8Bit("导入"), this);
    connect(importAction, &QAction::triggered, this, &MainWindow::importDialog);
    fileMenu->addAction(importAction);

    QAction *onlineImportAction = new QAction(QIcon(":/icon/import.png"), QString::fromLocal8Bit("从网络导入"), this);
    connect(onlineImportAction, &QAction::triggered, this, &MainWindow::onlineLoadingDialog);
    fileMenu->addAction(onlineImportAction);

    QAction *emptyAction = new QAction(QIcon(":/icon/refresh.png"), QString::fromLocal8Bit("清空"), this);
    connect(emptyAction, &QAction::triggered, this, &MainWindow::removeAll);
    fileMenu->addAction(emptyAction);

    QAction *helpAction = new QAction(QIcon(":/icon/help.png"), QString::fromLocal8Bit("用户手册"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::helpDialog);
    helpMenu->addAction(helpAction);
//-----------------------------------* table *-----------------------------------
    //train Table initialize
    trainTable = new QTableWidget(this);
    trainTable->setColumnCount(9);
    trainTable->setRowCount(0);

    connect(trainTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::modifyTrainDialog);

    QStringList headers;
    headers << QString::fromLocal8Bit("班次号") << QString::fromLocal8Bit("始发站")
            << QString::fromLocal8Bit("终点站") << QString::fromLocal8Bit("发车时间")
            << QString::fromLocal8Bit("行车时间") << QString::fromLocal8Bit("票价")
            << QString::fromLocal8Bit("额定载客量") << QString::fromLocal8Bit("余票数")
            << QString::fromLocal8Bit("路线/余票");
    trainTable->setHorizontalHeaderLabels(headers);
    trainTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // uneditable
    trainTable->setSelectionBehavior(QAbstractItemView::SelectRows);//select entire row
    trainTable->verticalHeader()->setVisible(false); //hide vertical header
    trainTable->setSelectionMode(QAbstractItemView::SingleSelection);//single selection
    //table width
    trainTable->resizeColumnsToContents();
    trainTable->resizeRowsToContents();
    trainTable->setColumnWidth(0, 65);
    trainTable->setColumnWidth(1, 75);
    trainTable->setColumnWidth(2, 75);
    trainTable->setColumnWidth(3, 140);
    trainTable->setColumnWidth(5, 60);
    trainTable->setColumnWidth(8, 115);

    //ticket table init
    ticketTable = new QTableWidget(this);
    ticketTable->setColumnCount(8);
    ticketTable->setRowCount(0);
    ticketTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // uneditable
    ticketTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ticketTable->setSelectionMode(QAbstractItemView::SingleSelection); //single selection
    ticketTable->verticalHeader()->setVisible(false);//hide vertical header
    //table header
    headers.clear();
    headers << QString::fromLocal8Bit("订单号") << QString::fromLocal8Bit("购票人")
            << QString::fromLocal8Bit("车次号") << QString::fromLocal8Bit("始发站")
            << QString::fromLocal8Bit("终点站") << QString::fromLocal8Bit("票价")
            << QString::fromLocal8Bit("票数") << QString::fromLocal8Bit("总价");
    ticketTable->setHorizontalHeaderLabels(headers);
    //table width
    ticketTable->resizeColumnsToContents();
    ticketTable->resizeRowsToContents();
    ticketTable->setColumnWidth(0,100 * 1.27);
    ticketTable->setColumnWidth(1, 72 * 1.27);
    ticketTable->setColumnWidth(2, 68 * 1.27);
    ticketTable->setColumnWidth(3, 78 * 1.27);
    ticketTable->setColumnWidth(4, 78 * 1.27);
    ticketTable->setColumnWidth(5, 62 * 1.27);
    ticketTable->setColumnWidth(6, 60 * 1.27);
    ticketTable->setColumnWidth(7, 70 * 1.27);

    //add table
    ui->tabWidget->addTab(trainTable, QString::fromLocal8Bit("火车"));
    ui->tabWidget->addTab(ticketTable, QString::fromLocal8Bit("订单"));

//----------------------------------* time label *------------------------------------

    timeLabel = new QLabel(this);
    QVBoxLayout *timeLayout = new QVBoxLayout(this);
    timeLayout->addWidget(timeLabel);
    ui->groupBox->setLayout(timeLayout);
    QTimer *timer = new QTimer(this);
    connect(timer,
            &QTimer::timeout,
            [&](){timeLabel->setText("  " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));});
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::modifyTrainDialog()
{
    if(this->visitorMode == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("游客身份无法修改车次"));
        return;
    }
    //set default content
    int curRow = trainTable->currentRow();
    if(curRow == -1)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("修改失败"), QString::fromLocal8Bit("请选择要修改的车次"));
        return;
    }
    else
    {
//        format
//        trainIDLine
//        departureTimeLine
//        drivingTimeLine
//        priceLine
//        maxPassengersLine
//        routesText  以|分隔
        QStringList trainData;
        trainData << trainTable->item(curRow, 0)->text();
        trainData << trainTable->item(curRow, 3)->text();
        trainData << trainTable->item(curRow, 4)->text();
        trainData << trainTable->item(curRow, 5)->text();
        trainData << trainTable->item(curRow, 6)->text();

        QComboBox *comboBox = (QComboBox*)trainTable->cellWidget(curRow, 8);
        QString singleRoute;
        for(int i = 0 ; i < comboBox->count(); ++i)//split route and seats data in the comboBox
        {
            if(i)
                singleRoute += '|';

            QString stationWithSeat = comboBox->itemText(i);
            QStringList t = stationWithSeat.split(" ", QString::SkipEmptyParts);
            if(t.size() < 2)
            {
                qDebug() << "wrong in modiftTrain()";
                return;
            }

            if(t.at(1) != trainData[4] && i != comboBox->count() - 1)
            {
                QMessageBox::warning(this, QString::fromLocal8Bit("修改失败"), QString::fromLocal8Bit("该车次已售票,不可修改"));
                return;
            }
            singleRoute += t.at(0);
        }
        trainData << singleRoute;

        trainAMDialog *modifyDialog = new trainAMDialog(this);
        modifyDialog->setTrainData(trainData);  //set default data
        connect(modifyDialog, &trainAMDialog::modifyValueChanged, this, &MainWindow::modifyTrainInfo);
        modifyDialog->exec();
    }
}

void MainWindow::modifyTrainInfo(QString preTrainID, QString preDepatureTime, QStringList trainData)
{
    if(trainRoutes.modifyRoute(preTrainID, preDepatureTime, trainData) == true)
    {
        trainRoutes.refreshTrainInfo(trainTable);
        QMessageBox::information(this,  QString::fromLocal8Bit("成功"),  QString::fromLocal8Bit("修改成功"));
    }
    else
        QMessageBox::warning(this,  QString::fromLocal8Bit("失败"),  QString::fromLocal8Bit("有重复车次"));
}

void MainWindow::addTrainDialog()
{
    if(this->visitorMode == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("游客身份无法添加车次"));
        return;
    }
    trainAMDialog *modifyDialog = new trainAMDialog(this);
    connect(modifyDialog, &trainAMDialog::addValueChanged, this, &MainWindow::setTrainInfo);
    modifyDialog->exec();
}


void MainWindow::setTrainInfo(QStringList trainData)
{
    if(trainRoutes.createRoute(trainData) == true)
    {
        trainRoutes.refreshTrainInfo(trainTable);
        QMessageBox::information(this,  QString::fromLocal8Bit("成功"),  QString::fromLocal8Bit("添加成功"));
    }
    else
        QMessageBox::warning(this,  QString::fromLocal8Bit("失败"),  QString::fromLocal8Bit("不允许添加重复车次"));

}
void MainWindow::importDialog()
{
    ImportDialog *dialog = new ImportDialog(this);
    connect(dialog, &ImportDialog::loadFromFile, this, &MainWindow::loadFromFile);
    connect(dialog, &ImportDialog::onlineLoad, this, &MainWindow::onlineLoadingDialog);
    dialog->exec();
}

void MainWindow::loadFromFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromLocal8Bit("请选择导入文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    int importNum = 0, skipNum = 0;
    QString skipLine;
    if(trainRoutes.loadFromFile(path, importNum, skipNum, skipLine) == true)
    {
        trainRoutes.refreshTrainInfo(trainTable);
        if(skipNum == 0)
            QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("共导入%1条车次信息").arg(importNum));
        else
            QMessageBox::information(this,
                                     QString::fromLocal8Bit("成功"),
                                     QString::fromLocal8Bit("共导入%1条车次信息\n有%2条重复车次信息未导入:  \n%3").arg(importNum).arg(skipNum).arg(skipLine));
    }
    else
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("无法打开文件"));

}

void MainWindow::exportDialog()
{
    ExportDialog *exportDialog = new ExportDialog(this);
    connect(exportDialog, &ExportDialog::trainExport, this, &MainWindow::saveToFile);
    connect(exportDialog, &ExportDialog::ticketExport, this, &MainWindow::ticketsSaveToFile);
    exportDialog->exec();
}

void MainWindow::saveToFile()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                QString::fromLocal8Bit("请选择导出文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    if(trainRoutes.saveToFile(path))
        QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("导出文件成功"));
    else
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("无法打开文件"));
}

void MainWindow::ticketsSaveToFile()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                QString::fromLocal8Bit("请选择导出文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    if(trainRoutes.ticketsSaveToFile(path))
        QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("导出文件成功"));
    else
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("无法打开文件"));
}

void MainWindow::refreshTable()
{
    trainRoutes.refreshTrainInfo(trainTable);
    trainRoutes.refreshTicketInfo(ticketTable);
}

void MainWindow::sellTicketDialog()
{
    int rowNum = trainTable->currentRow();
    if(rowNum >= 0)
    {
        QString trainID_ = trainTable->item(rowNum, 0)->text();
        QString departureTime_ = trainTable->item(rowNum, 3)->text();
        QString price_ = trainTable->item(rowNum, 5)->text();
        QStringList routesWithSeats;
        QComboBox *comboBox = (QComboBox*)trainTable->cellWidget(rowNum, 8);
        for(int i = 0 ; i < comboBox->count(); ++i)
        {
            routesWithSeats << comboBox->itemText(i);
        }

        SellDialog *sellDialog = new SellDialog(this);
        sellDialog->setTicketData(trainID_, departureTime_, price_, routesWithSeats);
        connect(sellDialog, &SellDialog::buyTicket, this, &MainWindow::sellTicket);
        sellDialog->exec();
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("购票失败"), QString::fromLocal8Bit("请选择要购买的车次"));
    }
}

void MainWindow::sellTicket(Ticket ticket)
{
    trainRoutes.sellTicket(ticket);
    trainRoutes.refreshTrainInfo(trainTable);
    trainRoutes.refreshTicketInfo(ticketTable);
}

void MainWindow::deleteRoute()
{
    if(this->visitorMode == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("游客身份无法删除车次"));
        return;
    }
    int rowNum = trainTable->currentRow();
    if(rowNum >= 0)
    {
        QComboBox *comboBox = (QComboBox*)trainTable->cellWidget(rowNum, 8);
        for(int i = 0; i < comboBox->count(); ++i)
        {
            QStringList routesWithSeats = comboBox->itemText(i).split(QString(" "), QString::SkipEmptyParts);
            if(routesWithSeats.size() < 2)
                qDebug() << "error in deleteRoute()";
            if(routesWithSeats[1] != trainTable->item(rowNum, 6)->text() && i != comboBox->count() - 1)
            {
                QMessageBox::warning(this, QString::fromLocal8Bit("删除失败"), QString::fromLocal8Bit("该车次已售票,不可删除"));
                return;
            }
        }

        if(QMessageBox::Yes == QMessageBox::question(this,QString::fromLocal8Bit("删除确认"),QString::fromLocal8Bit("你确定要删除%1车次列车吗?").arg(trainTable->item(rowNum, 0)->text())))
        {
            trainRoutes.deleteRoute(trainTable->item(rowNum, 0)->text(),
                                    trainTable->item(rowNum, 3)->text());
            //trainRoutes.refreshTrainInfo(trainTable);
            trainTable->removeRow(rowNum);  //much fater
        }
        else
            return;
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("删除失败"), QString::fromLocal8Bit("请选择要删除的车次"));
    }
}

void MainWindow::refundTicketDialog()
{
    int rowNum = ticketTable->currentRow();
    if(rowNum >= 0)
    {
        if(QMessageBox::Yes == QMessageBox::question(this,QString::fromLocal8Bit("退票确认"),QString::fromLocal8Bit("你确定要删除%1号订单吗?").arg(ticketTable->item(rowNum, 0)->text())))
        {
            int ticketID = ticketTable->item(rowNum, 0)->text().toInt();
            trainRoutes.refundTicket(ticketID);
            trainRoutes.refreshTrainInfo(trainTable);
            ticketTable->removeRow(rowNum);
            //trainRoutes.refreshTicketInfo(ticketTable);
        }
        else
            return;
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("退票失败"), QString::fromLocal8Bit("请选择要退的票"));
    }
}

void MainWindow::queryDialog()
{
    QueryDialog *queryDialog = new QueryDialog(this);
    connect(queryDialog, &QueryDialog::filterDataChanged, this, &MainWindow::filterTrainData);
    queryDialog->show();
}

void MainWindow::filterTrainData(QString trainID, QString startingStation, QString terminalStation)
{
    //refresh
    for(int i = 0; i < trainTable->rowCount(); ++i)
        trainTable->showRow(i);
    //filter trainID
    for(int i = 0; i < trainTable->rowCount(); ++i)
    {
        if(!trainTable->item(i, 0)->text().contains(trainID))
            trainTable->hideRow(i);
    }
    //filter starting station
    for(int rowNum = 0; rowNum < trainTable->rowCount(); ++rowNum)
    {
        bool isFind = false;
        QComboBox *comboBox = (QComboBox*)trainTable->cellWidget(rowNum, 8);
        for(int i = 0 ; i < comboBox->count(); ++i)
        {
            if(comboBox->itemText(i).contains(startingStation))
            {
                comboBox->setCurrentIndex(i);
                isFind = true;
                break;
            }
        }
        if(isFind == false)
            trainTable->hideRow(rowNum);
    }

    //filter terminal station
    for(int rowNum = 0; rowNum < trainTable->rowCount(); ++rowNum)
    {
        bool isFind = false;
        QComboBox *comboBox = (QComboBox*)trainTable->cellWidget(rowNum, 8);
        int startingStationIndex = comboBox->currentIndex();
        for(int i = 0 ; i < comboBox->count(); ++i)
        {
            //terminal should be after startingStation
            if(comboBox->itemText(i).contains(terminalStation) && startingStationIndex < i)
            {
                isFind = true;
                break;
            }
        }
        if(isFind == false)
            trainTable->hideRow(rowNum);
    }
}

void MainWindow::openRouteQueryDialog()
{
    this->routeQueryDialog = new RouteQueryDialog(this);
    QStringList result;
    connect(routeQueryDialog,
            &RouteQueryDialog::sendQuery,
            [&](QString s, QString t, int mode){result.clear();result = trainRoutes.queryRoute(s, t, mode);routeQueryDialog->setLine(result);});
    this->routeQueryDialog->exec();

}


void MainWindow::configDialog()
{
    if(this->visitorMode == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("游客身份无法修改配置文件"));
        return;
    }
    ConfigDialog *configDialog = new ConfigDialog(this);
    configDialog->setDefaultInfo(this->configInfo);
    connect(configDialog, &ConfigDialog::configChanged, this, &MainWindow::setConfig);
    configDialog->exec();
}

void MainWindow::setConfig(QStringList newConfig)
{
    QFile configFilePath("train.conf");

    if (!configFilePath.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("导出配置文件失败"));
        return;
    }
    QTextStream out(&configFilePath);
    for(int i = 0; i < newConfig.size(); ++i)
    {
        out << newConfig.at(i) << endl;
    }
    configFilePath.close();

    this->configInfo.clear();
    this->configInfo = newConfig;

    if(this->configInfo.at(6) == "true")//set auto flag first
        trainRoutes.setAutoExport(true);
    else
        trainRoutes.setAutoExport(false);
    if(this->configInfo.at(7) == "true")
        trainRoutes.setAutoImport(true);
    else
        trainRoutes.setAutoImport(false);

    trainRoutes.setTrainExportPath(this->configInfo.at(2));
    trainRoutes.setTicketExportPath(this->configInfo.at(3));
    trainRoutes.setTrainImportPath(this->configInfo.at(4));
    trainRoutes.setTicketImportPath(this->configInfo.at(5));

    QMessageBox::information(this, QString::fromLocal8Bit("完成"), QString::fromLocal8Bit("设置成功"));
}

void MainWindow::setVisitorMode(bool mode)
{
    this->visitorMode = mode;
    if(visitorMode == true)
        this->setWindowTitle(QString::fromLocal8Bit("车票管理系统(访客模式)"));
    else
        this->setWindowTitle(QString::fromLocal8Bit("车票管理系统(管理员模式)"));
}

void MainWindow::helpDialog()
{
    QDesktopServices bs;
    bs.openUrl(QUrl("help.docx"));
}

void MainWindow::removeAll()
{
    if(this->visitorMode == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("游客身份无法重置系统"));
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this, QString::fromLocal8Bit("清空"), QString::fromLocal8Bit("你确定删除所有车次车票信息吗?\n删除操作无法复原!")))
    {
        trainRoutes.removeAll();
        this->refreshTable();
    }
    else
        return;
}

void MainWindow::autoImport()
{
    if(this->configInfo.at(7) == "true")
    {
        int importNum = 0, skipNum = 0;
        QString skipLine;
        if(trainRoutes.loadFromFile(this->configInfo.at(4), importNum, skipNum, skipLine) == true)
        {
            trainRoutes.refreshTrainInfo(trainTable);
            if(skipNum == 0)
                QMessageBox::information(this, QString::fromLocal8Bit("自动导入"), QString::fromLocal8Bit("从%1导入%2条车次信息").arg(this->configInfo.at(4)).arg(importNum));
            else
                QMessageBox::information(this,
                                         QString::fromLocal8Bit("成功"),
                                         QString::fromLocal8Bit("共导入%1条车次信息\n有%2条重复车次信息未导入:  \n%3").arg(importNum).arg(skipNum).arg(skipLine));
        }
        else
            QMessageBox::warning(this, QString::fromLocal8Bit("自动导入"), QString::fromLocal8Bit("无法打开文件%1").arg(this->configInfo.at(4)));
    }
}

void MainWindow::onlineLoadingDialog()
{
    OnlineLoadingDialog *onlineImportDialog = new OnlineLoadingDialog(this);
    connect(onlineImportDialog, &OnlineLoadingDialog::beginLoadFile, this, &MainWindow::onlineLoadFile);
    onlineImportDialog->exec();
}

void MainWindow::onlineLoadFile()
{
    int importNum = 0, skipNum = 0;
    QString skipLine;
    if(trainRoutes.loadFromFile("train.data", importNum, skipNum, skipLine) == true)
    {
        trainRoutes.refreshTrainInfo(trainTable);
        if(skipNum == 0)
            QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("共导入%1条车次信息").arg(importNum));
        else
            QMessageBox::information(this,
                                     QString::fromLocal8Bit("成功"),
                                     QString::fromLocal8Bit("共导入%1条车次信息\n有%2条重复车次信息未导入:  \n%3").arg(importNum).arg(skipNum).arg(skipLine));
    }
    else
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("无法打开文件"));

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }

    int importNum = 0, skipNum = 0;
    QString skipLine;
    if(trainRoutes.loadFromFile(fileName, importNum, skipNum, skipLine) == true)
    {
        trainRoutes.refreshTrainInfo(trainTable);
        if(skipNum == 0)
            QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("共导入%1条车次信息").arg(importNum));
        else
            QMessageBox::information(this,
                                     QString::fromLocal8Bit("成功"),
                                     QString::fromLocal8Bit("共导入%1条车次信息\n有%2条重复车次信息未导入:  \n%3").arg(importNum).arg(skipNum).arg(skipLine));
    }
    else
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("无法打开文件"));
}

//----------------------------------------------------------------//
//                                                                //
//        To understand recursion, see the top of this file       //
//                                                                //
//----------------------------------------------------------------//
