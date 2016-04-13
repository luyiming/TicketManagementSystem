#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QTextStream>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/train"));
    ui->userNameLine->setFocus();
    ui->pwdLine->setEchoMode(QLineEdit::Password);

    this->usrname = "admin";
    this->password = "admin";
    QFile configFile("train.conf");
    bool flag = true;
    if(configFile.exists())
    {
        if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::information(this, QString::fromLocal8Bit("信息"), QString::fromLocal8Bit("配置文件导入出错"));
            flag = false;
        }
        if(flag == true)
        {
            QTextStream in(&configFile);
            this->usrname = in.readLine();
            this->password = in.readLine();
            configFile.close();
        }
    }
    connect(ui->exitButton, &QPushButton::clicked, this, &LoginDialog::reject);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    QString name = ui->userNameLine->text();
    QString pwd = ui->pwdLine->text();
    if(name == this->usrname && pwd == this->password)
    {
        emit sendVisitorMode(false);
        accept();
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("验证失败"), QString::fromLocal8Bit("用户名或密码错误"));
        ui->userNameLine->setFocus();
        return;
    }

}

void LoginDialog::on_visitorButton_clicked()
{
    emit sendVisitorMode(true);
    accept();
}
