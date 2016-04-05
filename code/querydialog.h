#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>
#include <QString>
namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = 0);
    ~QueryDialog();

private slots:
    void on_trainIDLine_textChanged(const QString &arg1);

    void on_startingStationLine_textChanged(const QString &arg1);

    void on_terminalStationLine_textChanged(const QString &arg1);

signals:
    void filterDataChanged(QString trainID, QString startingStation, QString terminalStation);

private:
    Ui::QueryDialog *ui;
};

#endif // QUERYDIALOG_H
