#ifndef ROUTEQUERYDIALOG_H
#define ROUTEQUERYDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QStringList>
namespace Ui {
class RouteQueryDialog;
}

class RouteQueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RouteQueryDialog(QWidget *parent = 0);
    ~RouteQueryDialog();
    void setLine(QStringList line);

signals:
    void sendQuery(QString startingStation, QString terminalStation, int mode);

private slots:
    void on_queryButton_clicked();

    void on_clearButton_clicked();

    void on_queryButton_2_clicked();

private:
    Ui::RouteQueryDialog *ui;
};

#endif // ROUTEQUERYDIALOG_H
