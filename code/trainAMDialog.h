#ifndef TRAINAMDIALOG_H
#define TRAINAMDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include "routes.h"

namespace Ui {
class trainAMDialog;
}

class trainAMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit trainAMDialog(QWidget *parent = 0);
    void setTrainData(QStringList trainData_);
    ~trainAMDialog();

signals:
    void modifyValueChanged(QString preTrainID, QString preDepatureTime, QStringList tranData);
    void addValueChanged(QStringList tranData);
private slots:
    void on_pushButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::trainAMDialog *ui;
    QStringList trainData;
    QString preTrainID;
    QString preDepatureTime;
};

#endif // TRAINAMDIALOG_H
