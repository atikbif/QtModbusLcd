#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T10:45:30
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = panelData
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    font.cpp \
    epsonfont.cpp \
    modbusvar.cpp \
    serialchannel.cpp \
    lineofsymbols.cpp \
    imageofvariable.cpp \
    display.cpp \
    settings.cpp \
    lcdwidget.cpp \
    displaydata.cpp \
    varconfigdialog.cpp \
    listofvars.cpp \
    vareditdialog.cpp \
    dialogvarchoose.cpp \
    dialogcommonsettings.cpp \
    serialcommunication.cpp \
    communicationdialog.cpp \
    listofrequests.cpp \
    requestscreator.cpp

HEADERS  += mainwindow.h \
    font.h \
    epsonfont.h \
    modbusvar.h \
    serialchannel.h \
    lineofsymbols.h \
    imageofvariable.h \
    display.h \
    settings.h \
    lcdwidget.h \
    displaydata.h \
    varconfigdialog.h \
    listofvars.h \
    vareditdialog.h \
    dialogvarchoose.h \
    dialogcommonsettings.h \
    serialcommunication.h \
    communicationdialog.h \
    listofrequests.h \
    requestscreator.h

FORMS    += mainwindow.ui \
    varconfigdialog.ui \
    vareditdialog.ui \
    dialogvarchoose.ui \
    dialogcommonsettings.ui \
    communicationdialog.ui

RESOURCES += \
    res.qrc

OTHER_FILES +=
