#include "mainwindow.h"
#include "logindialog.h"
#include <QObject>
#include <QPushButton>
#include <QApplication>
#include <QDialog>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));

    LoginDialog logindlg;
    MainWindow mainwindow;
    mainwindow.setWindowTitle("Ticket Manegement System");
    auto exit = logindlg.exec();
    if(QDialog::Accepted == exit)
        mainwindow.show();
    else if(QDialog::Rejected == exit)
        return 0;

    return a.exec();
}
