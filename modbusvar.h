#ifndef MODBUSVAR_H
#define MODBUSVAR_H

#include <QtGlobal>
#include <QString>

// Class describing modbus variable

typedef struct
{
    double k;  // multiply coefficient;
    double b;  // addition value
}correction;    // y=k*x+b (x is an input value, y is an outcome)

class modbusVar
{
    int memType;
    int plcAddr;
    int memAddr;
    correction coeff;
    quint16 value;
    QString comment;
    QString name;
    QString groupName;
    bool isEditable;
    bool isSign;
public:
    enum {COIL,DISCR_INPUT,HOLD_REG,INP_REG,DOUBLE_HOLD_REG,DOUBLE_INP_REG,UNKNOWN};
    modbusVar(int varType = HOLD_REG);
    void setPlcAddress(int value);
    int getPlcAddress(void) const;
    void setMemAddress(int value);
    int getMemAddress(void) const;
    void setData(quint16 value);
    quint16 getData(void) const;
    double getCorrectedData(void) const;
    void setCorrection(double k, double b);
    double getAddion(void);
    double getMultiplier(void);
    void setName(const QString &name) {this->name = name;}
    QString getName(void) {return name;}
    void setComment(const QString &comment) {this->comment = comment;}
    QString getComment(void) {return comment;}
    void setGroupName(const QString &name) {groupName = name;}
    QString getGroupName(void) {return groupName;}
    bool isItInGroup(void) {if(groupName.length()==0) return false;return true;}
    QString getType(void);
    int getTypeCode(void);
    void setType(const QString &newType);
    bool isItEditable(void) {return isEditable;}
    void setEditable(bool value) {isEditable = value;}
    bool isVarSign(void) {return isSign;}
    void setSign(bool value) {isSign = value;}
    QVector<QString> getTypes(void);
};

#endif // MODBUSVAR_H
