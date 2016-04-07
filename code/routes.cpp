#include "routes.h"
#include "magalgorithm.h"
#include <QString>
#include <QTableWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTableWidget>
#include <QDebug>
#include <QList>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

//----------------------------------------------------------------//
//                                                                //
//       To understand recursion, see the bottom of this file     //
//                                                                //
//----------------------------------------------------------------//


bool Routes::modifyRoute(QString preTrainID, QString preDepatureTime, QStringList trainData)
{
    if(preTrainID != trainData[0] || preDepatureTime != trainData[1])
    {
        if(this->isTrainExist(trainData.at(0).toStdString(), trainData.at(1).toStdString()) == true)
        {
            return false;
        }
    }
    deleteRoute(preTrainID.toStdString(), preDepatureTime.toStdString());
    createRoute(trainData);
    if(isAutoExport == true)
        saveToFile(this->trainExportPath);
    return true;
}

bool Routes::createRoute(QStringList trainData)
{
    //format
    //trainID       0
    //departureTime 1
    //drivingTime   2
    //price         3
    //maxPassengers 4
    //routes        5->n
    if(isTrainExist(trainData[0].toStdString(), trainData[1].toStdString()) == true)
    {
        return false;
    }

    Route tempRoute;
    tempRoute.trainID = trainData[0].toStdString();
    tempRoute.departureTime = trainData[1].toStdString();
    tempRoute.drivingTime = trainData[2].toStdString();
    tempRoute.price = trainData[3].toDouble();
    tempRoute.maxPassengers = trainData[4].toInt();

    for(int i = 5; i < trainData.size(); ++i)
    {
        Station *tempStation = new Station;
        tempStation->name = trainData[i].toStdString();
        tempStation->seatsLeft = trainData[4].toInt();
        tempStation->trainID = trainData[0].toStdString();

        cities[tempStation->name].push_back(tempStation);

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
    if(isAutoExport == true)
        saveToFile(this->trainExportPath);
    return true;
}


void Routes::refreshTrainInfo(QTableWidget* &trainTable)
{
    trainTable->setSortingEnabled(false);   // Magic. Do not touch.

    int rowNum = trainTable->rowCount();
    for(int i = rowNum -1; i >= 0 ; i--)
    {
        trainTable->removeRow(i);
    }
    trainTable->clearContents();
    for(int i = 0; i < routes.size(); ++i)
    {
        int curRow = trainTable->rowCount();
        trainTable->insertRow(curRow);
        trainTable->setItem(curRow, 0, new TableItem(QString::fromStdString(routes[i].trainID)));
        trainTable->setItem(curRow, 1, new TableItem(QString::fromStdString(routes[i].startingStation->name)));
        trainTable->setItem(curRow, 2, new TableItem(QString::fromStdString(routes[i].terminalStation->name)));
        trainTable->setItem(curRow, 3, new TableItem(QString::fromStdString(routes[i].departureTime)));
        trainTable->setItem(curRow, 4, new TableItem(QString::fromStdString(routes[i].drivingTime)));
//        TableItem *newItem = new TableItem;
//        newItem->setTextAlignment(Qt::AlignHCenter |  Qt::AlignVCenter);

        trainTable->setItem(curRow, 5, new TableItem(routes[i].price));
        trainTable->setItem(curRow, 6, new TableItem(routes[i].maxPassengers));

        QComboBox *comboBox = new QComboBox();
        for(Station* p = routes[i].startingStation; p != NULL; p = p->next)
        {
            if(p->next == NULL)
                comboBox->addItem(QString::fromStdString(p->name) + "   0");
            else
                comboBox->addItem(QString::fromStdString(p->name) + "   " + QString::number(p->seatsLeft));

            if(p == routes[i].startingStation)
                trainTable->setItem(curRow, 7, new TableItem(p->seatsLeft));
        }

        trainTable->setCellWidget(curRow, 8, comboBox);
        trainTable->resizeRowToContents(curRow);
    }

    trainTable->setSortingEnabled(true);
}

void Routes::refreshTicketInfo(QTableWidget* &ticketTable)
{
    ticketTable->setSortingEnabled(false);

    int rowNum = ticketTable->rowCount();
    for(int i = rowNum -1; i >= 0 ; i--)
    {
        ticketTable->removeRow(i);
    }
    ticketTable->clearContents();
    for(int i = 0; i < this->tickets.size(); ++i)
    {
        int curRow = ticketTable->rowCount();
        ticketTable->insertRow(curRow);
        ticketTable->setItem(curRow, 0, new QTableWidgetItem(QString::number(tickets[i].ticketID)));
        ticketTable->setItem(curRow, 1, new QTableWidgetItem(tickets[i].name));
        ticketTable->setItem(curRow, 2, new QTableWidgetItem(tickets[i].trainID));
        ticketTable->setItem(curRow, 3, new QTableWidgetItem(tickets[i].routes[0]));
        ticketTable->setItem(curRow, 4, new QTableWidgetItem(tickets[i].routes[tickets[i].routes.size() - 1]));
        ticketTable->setItem(curRow, 5, new QTableWidgetItem(QString::number(tickets[i].price)));
        ticketTable->setItem(curRow, 6, new QTableWidgetItem(QString::number(tickets[i].number)));
        ticketTable->setItem(curRow, 7, new QTableWidgetItem(QString::number(tickets[i].price * tickets[i].number)));

        ticketTable->resizeRowToContents(curRow);
    }
    ticketTable->setSortingEnabled(true);
}


bool Routes::saveToFile(QString path)
{
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return false;
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
                if(p->next == NULL)
                    out << QString::fromStdString(p->name) << " 0|";
                else
                    out << QString::fromStdString(p->name) << " " << p->seatsLeft << "|";
            }
            out << endl;
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool Routes::ticketsSaveToFile(QString path)
{
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return false;
        }
        QTextStream out(&file);
        out << QString::fromLocal8Bit("订单号\t购票人\t车次号\t始发站\t终点站\t票价\t票数\t总价\n");
        out << QString("--------------------------------------------------------------\n");

        for(int i = 0; i < tickets.size(); ++i)
        {
            out << tickets[i].ticketID << "\t";
            out << tickets[i].name << "\t";
            out << tickets[i].trainID << "\t";
            out << tickets[i].routes[0] << "\t";
            out << tickets[i].routes[tickets[i].routes.size() - 1] << "\t";
            out << tickets[i].price << "\t\t";
            out << tickets[i].number << "\t\t";
            out << tickets[i].number * tickets[i].price;
            out << endl;
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool Routes::loadFromFile(QString path, int &importNum, int &skipNum, QString &skipLine)
{
    skipNum = importNum = 0;
    skipLine.clear();
    if(!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return false;
        }
        QTextStream in(&file);
        QString t = in.readLine();
        if(t.contains(","))
        {
            if(t.contains(" "))
            {
                QStringList trainData;
                while(1)
                {
                    QStringList sp1 = t.split(",");
                    trainData << sp1.at(0);     //trainID
                    trainData << "" << "" << "";//departureTime, drivingTime, price
                    QStringList sp2 = sp1.at(1).split("|", QString::SkipEmptyParts);
                    trainData << sp2.at(0).trimmed();   //maxPassengers
                    for(int i = 1; i < sp2.size(); ++i) //routes
                        trainData << sp2.at(i);
                    if(createRoute(trainData) == true)
                    {
                        importNum++;
                    }
                    else
                    {
                        if(skipNum)
                            skipLine += ", ";
                        skipLine += trainData.at(0);
                        skipNum++;
                    }
                    trainData.clear();
                    if(in.atEnd())
                        break;
                    t = in.readLine();
                }
            }
            else
            {
                QStringList trainData;
                while(1)
                {
                    QStringList sp1 = t.split(",");
                    trainData << sp1.at(0);
                    trainData << "" << "" << "";
                    QStringList sp2 = sp1.at(1).split("|", QString::SkipEmptyParts);
                    trainData << sp2.at(0);
                    for(int i = 1; i < sp2.size(); ++i)
                        trainData << sp2.at(i);
                    if(createRoute(trainData) == true)
                    {
                        importNum++;
                    }
                    else
                    {
                        if(skipNum)
                            skipLine += ", ";
                        skipLine += trainData.at(0);
                        skipNum++;
                    }
                    trainData.clear();
                    if(in.atEnd())
                        break;
                    t = in.readLine();
                }
            }
        }
        else if(!t.contains(","))
        {
            in.readLine();
            QStringList trainData;
            while(!in.atEnd())
            {
                t = in.readLine();
                QStringList sp1 = t.split(" ", QString::SkipEmptyParts);
                if(sp1.size() < 7)
                    continue;
                trainData << sp1.at(0)
                          << sp1.at(1) + " " + sp1.at(2)
                          << sp1.at(3)
                          << sp1.at(4)
                          << sp1.at(5);
                QStringList sp2 = sp1.at(6).split("|", QString::SkipEmptyParts);
                for(int i = 0; i < sp2.size(); ++i)//routes
                    trainData << sp2.at(i);
                if(createRoute(trainData) == true)
                {
                    importNum++;
                }
                else
                {
                    if(skipNum)
                        skipLine += ", ";
                    skipLine += trainData.at(0);
                    skipNum++;
                }
                trainData.clear();
            }
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

vector<Route>::iterator Routes::findRoute(string trainID, string departureTime)
{
    for(vector<Route>::iterator it = routes.begin(); it != routes.end(); ++it)
    {
        if(it->trainID == trainID && it->departureTime == departureTime)
            return it;
    }
    return routes.end();
}

bool Routes::isTrainExist(string trainID, string departureTime)
{
    if(findRoute(trainID, departureTime) == routes.end())
        return false;
    else
        return true;
}

void Routes::removeAll()
{
    for(int i = 1000; i < ticketID; ++i)
        refundTicket(i);
    cities.clear();
    while(!routes.empty())
        deleteRoute(routes[0].trainID, routes[0].departureTime);
    ticketID = 1000;
}

void Routes::deleteRoute(string trainID, string departureTime)
{
    vector<Route>::iterator index = findRoute(trainID, departureTime);
    if(index == routes.end())
    {
        qDebug() << "something may be wrong when transfering data, see routes.cpp--deleteRoute()";
        return;
    }
    Station *it1 = NULL;
    Station *it2 = (*index).startingStation;
    while(it2 != NULL)
    {
        //delete Station in the cities vector
        string name = it2->name;
        for(vector<Station*>::iterator it = cities[name].begin(); it != cities[name].end(); )
        {
            if(it2 == *it)
            {
                it = cities[name].erase(it);
                break;
            }
            else
                it++;
        }
        it1 = it2;
        it2 = it2->next;
        delete it1;
    }

//    for(auto it = cities.begin(); it != cities.end(); it++)
//    {
//        qDebug() << QString::fromStdString(it->first) << it->second.size();
//    }
    if(index != routes.end())
    {
        routes.erase(index);
    }
    if(isAutoExport == true)
        saveToFile(this->trainExportPath);
}

QString Routes::sellTicket(Ticket ticket)
{
    ticket.ticketID = this->ticketID++;
    tickets.push_back(ticket);
    vector<Route>::iterator RouteIt = this->findRoute(ticket.trainID.toStdString(), ticket.departureTime.toStdString());
    int index = 0;
    for(Station *it = RouteIt->startingStation; it != NULL; it = it->next)
    {
        if(it->name == ticket.routes[index].toStdString())
        {
            it->seatsLeft -= ticket.number;
            index++;
        }
        if(index >= ticket.routes.size())
            break;
    }
    if(isAutoExport == true)
    {
        saveToFile(this->trainExportPath);
        ticketsSaveToFile(this->ticketExportPath);
    }
    return ticket.ticketID;
}

void Routes::refundTicket(int ticketID)
{
    for(auto it = this->tickets.begin(); it != this->tickets.end(); ++it)
    {
        if(it->ticketID == ticketID)
        {

            vector<Route>::iterator RouteIt = this->findRoute(it->trainID.toStdString(), it->departureTime.toStdString());
            int index = 0;
            for(Station *it2 = RouteIt->startingStation; it2 != NULL; it2 = it2->next)
            {
                if(it2->name == it->routes[index].toStdString())
                {
                    it2->seatsLeft += it->number;
                    index++;
                }
                if(index >= it->routes.size())
                    break;
            }
            tickets.erase(it);
            if(isAutoExport == true)
            {
                saveToFile(this->trainExportPath);
                ticketsSaveToFile(this->ticketExportPath);
            }
            return;
        }
    }
    qDebug() << "refundTicket() may be wrong";
}

QStringList Routes::bfs(QString startingStation, QString terminalStation)
{
    queue<Station*> q;
    map<Station*, Station*> parent;
    parent.clear();
    vector<Station*> res;
    QList<Station*> v;
    for(int i = 0; i < cities[startingStation.toStdString()].size(); ++i)
    {
        v << cities[startingStation.toStdString()][i];
        q.push(cities[startingStation.toStdString()][i]);
    }

    while(!q.empty())
    {
        Station* t = q.front();
        q.pop();
        if(t->name == terminalStation.toStdString())
        {
            res.push_back(t);
            break;
        }
        if(t->next == NULL)
            continue;
        for(int i = 0; i < cities[t->next->name].size(); ++i)
        {
            Station* r = cities[t->next->name][i];
            if(v.contains(r))
            {
                continue;
            }
            v << r;
            parent[r] = t;
            q.push(r);
        }
    }
    if(res.size() == 0)
    {
        return QStringList();
    }
    else
    {
        QString l, ls;
        for(int i = 0; ; ++i)
        {
            if(parent.count(res[i]))
                res.push_back(parent[res[i]]);
            else
                break;
        }
        for(int i = res.size() - 1; i >= 0; --i)
        {
            l += QString::fromStdString(res[i]->name);
            if(i)
                l += "->";
        }
        for(int i = res.size() - 1; i >= 1; --i) //终点站的trainID不需要
        {
            ls += QString::fromStdString(res[i]->trainID);
            if(i > 1)
                ls += "->";
        }
        QStringList result;
        result << l << ls;
        return result;
    }
}

QStringList Routes::bfs_depth(QString startingStation, QString terminalStation)
{
    map<Station*, Station*> parent;
    vector<Station*> res;
    for(int minDepth = 1; ; ++minDepth)
    {
        bool last = false;
        bool flag = false;
        int numLessThanMinDepth = 0;
        map<Station*, int> depth;
        queue<Station*> q;
        parent.clear();
        res.clear();
        QList<Station*> v;
        for(int i = 0; i < cities[startingStation.toStdString()].size(); ++i)
        {
            v << cities[startingStation.toStdString()][i];
            q.push(cities[startingStation.toStdString()][i]);
            depth[cities[startingStation.toStdString()][i]] = 1;
            numLessThanMinDepth++;
        }

        while(!q.empty())
        {
            Station* t = q.front();
            q.pop();
            if(depth[t] <= minDepth)
                numLessThanMinDepth--;
            if(depth[t] > minDepth)
            {
                flag = true;
                continue;
            }
            if(t->name == terminalStation.toStdString() && depth[t] == minDepth)
            {
                res.push_back(t);
                last = true;
                break;
            }
            if(t->next == NULL)
                continue;
            for(int i = 0; i < cities[t->next->name].size(); ++i)
            {
                Station* r = cities[t->next->name][i];
                if(v.contains(r))
                    continue;
                v << r;
                parent[r] = t;
                if(r->trainID == t->trainID)
                {
                    depth[r] = depth[t];
                    numLessThanMinDepth++;
                }
                else
                {
                    depth[r] = depth[t] + 1;
                    if(depth[r] <= minDepth)
                        numLessThanMinDepth++;
                }
                q.push(r);
            }
            if(numLessThanMinDepth == 0)
            {
                if(q.size() != 0)
                    flag = true;
                break;
            }
        }
        if(flag == false)
            break;
        if(last == true)
            break;
    }


    if(res.size() == 0)
    {
        return QStringList();
    }
    else
    {
        QString l, ls;
        for(int i = 0; ; ++i)
        {
            if(parent.count(res[i]))
                res.push_back(parent[res[i]]);
            else
                break;
        }
        for(int i = res.size() - 1; i >=0; --i)
        {
            l += QString::fromStdString(res[i]->name);
            if(i)
                l += "->";
        }
        for(int i = res.size() - 1; i >= 1; --i) //终点站的trainID不需要
        {
            ls += QString::fromStdString(res[i]->trainID);
            if(i > 1)
                ls += "->";
        }
        QStringList result;
        result << l << ls;
        return result;
    }
}

QStringList Routes::queryRoute(QString startingStation, QString terminalStation, int mode)
{
    if(startingStation == terminalStation)
    {   QStringList t;
        t << QString::fromLocal8Bit("起始站和终点站不可以相同");
        t << "";
        return t;
    }
    if(mode == 0)
        return this->bfs(startingStation, terminalStation);
    else if(mode == 1)
        return this->bfs_depth(startingStation, terminalStation);
    else
        return QStringList();

}
void Routes::setAutoExport(bool flag)
{
    this->isAutoExport = flag;
}

void Routes::setAutoImport(bool flag)
{
    this->isAutoImport = flag;
}

void Routes::setTrainExportPath(QString path)
{
    this->trainExportPath = path;
    if(isAutoExport == true)
        saveToFile(this->trainExportPath);
}

void Routes::setTrainImportPath(QString path)
{
    this->trainImportPath = path;
}

void Routes::setTicketExportPath(QString path)
{
    this->ticketExportPath = path;
    if(isAutoExport == true)
        ticketsSaveToFile(this->ticketExportPath);
}

void Routes::setTicketImportPath(QString path)
{
    this->ticketImportPaht = path;
}

//----------------------------------------------------------------//
//                                                                //
//        To understand recursion, see the top of this file       //
//                                                                //
//----------------------------------------------------------------//
