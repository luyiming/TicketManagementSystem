#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void setDefaultInfo(QStringList info);
private slots:
    void on_changeFilePathButton_clicked();

    void on_trainImportButton_clicked();

    void on_changeUserButton_clicked();

    void on_userBackButton_clicked();

    void on_confirmButton_clicked();

    void on_ticketExportButton_clicked();

    void on_ticketImportButton_clicked();

    void on_quitButton_clicked();

signals:
    void configChanged(QStringList config);

private:
    Ui::ConfigDialog *ui;
    QStringList configInfo;
};

#endif // CONFIGDIALOG_H
