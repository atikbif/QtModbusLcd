#include "imageofvariable.h"
#include <QRegularExpression>

ImageOfVariable::ImageOfVariable(): maxCountOfCells(10), position(0)
{
    pattern = "";
}

void ImageOfVariable::setFormat(const QString &inputString)
{
    QRegularExpression re("[.,]");
    QStringList pattern = inputString.split(re);
    if(pattern.count()>1)
    {
        CellsBeforeDot = pattern[0].length();
        CellsAfterDot = pattern[1].length();
    }
    else
    {
        CellsAfterDot = 0;
        CellsBeforeDot = inputString.length();
    }

    // check limit of length
    while(getLength() > maxCountOfCells)
    {
        if(CellsAfterDot) CellsAfterDot--;
        else if(CellsBeforeDot) CellsBeforeDot--;
        if((CellsAfterDot==0)&&(CellsBeforeDot==0)) break;
    }
    int i=0;
    while(i<CellsBeforeDot) {i++;this->pattern+="#";}
    if(CellsAfterDot)
    {
        this->pattern+=".";
        i=0;
        while(i<CellsAfterDot)
        {
            i++;
            this->pattern+="#";
        }
    }
}

void ImageOfVariable::setPosition(unsigned char pos)
{
    position = pos;
}

unsigned char ImageOfVariable::getPosition() const
{
    return position;
}

unsigned char ImageOfVariable::getCountOfCellsBeforeDot() const
{
    return CellsBeforeDot;
}

unsigned char ImageOfVariable::getCountOfCellsAfterDot() const
{
    return CellsAfterDot;
}

unsigned char ImageOfVariable::getLength()
{
    return (CellsAfterDot==0 ? CellsBeforeDot: CellsBeforeDot + CellsAfterDot + 1);
}

QString ImageOfVariable::getFormat()
{
    return pattern;
}
