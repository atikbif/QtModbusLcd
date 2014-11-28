#include "displaydata.h"

DisplayData::DisplayData(unsigned char rowCount): row(rowCount)
{
    lines.resize(rowCount);
}

LineOfSymbols *DisplayData::getLine(unsigned char row, unsigned char number)
{
    if(row < lines.count())
    {
        if(number < lines[row].count()) return &lines[row][number];
    }
    return nullptr;
}

void DisplayData::insertLine(LineOfSymbols& line, unsigned char row, unsigned char count)
{
    if(row>=lines.count()) lines.resize(row+1);
    if(count>=lines[row].count()) lines[row].resize(count+1);
    lines[row][count] = line;
}

void DisplayData::insertLineWithShift(LineOfSymbols &line, unsigned char row, unsigned char count)
{
    if(row>=lines.count()) lines.resize(row+1);
    //if(count>=lines[row].count()) lines[row].resize(count+1);
    lines[row].insert(count,line);
}

void DisplayData::deleteLine(unsigned char row, unsigned char number)
{
    if(row<lines.count())
    {
        if(number<lines[row].count()) lines[row].remove(number);
    }
}

quint16 DisplayData::getCountOfLinesInRow(unsigned char rowNum)
{
    if(rowNum<lines.count())
    {
        return lines[rowNum].count();
    }
    return 0;
}

void DisplayData::clear()
{
    for(int i=0;i<lines.count();i++) lines[i].clear();
}


