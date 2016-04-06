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
    routes.cpp \
    selldialog.cpp \
    trainAMDialog.cpp \
    querydialog.cpp \
    RouteQueryDialog.cpp \
    ConfigDialog.cpp \
    ExportDialog.cpp \
    OnlineLoadingDialog.cpp \
    ImportDialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    routes.h \
    selldialog.h \
    trainAMDialog.h \
    querydialog.h \
    RouteQueryDialog.h \
    ConfigDialog.h \
    ExportDialog.h \
    OnlineLoadingDialog.h \
    ImportDialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    selldialog.ui \
    trainAMDialog.ui \
    querydialog.ui \
    RouteQueryDialog.ui \
    ConfigDialog.ui \
    ExportDialog.ui \
    OnlineLoadingDialog.ui \
    ImportDialog.ui

RESOURCES += \
    res.qrc
