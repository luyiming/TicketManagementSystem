#ifndef ROUTES_H
#define ROUTES_H

#include <QObject>
#include <string>
#include <vector>
#include <map>
#include <QTableWidget>
#include <QWidget>

using namespace std;

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

class Routes : public QObject
{
    Q_OBJECT

private:
    map<string, vector<Station*> > cities; //从城市名到列车的关联数组
    vector<Route> routes;   //所有路线
public:
    Routes()
    {
        routes.clear();
        cities.clear();
    }



    int findRoute(string trainID, string departureTime);    //查找路线
    void deleteRoute(string trainID, string departureTime);
    void modifyRoute();
    void sort();

    void createRoute(QString &data);
    void refreshTrainInfo(QTableWidget* &trainTable);
    void loadFromFile(QWidget* mainWindow, QTableWidget* &trainTable);
    void saveToFile(QWidget* mainWindow);
};

#endif // ROUTES_H
