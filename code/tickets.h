#ifndef TICKETS_H
#define TICKETS_H

#include <QObject>
#include <string>
#include <vector>
#include "routes.h"
#include "selldialog.h"
#include "ui_selldialog.h"
using namespace std;

struct Ticket
{
    int orderID;
    Route *route;
    int number;
};

class Tickets : public QObject
{
    Q_OBJECT

private:
    vector<Ticket> tickets;
public:
    Tickets()
    {
        tickets.clear();
    }
    void Tickets::sellTicket(int index, Routes trainRoutes, QWidget *mainWindow);
    void refundTicket();
    void browse();
    void saveToFile();
};

#endif // TICKETS_H
