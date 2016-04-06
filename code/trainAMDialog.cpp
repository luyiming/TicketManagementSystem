#include "trainAMDialog.h"
#include "ui_trainAMDialog.h"
#include <QMessageBox>
#include <QDebug>
trainAMDialog::trainAMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::trainAMDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/train"));
}

void trainAMDialog::setTrainData(QStringList trainData_)
{
    if(trainData_.size() >= 6)
    {
        ui->trainIDLine->setText(trainData_[0]);
        ui->departureTimeLine->setText(trainData_[1]);
        ui->drivingTimeLine->setText(trainData_[2]);
        ui->priceLine->setText(trainData_[3]);
        ui->maxPassengersLine->setText(trainData_[4]);
        ui->routesText->setPlainText(trainData_[5]);

        this->preTrainID = trainData_[0];
        this->preDepatureTime = trainData_[1];
    }
}


trainAMDialog::~trainAMDialog()
{
    delete ui;
}

void trainAMDialog::on_pushButton_clicked()
{
    trainData.clear();
    trainData << ui->trainIDLine->text();
    trainData << ui->departureTimeLine->text();
    trainData << ui->drivingTimeLine->text();
    trainData << ui->priceLine->text();
    trainData << ui->maxPassengersLine->text();
    trainData << ui->routesText->toPlainText().split("|", QString::SkipEmptyParts);

    if(trainData.count("") > 0)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("更改失败"), QString::fromLocal8Bit("火车信息不完整"));
        trainData.clear();
        return;
    }
//    if(preTrainID == trainData[0]  && preDepatureTime == trainData[1])
//    {
//        this->close();
//        return;
//    }
    if(this->preTrainID == "" && this->preDepatureTime == "")
        emit addValueChanged(trainData);
    else
        emit modifyValueChanged(preTrainID, preDepatureTime, trainData);
    this->close();
}

void trainAMDialog::on_quitButton_clicked()
{
    this->close();
}
