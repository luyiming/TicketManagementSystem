#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

signals:
    void visitorMode(bool);

private slots:
    void on_loginButton_clicked();

    void on_visitorButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString usrname;
    QString password;
};

#endif // LOGINDIALOG_H
