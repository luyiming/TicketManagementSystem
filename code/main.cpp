#include "mainwindow.h"
#include "logindialog.h"
#include <QObject>
#include <QPushButton>
#include <QApplication>
#include <QDialog>
#include <QDebug>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QStringList>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainwindow;

    LoginDialog loginDialog;

    QObject::connect(&loginDialog, &LoginDialog::visitorMode, &mainwindow, &MainWindow::setVisitorMode);

    auto exit = loginDialog.exec();

    if(QDialog::Accepted == exit)
    {
        mainwindow.show();
        mainwindow.autoImport();
    }
    else if(QDialog::Rejected == exit)
        return 0;

    return a.exec();
}
