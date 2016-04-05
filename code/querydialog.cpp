#include "querydialog.h"
#include "ui_querydialog.h"

QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/train"));
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

void QueryDialog::on_trainIDLine_textChanged(const QString &arg1)
{
    QString trainID = ui->trainIDLine->text();
    QString startingStation = ui->startingStationLine->text();
    QString terminalStation = ui->terminalStationLine->text();
    emit filterDataChanged(trainID, startingStation, terminalStation);
}

void QueryDialog::on_startingStationLine_textChanged(const QString &arg1)
{
    QString trainID = ui->trainIDLine->text();
    QString startingStation = ui->startingStationLine->text();
    QString terminalStation = ui->terminalStationLine->text();
    emit filterDataChanged(trainID, startingStation, terminalStation);
}

void QueryDialog::on_terminalStationLine_textChanged(const QString &arg1)
{
    QString trainID = ui->trainIDLine->text();
    QString startingStation = ui->startingStationLine->text();
    QString terminalStation = ui->terminalStationLine->text();
    emit filterDataChanged(trainID, startingStation, terminalStation);
}
