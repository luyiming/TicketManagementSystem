#include "routes.h"
#include "magalgorithm.h"
#include <string>
#include <QString>
#include <sstream>
#include <vector>
#include <QTableWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTableWidget>

using namespace std;

void Routes::createRoute(QString &Qdata)
{
    std::string data = Qdata.toStdString();

    stringstream ss(data);

    //输入格式
    string trainID;    //火车班次
    string departureTimeYear;//出发时间
    string departureTimeDay;
    string drivingTime;    //行车时间
    int price;  //票价
    int maxPassengers;  //额定载客数
    string routeLine;//路线

    while(ss >> trainID >> departureTimeYear >> departureTimeDay >> drivingTime >> price >> maxPassengers >> routeLine)
    {
        Route tempRoute;
        tempRoute.trainID = trainID;
        tempRoute.departureTime = departureTimeYear + " " + departureTimeDay;
        tempRoute.drivingTime = drivingTime;
        tempRoute.price = price;
        tempRoute.maxPassengers = maxPassengers;

        std::vector<std::string> stations = splitString(routeLine, "|");
        for(int i = 0; i < stations.size(); ++i)
        {

            Station *tempStation = new Station;
            tempStation->name = stations[i];
            tempStation->seatsLeft = maxPassengers;
            tempStation->trainID = trainID;

            cities[stations[i]].push_back(tempStation); //添加城市列表

            if(tempRoute.startingStation == NULL)
            {
                tempRoute.startingStation = tempStation;
                tempRoute.terminalStation = tempStation;
            }
            else
            {
                tempRoute.terminalStation->next = tempStation;
                tempStation->prev = tempRoute.terminalStation;
                tempRoute.terminalStation = tempStation;
            }
        }
        routes.push_back(tempRoute);
    }

}



void Routes::refreshTrainInfo(QTableWidget* &trainTable)
{
    trainTable->setSortingEnabled(false);//若不加这句插入会有问题

    int rowNum = trainTable->rowCount();
    for(int i = rowNum -1; i >= 0 ; i--)
    {
        trainTable->removeRow(i);
    }
    for(int i = 0; i < routes.size(); ++i)
    {
        int curRow = trainTable->rowCount();
        trainTable->insertRow(curRow);
        trainTable->setItem(curRow, 0, new QTableWidgetItem(QString::fromStdString(routes[i].trainID)));   //班次号
        trainTable->setItem(curRow, 1, new QTableWidgetItem(QString::fromStdString(routes[i].startingStation->name)));   //始发站
        trainTable->setItem(curRow, 2, new QTableWidgetItem(QString::fromStdString(routes[i].terminalStation->name)));   //终点站
        trainTable->setItem(curRow, 3, new QTableWidgetItem(QString::fromStdString(routes[i].departureTime)));   //发车时间
        trainTable->setItem(curRow, 4, new QTableWidgetItem(QString::fromStdString(routes[i].drivingTime)));   //行车时间
        trainTable->setItem(curRow, 5, new QTableWidgetItem(QString::number(routes[i].price) + QString::fromLocal8Bit("元")));//票价
        trainTable->setItem(curRow, 6, new QTableWidgetItem(QString::number(routes[i].maxPassengers)));   //额定载客量
        trainTable->setItem(curRow, 7, new QTableWidgetItem(QString::number(routes[i].maxPassengers)));   //余票数就是额定载客数

        QComboBox *comboBox = new QComboBox();

        for(Station* p = routes[i].startingStation; p != NULL; p = p->next)
        {
            comboBox->addItem(QString::fromStdString(p->name) + "   " + QString::number(p->seatsLeft));
        }
        trainTable->setCellWidget(curRow, 8, comboBox);
        trainTable->resizeRowToContents(curRow);
    }

    //排序enable
    trainTable->setSortingEnabled(true);
}

void Routes::saveToFile(QWidget* mainWindow)
{
    QString path = QFileDialog::getSaveFileName(mainWindow,
                                                QString::fromLocal8Bit("请选择导出路径"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(mainWindow, tr("Write File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << QString::fromLocal8Bit("班次号       发车时间     行车时间  票价  额定载客量          路线\n");
        out << QString("-------------------------------------------------------------------------------------\n");
        for(int i = 0; i < routes.size(); ++i)
        {
            out << QString::fromStdString(routes[i].trainID) << "\t";
            out << QString::fromStdString(routes[i].departureTime) << "\t";
            out << QString::fromStdString(routes[i].drivingTime) << "\t";
            out << routes[i].price << "\t\t";
            out << routes[i].maxPassengers << "\t\t";
            for(Station* p = routes[i].startingStation; p != NULL; p = p->next)
            {
                out << QString::fromStdString(p->name) << " " << p->seatsLeft << "|";
            }
            out << endl;
        }
        file.close();
        QMessageBox::information(mainWindow, tr("Information"), QString::fromLocal8Bit("导出文件成功"));
    }
    else
    {
        QMessageBox::warning(mainWindow, tr("Path"), tr("You did not select any file."));
    }
}

void Routes::loadFromFile(QWidget *mainWindow, QTableWidget* &trainTable)
{
    QString path = QFileDialog::getOpenFileName(mainWindow,
                                                QString::fromLocal8Bit("请选择导入文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(mainWindow, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        in.readLine();
        in.readLine();
        QString str = in.readAll();
        createRoute(str);   //更新数据
        refreshTrainInfo(trainTable);  //刷新显示
        file.close();
    }
    else
    {
        QMessageBox::warning(mainWindow, tr("Path"), tr("You did not select any file."));
    }
}

int Routes::findRoute(string trainID, string departureTime)
{
    for(int i = 0; i < routes.size(); ++i)
    {
        if(routes[i].trainID == trainID && routes[i].departureTime == departureTime)
            return i;
    }
    return -1;
}

void Routes::deleteRoute(string trainID, string departureTime)
{
    int index = findRoute(trainID, departureTime);

}
