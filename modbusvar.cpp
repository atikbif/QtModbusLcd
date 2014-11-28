#include "modbusvar.h"
#include <QVector>

modbusVar::modbusVar(int varType)
{
    if((varType<UNKNOWN)&&(varType>=0)) memType = varType; else memType = UNKNOWN;
    plcAddr = 1;
    memAddr = 0;
    coeff.k = 1;
    coeff.b = 0;
    value = 0;
    isEditable = false;
    isSign = false;
}

void modbusVar::setPlcAddress(int value)
{
    if((value<256)&&(value>=0)) plcAddr = value; else plcAddr = 1;
}

int modbusVar::getPlcAddress() const
{
    return plcAddr;
}

void modbusVar::setMemAddress(int value)
{
    if(value>=0) memAddr = value;
}

int modbusVar::getMemAddress() const
{
    return memAddr;
}

void modbusVar::setData(quint16 value)
{
    this->value = value;
}

quint16 modbusVar::getData() const
{
    return value;
}

double modbusVar::getCorrectedData() const
{
    return (value*coeff.k+coeff.b);
}

void modbusVar::setCorrection(double k, double b)
{
    coeff.k = k;
    coeff.b = b;
}

double modbusVar::getAddion()
{
    return coeff.b;
}

double modbusVar::getMultiplier()
{
    return coeff.k;
}

QString modbusVar::getType()
{
    QString type;
    switch (memType) {
        case COIL: type = "COIL";break;
        case DISCR_INPUT: type = "DISCRETE INPUTS";break;
        case HOLD_REG: type = "HOLDING REGISTERS";break;
        case INP_REG:type = "INPUT REGISTERS"; break;
        case DOUBLE_HOLD_REG:type = "DOUBLE HOLDING REGISTERS"; break;
        case DOUBLE_INP_REG:type = "DOUBLE INPUT REGISTERS"; break;
        default: type = "UNKNOWN"; break;
    }
    return type;
}

int modbusVar::getTypeCode()
{
    return memType;
}

void modbusVar::setType(const QString &newType)
{
    if(newType=="COIL") memType=COIL;
    if(newType=="DISCRETE INPUTS") memType=DISCR_INPUT;
    if(newType=="HOLDING REGISTERS") memType=HOLD_REG;
    if(newType=="INPUT REGISTERS") memType=INP_REG;
    if(newType=="DOUBLE HOLDING REGISTERS") memType=DOUBLE_HOLD_REG;
    if(newType=="DOUBLE INPUT REGISTERS") memType=DOUBLE_INP_REG;
}

QVector<QString> modbusVar::getTypes()
{
    QVector<QString> types;
    types += "COIL";
    types += "DISCRETE INPUTS";
    types += "HOLDING REGISTERS";
    types += "INPUT REGISTERS";
    types += "DOUBLE HOLDING REGISTERS";
    types += "DOUBLE INPUT REGISTERS";
    return types;
}
