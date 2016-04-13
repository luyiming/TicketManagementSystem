#include "RouteQueryDialog.h"
#include "ui_RouteQueryDialog.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QDebug>

RouteQueryDialog::RouteQueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RouteQueryDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/train"));
}

RouteQueryDialog::~RouteQueryDialog()
{
    delete ui;
}

void RouteQueryDialog::setLine(QStringList line)
{
    ui->listWidget->clear();
    if(line.size() == 2)
    {
        ui->listWidget->addItem(line.at(0));
        ui->listWidget->addItem(line.at(1));
    }
    else
    {
        ui->listWidget->addItem(QString::fromLocal8Bit("未找到路线"));
    }
}

void RouteQueryDialog::on_clearButton_clicked()
{
    ui->listWidget->clear();
    ui->startingStationLine->clear();
    ui->terminalStationLine->clear();
    ui->startingStationLine->setFocus();
}

void RouteQueryDialog::on_queryButton_clicked()
{
    QString startingStation = ui->startingStationLine->text().trimmed();
    QString terminalStation = ui->terminalStationLine->text().trimmed();
    if(startingStation == "" || terminalStation == "")
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("查询失败"), QString::fromLocal8Bit("站点不能为空"));
        return;
    }
    else
    {
        emit sendQuery(startingStation, terminalStation, 0);
    }
}

void RouteQueryDialog::on_queryButton_2_clicked()
{
    QString startingStation = ui->startingStationLine->text().trimmed();
    QString terminalStation = ui->terminalStationLine->text().trimmed();
    if(startingStation == "" || terminalStation == "")
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("查询失败"), QString::fromLocal8Bit("站点不能为空"));
        return;
    }
    else
    {
        emit sendQuery(startingStation, terminalStation, 1);
    }
}
