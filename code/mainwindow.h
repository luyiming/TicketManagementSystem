#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QStringList>
#include "routes.h"
#include <QTableWidgetItem>
#include "RouteQueryDialog.h"

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTableWidget *trainTable;
    QTableWidget *ticketTable;


private slots:
    void on_quitButton_clicked();
    void displayTime();

    void modifyTrainDialog();
    void addTrainDialog();
    void setTrainInfo(QStringList data);
    void modifyTrainInfo(QString preTrainID, QString preDepatureTime, QStringList data);

    void sellTicket(Ticket ticket);
    void sellTicketDialog();
    void refundTicketDialog();

    void loadFromFile();
    void exportDialog();
    void saveToFile();
    void ticketsSaveToFile();
    void refreshTable();
    void deleteRoute();

    void queryDialog();
    void filterTrainData(QString trainID, QString startingStation, QString terminalStation);
    void openRouteQueryDialog();

    void configDialog();
    void setConfig(QStringList config);

    void helpDialog();
    void removeAll();
public:
    void setVisitorMode(bool mode);
    void autoImport();
private:
    Ui::MainWindow *ui;
    QLabel *timeLabel;
    Routes trainRoutes;
    RouteQueryDialog *routeQueryDialog;
    QStringList configInfo;
    bool visitorMode;
};

#endif // MAINWINDOW_H
