#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QStringList>
#include "routes.h"
#include "tickets.h"

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
    void modifyData();
    void setTrainInfo(QStringList data); //will be removed
    void loadFromFile();
    void saveToFile();
    void refreshTable();
    //void removeItem();
    void sellTicket();

private:
    Ui::MainWindow *ui;
    QLabel *timeLabel;
    Routes trainRoutes;
    Tickets trainTickets;

};

#endif // MAINWINDOW_H
