#ifndef ROUTES_H
#define ROUTES_H

#include <QObject>
#include <string>
#include <vector>
#include <map>
#include <QTableWidget>
#include <QWidget>
#include <QStringList>

struct Ticket
{
    int ticketID;
    int number;
    int price;
    QString trainID;
    QString departureTime;
    QStringList routes;
    QString name;
    Ticket() : ticketID(-1), number(-1), price(-1){}
};

struct Station
{
    Station():prev(NULL),next(NULL){}
    std::string name;
    int seatsLeft;
    std::string trainID;
    Station *prev;      //previous station
    Station *next;      //next station

};

struct Route
{
    Route():startingStation(NULL),terminalStation(NULL){}
    std::string trainID;
    std::string departureTime;
    std::string drivingTime;
    int maxPassengers;
    int price;
    Station *startingStation;
    Station *terminalStation;
};

//-------------------------------------------------------------------
class Routes : public QObject
{
    Q_OBJECT

private:
    std::map<std::string, std::vector<Station*> > cities;
    std::vector<Route> routes;
    std::vector<Ticket> tickets;
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

    //TODO: remove
    std::vector<Route>::iterator findRoute(std::string trainID, std::string departureTime);
    bool isTrainExist(std::string trainID, std::string departureTime);

    //----------------* route *-----------------
    void deleteRoute(QString trainID, QString departureTime);
    bool modifyRoute(QString preTrainID, QString preDepatureTime, QStringList trainData);
    bool createRoute(QStringList trainData);
    QStringList queryRoute(QString startingStation, QString terminalStation, int mode);
    QStringList bfs(QString startingStation, QString terminalStation);
    QStringList bfs_depth(QString startingStation, QString terminalStation);
    //---------------* ticket *-----------------
    QString sellTicket(Ticket ticket);
    void refundTicket(int ticketID);
    //------------* table & file *--------------
    void refreshTrainInfo(QTableWidget* &trainTable);
    void refreshTicketInfo(QTableWidget* &ticketTable);
    bool loadFromFile(QString path, int &importNum, int &skipNum, QString &skipLine);
    bool saveToFile(QString path);   
    bool ticketsSaveToFile(QString path);
    QStringList search(QString startingStation, QString terminalStation);
    void removeAll();
    //---------------* config *-----------------
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
            // Compare cell data as integers for the 5, 6, 7 column.
            return text().toInt() < rhs.text().toInt();
        }
        return text() > rhs.text();
    }
};

#endif // ROUTES_H
