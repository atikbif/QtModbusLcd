#ifndef DISPLAY_H
#define DISPLAY_H

#include "lineofsymbols.h"
#include <QVector>
#include "displaydata.h"

class Display
{
    // current coordinates of cursor
    unsigned char xPos; // number of cell in line
    unsigned char yPos; // number of line
    DisplayData *lcdData;
    QVector<int> currentLines;
    bool repl;

public:
    Display(DisplayData *lcdData);

    LineOfSymbols* getCurrentLine(unsigned char rowNum);


    // change current coordinates
    void posUp(void);
    void posDown(void);
    void posLeft(void);
    void posRight(void);
    void setReplaceMode(bool value);
    bool getReplaceMode(void);
    void toggleReplaceMode(void);
    void setPos(unsigned char x, unsigned char y);
    unsigned char getCountOfRows(void);
    unsigned char getX(void){return xPos;}
    unsigned char getY(void){return yPos;}
    quint16 getCurrentLineNumber(unsigned char rowNum);
    DisplayData* getData(void) {return lcdData;}
    void setLineNumber(int rowNum, int lineNum);
};

#endif // DISPLAY_H
