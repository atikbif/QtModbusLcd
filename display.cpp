#include "display.h"

Display::Display(DisplayData *lcdData)
{
    this->lcdData = lcdData;
    xPos = 0;
    yPos = 0;
    currentLines.resize(lcdData->getRowCount());
    repl = false;
}

LineOfSymbols *Display::getCurrentLine(unsigned char rowNum)
{
    if(rowNum<lcdData->getRowCount())
    {
        if(currentLines[rowNum]<lcdData->getCountOfLinesInRow(rowNum))
        {
            return lcdData->getLine(rowNum,currentLines[rowNum]);
        }
    }
    return nullptr;
}

void Display::posUp()
{
    if(yPos>0) yPos--;
    LineOfSymbols* ptr = getCurrentLine(yPos);
    if(ptr != nullptr)
    {
        if(xPos>=ptr->getLength()) xPos = ptr->getLength()-1;
    }
}

void Display::posDown()
{
    if(yPos<lcdData->getRowCount()-1) yPos++;
    LineOfSymbols* ptr = getCurrentLine(yPos);
    if(ptr != nullptr)
    {
        if(xPos>=ptr->getLength()) xPos = ptr->getLength()-1;
    }
}

void Display::posLeft()
{
    if(xPos>0) xPos--;
}

void Display::posRight()
{
    LineOfSymbols* ptr =getCurrentLine(yPos);
    if(ptr != nullptr)
    {
        if(xPos<ptr->getLength()-1) xPos++;
    }
}

void Display::setReplaceMode(bool value)
{
    repl = value;
    for(int i=0;i<lcdData->getRowCount();i++)
    {
        for(int j=0;j<lcdData->getCountOfLinesInRow(i);j++)
            (lcdData->getLine(i,j))->setReplaceMode(value);
    }
}

bool Display::getReplaceMode()
{
    return repl;
}

void Display::toggleReplaceMode()
{
    setReplaceMode(!repl);
}

void Display::setPos(unsigned char x, unsigned char y)
{
    if(y<lcdData->getRowCount())
    {
        LineOfSymbols* ptr =getCurrentLine(y);
        if(ptr != nullptr)
        {
            if(x<ptr->getLength())
            {
                xPos = x;
                yPos = y;
            }
        }
    }
}

unsigned char Display::getCountOfRows()
{
    return lcdData->getRowCount();
}

quint16 Display::getCurrentLineNumber(unsigned char rowNum)
{
    if(rowNum<lcdData->getRowCount())
    {
        if(currentLines[rowNum]<lcdData->getCountOfLinesInRow(rowNum))
        {
            return currentLines[rowNum];
        }
    }
    return 0;
}

void Display::setLineNumber(int rowNum, int lineNum)
{
    if(rowNum<lcdData->getRowCount())
    {
        if(lineNum<lcdData->getCountOfLinesInRow(rowNum))
        {
            currentLines[rowNum] = lineNum;
        }
    }
}
