#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    ui->trainExportLine->setReadOnly(true);
    ui->trainImportLine->setReadOnly(true);
    ui->ticketExportLine->setReadOnly(true);
    ui->ticketImportLine->setReadOnly(true);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}


void ConfigDialog::on_changeUserButton_clicked()
{
    if(this->configInfo.at(0) == ui->usernameLine->text() && this->configInfo.at(1) == ui->passwordLine->text())
    {
        this->close();
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this, QString::fromLocal8Bit("修改确认"), QString::fromLocal8Bit("请确认修改:\n用户名:%1,密码:%2   ").arg(ui->usernameLine->text(), ui->passwordLine->text())))
    {
        this->configInfo.replace(0, ui->usernameLine->text());
        this->configInfo.replace(1, ui->passwordLine->text());
        emit configChanged(this->configInfo);
    }
    else
        this->setDefaultInfo(this->configInfo);
    this->close();
    return;
}

void ConfigDialog::on_userBackButton_clicked()
{
    ui->usernameLine->setText(configInfo.at(0));
    ui->passwordLine->setText(configInfo.at(1));
}

void ConfigDialog::setDefaultInfo(QStringList info)
{
    this->configInfo.clear();
    this->configInfo = info;
    ui->usernameLine->setText(info.at(0));
    ui->passwordLine->setText(info.at(1));
    ui->trainExportLine->setText(info.at(2));
    ui->ticketExportLine->setText(info.at(3));
    ui->trainImportLine->setText(info.at(4));
    ui->ticketImportLine->setText(info.at(5));
    if(info.at(6) == "true")
        ui->exportCheckBox->setChecked(true);
    else
        ui->exportCheckBox->setChecked(false);
    if(info.at(7) == "true")
        ui->importCheckBox->setChecked(true);
    else
        ui->importCheckBox->setChecked(false);
}


void ConfigDialog::on_confirmButton_clicked()
{
    if (this->configInfo.at(2) == ui->trainExportLine->text() &&
        this->configInfo.at(3) == ui->ticketExportLine->text()&&
        this->configInfo.at(4) == ui->trainImportLine->text() &&
        this->configInfo.at(5) == ui->ticketImportLine->text()&&
        this->configInfo.at(6) == (ui->exportCheckBox->isChecked() ? "true" : "false")&&
        this->configInfo.at(7) == (ui->importCheckBox->isChecked() ? "true" : "false"))
    {
        this->close();
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this, QString::fromLocal8Bit("修改确认"), QString::fromLocal8Bit("确认修改吗?")))
    {
        this->configInfo.replace(2, ui->trainExportLine->text());
        this->configInfo.replace(3, ui->ticketExportLine->text());
        this->configInfo.replace(4, ui->trainImportLine->text());
        this->configInfo.replace(5, ui->ticketImportLine->text());
        if(ui->exportCheckBox->isChecked())
            this->configInfo.replace(6, "true");
        else
            this->configInfo.replace(6, "false");
        if(ui->importCheckBox->isChecked())
            this->configInfo.replace(7, "true");
        else
            this->configInfo.replace(7, "false");
        emit configChanged(this->configInfo);
    }
    else
        this->setDefaultInfo(this->configInfo);
    this->close();
    return;
}

void ConfigDialog::on_changeFilePathButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromLocal8Bit("请选择导出文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
        ui->trainExportLine->setText(path);
}

void ConfigDialog::on_ticketExportButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromLocal8Bit("请选择导出文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
        ui->ticketExportLine->setText(path);
}

void ConfigDialog::on_trainImportButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromLocal8Bit("请选择导入文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
        ui->trainImportLine->setText(path);
}

void ConfigDialog::on_ticketImportButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString::fromLocal8Bit("请选择导入文件"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty())
        ui->ticketImportLine->setText(path);
}

void ConfigDialog::on_quitButton_clicked()
{
//    ui->usernameLine->setText(this->configInfo.at(0));
//    ui->passwordLine->setText(this->configInfo.at(1));
//    ui->trainExportLine->setText(this->configInfo.at(2));
//    ui->ticketExportLine->setText(this->configInfo.at(3));
//    ui->trainImportLine->setText(this->configInfo.at(4));
//    ui->ticketImportLine->setText(this->configInfo.at(5));
//    if(this->configInfo.at(6) == "true")
//        ui->exportCheckBox->setChecked(true);
//    else
//        ui->exportCheckBox->setChecked(false);
//    if(this->configInfo.at(7) == "true")
//        ui->importCheckBox->setChecked(true);
//    else
//        ui->importCheckBox->setChecked(false);
    this->close();
}
