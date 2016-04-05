#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

private slots:
    void on_trainButton_clicked();

    void on_ticketButton_clicked();

signals:
    void trainExport();
    void ticketExport();
private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
