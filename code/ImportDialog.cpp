#include "ImportDialog.h"
#include "ui_ImportDialog.h"

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

void ImportDialog::on_fileButton_clicked()
{
    emit loadFromFile();
    this->close();
}

void ImportDialog::on_onlineButton_clicked()
{
    emit onlineLoad();
    this->close();
}
