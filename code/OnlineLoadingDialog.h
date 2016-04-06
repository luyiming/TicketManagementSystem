#ifndef ONLINELOADINGDIALOG_H
#define ONLINELOADINGDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QString>

namespace Ui {
class OnlineLoadingDialog;
}

class OnlineLoadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OnlineLoadingDialog(QWidget *parent = 0);
    ~OnlineLoadingDialog();

signals:
    void beginLoadFile();

private slots:
    void on_startButton_clicked();
    void readOutput();
    void on_clearButton_clicked();
    void readFile(int exitCode, QProcess::ExitStatus exitStatus);
    void on_loadButton_clicked();

private:
    Ui::OnlineLoadingDialog *ui;
    QProcess *myProcess;
    QString output;
};

#endif // ONLINELOADINGDIALOG_H
