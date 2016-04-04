#include "importdialog.h"
#include "ui_importdialog.h"
#include <QMessageBox>

ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);

}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::on_pushButton_clicked()
{
    trainData << ui->trainIDLine->text();
    trainData << ui->startingLine->text();
    trainData << ui->terminalLine->text();
    trainData << ui->departureTimeLine->text();
    trainData << ui->drivingTimeLine->text();
    trainData << ui->priceLine->text();
    trainData << ui->maxPassengersLine->text();
    trainData << ui->routeLine->text();
    if(trainData.count("") > 0)
    {
        QMessageBox::warning(this, tr("Question"), QString::fromLocal8Bit("火车信息不完整"));
        trainData.clear();
    }
    else if(trainData.count("") == 0)
        emit valueChanged(trainData);
    this->close();
}

void ImportDialog::on_quitButton_clicked()
{
    this->close();
}
