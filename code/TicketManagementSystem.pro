#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T23:04:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TicketManagementSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    importdialog.cpp \
    routes.cpp \
    magalgorithm.cpp \
    tickets.cpp \
    selldialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    importdialog.h \
    routes.h \
    tickets.h \
    magalgorithm.h \
    selldialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    importdialog.ui \
    selldialog.ui
