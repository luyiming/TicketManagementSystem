#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = 0);
    ~ImportDialog();

signals:
    void loadFromFile();
    void onlineLoad();

private slots:
    void on_fileButton_clicked();

    void on_onlineButton_clicked();

private:
    Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
