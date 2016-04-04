#include "tickets.h"
#include "routes.h"
#include "selldialog.h"
#include "ui_selldialog.h"
#include <QObject>

void Tickets::sellTicket(int index, Routes trainRoutes, QWidget *mainWindow)
{
    SellDialog *sellDialog = new SellDialog(mainWindow);
    sellDialog->ui->trainIDLabel->setText(trainRoutes.routes[index].trainID);
    sellDialog->ui->departureTimeLabel->setText(trainRoutes.routes[index].departureTime);
    for(Station *p = trainRoutes.routes[index].startingStation; p != NULL; p = p->next)
    {
        sellDialog->ui->startingStationBox->addItem(QString::fromStdString(p->name));
        sellDialog->ui->terminalStationBox->addItem(QString::fromStdString(p->name));
    }
    //sellDialog->ui->startingStationBox->currentText();
    sellDialog->ui->seatsLeftLabel->setText(QString::number(trainRoutes.routes[index].startingStation->seatsLeft));

}
