#include "selldialog.h"
#include "ui_selldialog.h"

SellDialog::SellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellDialog)
{
    ui->setupUi(this);
}

SellDialog::~SellDialog()
{
    delete ui;
}

void SellDialog::on_pushButton_2_clicked()
{
    this->close();
}
