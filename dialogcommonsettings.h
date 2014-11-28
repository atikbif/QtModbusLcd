#ifndef DIALOGCOMMONSETTINGS_H
#define DIALOGCOMMONSETTINGS_H

#include <QDialog>
#include "listofvars.h"
#include <QString>

namespace Ui {
class DialogCommonSettings;
}

class DialogCommonSettings : public QDialog
{
    Q_OBJECT
    ListOfVars* list;

public:
    explicit DialogCommonSettings(QWidget *parent = 0);
    ~DialogCommonSettings();
    void setVars(ListOfVars* ptr) {list = ptr;}
    void updateDialog(void);
    QString getPortName(void);
    QString getPortBaud(void);
    QString getVarName(unsigned char strNum);
    void setPortName(const QString &pName);
    void setPortBaud(const QString &pBaud);
    void setStringsVar(unsigned char num, const QString &vName);
private:
    Ui::DialogCommonSettings *ui;
};

#endif // DIALOGCOMMONSETTINGS_H
