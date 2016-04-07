#include "OnlineLoadingDialog.h"
#include "ui_OnlineLoadingDialog.h"
#include <QString>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>
#include <QFile>
#include <QStringList>
#include <QDate>

OnlineLoadingDialog::OnlineLoadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OnlineLoadingDialog)
{
    ui->setupUi(this);
    ui->spinBox->setRange(1, 20);
    ui->spinBox->setValue(8);
}

OnlineLoadingDialog::~OnlineLoadingDialog()
{
    delete ui;
}

void OnlineLoadingDialog::on_startButton_clicked()
{
    myProcess = new QProcess(this);
    QString program = QString("python.exe");
    QString numOfThreads = QString::number(ui->spinBox->value());
    QString city;
    if(ui->cityBox->currentIndex() == 0)
        city = "liyang";
    else
        city = "NanJing";
    QStringList parameters = QStringList() << "12306.py" << "jiangsu" << city << "train.cache" << numOfThreads;
    myProcess->start(program, parameters);
    connect(myProcess, SIGNAL(readyRead()), this, SLOT(readOutput()));
    connect(myProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &OnlineLoadingDialog::readFile);
}

void OnlineLoadingDialog::readOutput()
{
     output += QString::fromLocal8Bit(myProcess->readAll());
     ui->textEdit->setText(output);
     QScrollBar *sb = ui->textEdit->verticalScrollBar();
     sb->setValue(sb->maximum());
}


void OnlineLoadingDialog::readFile(int exitCode, QProcess::ExitStatus exitStatus)
{
    QMessageBox::information(this, QString::fromLocal8Bit("完成"), QString::fromLocal8Bit("下载完成"));
}

void OnlineLoadingDialog::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

void OnlineLoadingDialog::on_loadButton_clicked()
{
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");

    QFile file("train.cache");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("打开文件失败"));
        return;
    }

    QFile file3("train.data");
    if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("打开文件失败"));
        return;
    }
    QTextStream fout(&file3);
    fout << QString::fromLocal8Bit("班次号       发车时间     行车时间  票价  额定载客量          路线\n");
    fout << QString("-------------------------------------------------------------------------------------\n");
    file3.close();

    QTextStream in(&file);
    in.setCodec("UTF-8");
    in.readLine();
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(in.atEnd())
            break;
        if(line.isEmpty())
            continue;
        QStringList list = line.split(" ", QString::SkipEmptyParts);
        bool priceFlag = false;
        QString price;
        QString routes;
        QString departureTime = currentDate + " ";
        QString drivingTime;
        QString trainID;
        int index = 1;
        QStringList trainData;
        for(int i = 0; i < list.size(); ++i)
        {
            QString s = list.at(i);
            if(s.contains("http://qq.ip138.com/train/"))
            {
                s.remove(0, sizeof("http://qq.ip138.com/train"));
                trainID = s.split(".htm").at(0);
                continue;
            }

            if(s.contains(QString::fromLocal8Bit("全程耗时")))
            {
                QString tmp;
                for(int k = s.size() - 5; k < s.size(); ++k)
                    tmp += s.at(k);
                QStringList tmp2 = tmp.split(":");
                if(tmp2.at(0).at(0) == "0")
                {
                    QString temp = tmp2.at(0);
                    temp.remove(0, 1);
                    tmp2.replace(0, temp);
                }

                if(tmp2.at(1).toInt() == 0)
                    drivingTime = tmp2.at(0) + "h";
                else
                {
                    QString mid = QString::number(tmp2.at(1).toDouble() / 60.0).remove(0, 1);
                    mid.resize(2);
                    drivingTime = tmp2.at(0) + mid + "h";
                }
                continue;
            }

            if(s.contains(QString::fromLocal8Bit("一等座")) || s.contains(QString::fromLocal8Bit("二等座")) || s.contains(QString::fromLocal8Bit("硬座")) || s.contains(QString::fromLocal8Bit("硬卧")))
            {
                if(priceFlag == false)
                {
                    priceFlag = true;
                    QStringList tmp = s.split(":");
                    price = tmp.at(tmp.size() - 1);

                    price.remove(price.size() - 1, 1);
//                    qDebug() << price;
//                    qDebug() << s;
                }
                continue;
            }
            if(priceFlag == true)
            {
                if(s == QString::number(index))
                {
                    if(index == 1)
                        departureTime += list.at(i + 2); //departure time

                    if(index != 1)
                        routes += "|";
                    routes += list.at(i + 1); //city

                    index++;
                }
            }
        }

        trainData << trainID << departureTime << drivingTime << price << "150" << routes;

        QFile file2("train.data");
        if (!file2.open(QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("打开文件失败"));
            return;
        }
        QTextStream out(&file2);
        for(int j = 0; j < trainData.size(); ++j)
        {
            if(j != 0)
            {
                out << " ";
            }
            out << trainData.at(j);
        }
        out << endl;
        file2.close();

    }
    file.close();

    emit beginLoadFile();
    this->close();
}
