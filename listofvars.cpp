#include "listofvars.h"

ListOfVars::ListOfVars()
{
}

void ListOfVars::addVar(modbusVar *ptr)
{
    list += ptr;
}

QVector<QString> ListOfVars::getListOfGroups()
{
    QVector<QString> names;
    foreach (modbusVar* var, list)
    {
        if(var!=nullptr)
        {
            QString grName = var->getGroupName();
            if(!grName.isEmpty())
            {
                if(!names.contains(grName)) names += grName;
            }
        }
    }
    return names;
}

modbusVar *ListOfVars::getVar(const QString &varName)
{
    foreach (modbusVar* var, list)
    {
        if(var->getName() == varName) return var;
    }
    return nullptr;
}

QVector<modbusVar *> ListOfVars::getVarsInGroup(const QString &groupName)
{
    QVector<modbusVar*> vars;
    foreach (modbusVar* var, list)
    {
        if(var->getGroupName() == groupName) vars += var;
    }
    return vars;
}

bool ListOfVars::deleteGroup(const QString &groupName)
{
    unsigned short i = 0;
    while(i<list.count())
    {
        if(list[i] != nullptr)
        {
            if(list[i]->getGroupName() == groupName) list.remove(i); else i++;
        }
    }
    return true;
}

bool ListOfVars::deleteVar(const QString &varName)
{
    unsigned short i = 0;
    while(i<list.count())
    {
        if(list[i] != nullptr)
        {
            if(list[i]->getName() == varName) list.remove(i); else i++;
        }
    }
    return true;
}

QVector<modbusVar *> ListOfVars::getRootVars()
{
    QVector<modbusVar*> rootVars;
    foreach(modbusVar* var, list)
    {
        if(var->getGroupName().isEmpty()) rootVars += var;
    }

    return rootVars;
}

QVector<modbusVar *> ListOfVars::getAllVars()
{
    return list;
}

QVector<QString> ListOfVars::getTypes()
{
    QVector<QString> names;
    foreach (modbusVar* var, list)
    {
        if(var!=nullptr)
        {
            QString vType = var->getType();
            if(!names.contains(vType)) names += vType;
        }
    }
    return names;
}

QVector<modbusVar *> ListOfVars::getVarsByType(const QString &vType)
{
    QVector<modbusVar*> vars;
    foreach (modbusVar* var, list)
    {
        if(var->getType() == vType) vars += var;
    }
    return vars;
}
