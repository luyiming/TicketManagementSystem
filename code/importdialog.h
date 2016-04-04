#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QString>

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
    void valueChanged(QStringList tranData);

private slots:
    void on_pushButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::ImportDialog *ui;
    QStringList trainData;
};

#endif // IMPORTDIALOG_H
