#include "selldialog.h"
#include "ui_selldialog.h"
#include <QString>
#include <QStringList>
#include <string>
#include <sstream>
#include <QMessageBox>

SellDialog::SellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/train"));

    //QT4信号槽语法不接受编译时检查，很容易出错.....
    connect(ui->spinBox, SIGNAL(valueChanged(QString)), this, SLOT(setTotalPrice()));
    connect(ui->startingStationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTerminalStation()));
    connect(ui->startingStationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeatsLeft()));
    connect(ui->finishButton, &QPushButton::clicked, this, &SellDialog::sellTicket);
}

SellDialog::~SellDialog()
{
    delete ui;
}

void SellDialog::on_pushButton_2_clicked()
{
    this->close();
}

void SellDialog::setTicketData(QString trainID2, QString departureTime2, QString price, QStringList routesWithSeats)
{
    ui->departureTimeLabel->setText(departureTime2);
    this->departureTime = departureTime2;
    ui->trainIDLabel->setText(trainID2);
    this->trainID = trainID2;
    ui->priceLabel->setText(price);

    for(int i = 0; i < routesWithSeats.size(); ++i)
    {
        QStringList temp = routesWithSeats[i].split(QString(" "), QString::SkipEmptyParts);
//        for(int j = 0; j < temp.size(); ++j)
//            qDebug() << temp[j];
        if(temp.size() < 2)
            qDebug() << "something may be wrong in routes and tickets, see selldialog.cpp";
        routes.append(temp[0]);
        seats.append(temp[1]);
    }
//    QString::trimmed()//去空格
    ui->startingStationBox->addItems(routes);
    setTerminalStation();
    setSeatsLeft();
    setTotalPrice();
}

void SellDialog::setTotalPrice(/*QString price*/)
{
    int priceInt  = ui->priceLabel->text().toInt();
    int seatsBought = ui->spinBox->text().toInt();
    ui->sumLabel->setText(QString::number(priceInt * seatsBought));
}

void SellDialog::setTerminalStation()
{
    int startingStationIndex = ui->startingStationBox->currentIndex();
    ui->terminalStationBox->clear();
    for(int i = startingStationIndex + 1; i < routes.size(); ++i)
    {
        ui->terminalStationBox->addItem(routes[i]);
    }
}

void SellDialog::setSeatsLeft()
{
    //qDebug() << "setSeatsLeft";
    int seatsIndex = ui->startingStationBox->currentIndex();
    ui->seatsLeftLabel->setText(seats[seatsIndex]);
    int seatsMax = seats[seatsIndex].toInt();
    ui->spinBox->setRange(0, seatsMax);
}

void SellDialog::sellTicket()
{
    Ticket ticket;
    ticket.departureTime = this->departureTime;
    ticket.name = ui->nameLine->text();
    ticket.number = ui->spinBox->text().toInt();
    ticket.trainID = this->trainID;
    ticket.price = ui->priceLabel->text().toInt();

    int startIndex = ui->startingStationBox->currentIndex();
    int terminalIndex = ui->terminalStationBox->currentIndex() + 1; //注意terminalStationBox下标不是从routes[0]开始的
    for(int i = startIndex; i <= startIndex + terminalIndex; ++i)
    {
        ticket.routes.push_back(this->routes[i]);
        //qDebug() << i;
    }

    int seatsIndex = ui->startingStationBox->currentIndex();

    if(ticket.name == "")
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("购票失败"), QString::fromLocal8Bit("姓名不能为空"));
        return;
    }
    if(ticket.number > seats[seatsIndex].toInt())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("购票失败"), QString::fromLocal8Bit("购票数不能超过余票数"));
        return;
    }
    if(ticket.number == 0)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("购票失败"), QString::fromLocal8Bit("购票数不能为空"));
        return;

    }

    emit buyTicket(ticket);
    this->close();
}
