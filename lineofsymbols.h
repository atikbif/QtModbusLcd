#ifndef LINEOFSYMBOLS_H
#define LINEOFSYMBOLS_H

#include <QVector>
#include "imageofvariable.h"
#include "modbusvar.h"
#include "epsonfont.h"
#include <QString>

struct variable
{
    ImageOfVariable view;
    QString varName;
};

class LineOfSymbols
{
    unsigned char length;
    bool replaceMode;
    QVector<quint16> symbols;
    QVector<variable> vars;
public:
    LineOfSymbols();
    void setReplaceMode(bool value);
    bool isReplaceModeActive(void);
    bool insertSymbol(quint16 code, unsigned char pos);
    void deleteSymbol(unsigned char pos);
    bool backspace(unsigned char pos);
    bool insertVar(const QString &pattern, modbusVar* var, unsigned int pos);
    bool replaceVar(const QString &pattern, modbusVar* var, unsigned int pos);
    bool deleteVar(unsigned char pos);
    bool isVarHere(unsigned char pos);
    QString getVar(unsigned char pos);
    QString getPattern(unsigned char pos);
    bool isBeginOfVarHere(unsigned char pos);
    quint16 getSymbol(unsigned char pos);
    unsigned char getLength(void) {return length;}
    const QString getAsString(EpsonFont* font);
    int getCountOfVars(void);
};

#endif // LINEOFSYMBOLS_H
