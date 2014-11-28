#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "lineofsymbols.h"
#include <QVector>

class DisplayData
{
    unsigned char row;
    QVector< QVector<LineOfSymbols> > lines;
public:
    DisplayData(unsigned char rowCount=4);
    unsigned char getRowCount() {return row;}
    LineOfSymbols* getLine(unsigned char row,unsigned char number);
    void insertLine(LineOfSymbols &line, unsigned char row, unsigned char count);
    void insertLineWithShift(LineOfSymbols &line, unsigned char row, unsigned char count);
    void deleteLine(unsigned char row, unsigned char number);
    quint16 getCountOfLinesInRow(unsigned char rowNum);
    void clear();
};

#endif // DISPLAYDATA_H
