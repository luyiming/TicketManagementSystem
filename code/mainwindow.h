#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QStringList>
#include "routes.h"
#include <QTableWidgetItem>
#include "RouteQueryDialog.h"
#include <QProcess>
#include "ImportDialog.h"
#include "ui_ImportDialog.h"
#include "trainAMDialog.h"
#include "ui_trainAMDialog.h"
#include <QGroupBox>
#include <QVBoxLayout>
//#include <QPaintDevice>
//#include <QPaintEngine>
//#include <QPainter>

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

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
//    void paintEvent(QPaintEvent *)
//    {
//        QPainter painter(this);
//        painter.setPen(Qt::blue);
//        painter.setBrush(Qt::blue);
//        painter.drawRect(0, 0, 500, 500);
//    }

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

    void importDialog();
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

    void onlineLoadingDialog();
    void onlineLoadFile();
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
