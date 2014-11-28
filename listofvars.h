#ifndef LISTOFVARS_H
#define LISTOFVARS_H

#include "modbusvar.h"
#include <QVector>
#include <QString>

// class for work with modbus variables' list

class ListOfVars
{
    QVector<modbusVar*> list;
public:
    ListOfVars();
    void addVar(modbusVar* ptr);
    QVector<QString> getListOfGroups(void);
    modbusVar* getVar(const QString &varName);
    QVector<modbusVar*> getVarsInGroup(const QString &groupName);
    bool deleteGroup(const QString &groupName);
    bool deleteVar(const QString &varName);
    QVector<modbusVar*> getRootVars(void);
    QVector<modbusVar*> getAllVars(void);
    QVector<QString> getTypes();
    QVector<modbusVar*> getVarsByType(const QString &vType);
    void deleteAllVars(void) {list.clear();}
};

#endif // LISTOFVARS_H
