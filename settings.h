#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings.h"
#include "displaydata.h"
#include "epsonfont.h"
#include "listofvars.h"
#include <QFile>
#include <QString>
#include <QStringList>

// class for read and write settings of program

class Settings
{
    QStringList strVars;
    QString portName;
    QString portBaud;
public:
    Settings();
    DisplayData* readDisplay(void);
    EpsonFont *readFont(void);
    ListOfVars* readVars(void);
    bool saveData(DisplayData* ptr,ListOfVars* list);
    bool openData(DisplayData* ptr,ListOfVars* list);
    QString getPortName(void) {return portName;}
    QString getPortBaud(void) {return portBaud;}
    QStringList getStringVars(void) {return strVars;}
    void setPortName(const QString &pName) {portName = pName;}
    void setPortBaud(const QString &pBaud) {portBaud = pBaud;}
    void setStrVars(const QStringList &sVars) {strVars = sVars;}
};

#endif // SETTINGS_H
