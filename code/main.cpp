#include "mainwindow.h"
#include "logindialog.h"
#include <QObject>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainwindow;

    LoginDialog loginDialog;

    QObject::connect(&loginDialog,
                     &LoginDialog::sendVisitorMode,
                     &mainwindow,
                     &MainWindow::setVisitorMode);

    auto exit = loginDialog.exec();

    if(QDialog::Accepted == exit) {
        mainwindow.show();
        mainwindow.autoImport();
    }
    else if(QDialog::Rejected == exit)
        return 0;

    return app.exec();
}
