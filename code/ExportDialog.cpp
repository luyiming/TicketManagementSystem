#include "ExportDialog.h"
#include "ui_ExportDialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::on_trainButton_clicked()
{
    emit trainExport();
    this->close();
}

void ExportDialog::on_ticketButton_clicked()
{
    emit ticketExport();
    this->close();
}
