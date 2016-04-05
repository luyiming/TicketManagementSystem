#ifndef ROUTES_H
#define ROUTES_H

#include <QObject>
#include <string>
#include <vector>
#include <map>
#include <QTableWidget>
#include <QWidget>
#include <QStringList>

using namespace std;

struct Ticket
{
    int ticketID;
    QString trainID;
    QString departureTime;
    QStringList routes;
    QString name;
    int number;
    int price;
    Ticket() : ticketID(-1), number(-1), price(-1){}
};

struct Station
{
    Station():prev(NULL),next(NULL){}
    string name;    //站点名字
    int seatsLeft;  //剩余座位
    string trainID;    //火车班次
    Station *prev;  //上一站
    Station *next;  //下一站

};

struct Route
{
    Route():startingStation(NULL),terminalStation(NULL){}
    string trainID;    //火车班次
    string departureTime;//出发时间
    string drivingTime;    //行车时间
    int maxPassengers;  //额定载客数
    int price;  //票价
    Station *startingStation;//起点站
    Station *terminalStation;//终点站
};

//-------------------------------------------------------------------
class Routes : public QObject
{
    Q_OBJECT

private:
    map<string, vector<Station*> > cities; //从城市名到列车的关联数组
    vector<Route> routes;   //所有路线
    vector<Ticket> tickets; //车票
    int ticketID;

    QString trainExportPath;
    QString trainImportPath;
    QString ticketExportPath;
    QString ticketImportPaht;
    bool isAutoExport;
    bool isAutoImport;
public:
    Routes()
    {
        routes.clear();
        cities.clear();
        ticketID = 1000;
        isAutoExport = false;
        isAutoImport = false;
    }
    bool isTrainExist(string trainID, string departureTime);
    vector<Route>::iterator findRoute(string trainID, string departureTime);
    void deleteRoute(string trainID, string departureTime);
    bool modifyRoute(QString preTrainID, QString preDepatureTime, QStringList trainData);
    bool createRoute(QStringList trainData);
    void refreshTrainInfo(QTableWidget* &trainTable);
    void refreshTicketInfo(QTableWidget* &ticketTable);
    bool loadFromFile(QString path, int &importNum, int &skipNum, QString &skipLine);
    bool saveToFile(QString path);   
    bool ticketsSaveToFile(QString path);
    void removeAll();
    QStringList queryRoute(QString startingStation, QString terminalStation, int mode);
    QStringList bfs(QString startingStation, QString terminalStation);
    QStringList bfs_mindepth(QString startingStation, QString terminalStation);
    //---------------------------------------
    QString sellTicket(Ticket ticket);
    void refundTicket(int ticketID);
    //---------------------------------------
    void setTrainExportPath(QString path);
    void setTrainImportPath(QString path);
    void setTicketExportPath(QString path);
    void setTicketImportPath(QString path);
    void setAutoExport(bool flag);
    void setAutoImport(bool flag);
};

//customize TableItem
class TableItem : public QTableWidgetItem
{
public:
    TableItem(const QString & text) :
        QTableWidgetItem(text)
    {}

    TableItem(int num) :
        QTableWidgetItem(QString::number(num))
    {}

    bool operator < (const QTableWidgetItem &rhs) const
    {
        if (rhs.column() == 5 || rhs.column() == 6 || rhs.column() == 7)
        {
            // Compare cell data as integers for the 5,6,7 column.
            return text().toInt() < rhs.text().toInt();
        }
        return text() > rhs.text();
    }
};

#endif // ROUTES_H
