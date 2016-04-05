#ifndef SELLDIALOG_H
#define SELLDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include "routes.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
namespace Ui {
class SellDialog;
}

class SellDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SellDialog(QWidget *parent = 0);
    ~SellDialog();

void setTicketData(QString trainID, QString departureTime, QString price, QStringList routes);

private slots:
    void on_pushButton_2_clicked();
    void setTotalPrice(/*QString price*/);
    void setTerminalStation();
    void setSeatsLeft();
    void sellTicket();
signals:
    void buyTicket(Ticket ticket);
private:
    Ui::SellDialog *ui;
    QStringList routes;
    QStringList seats;
    QString trainID;
    QString departureTime;
};

#endif // SELLDIALOG_H
