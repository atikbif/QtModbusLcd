#include "font.h"

Font::Font(const QString &fontName, int count, int sWidth, int sHeight)
{
    name = fontName;
    if(sWidth>0) symbolWidth = sWidth;
    if(sHeight>0) symbolHeight = sHeight;
    if(count>0) countOfSymbols = count;
    for(int i=0;i<countOfSymbols;i++)
    {
        QBitArray* symbImage = new QBitArray(symbolHeight * symbolWidth);
        symbolImages += symbImage;
        uc += 0;
    }
}

Font::~Font()
{
    for(int i=0;i<countOfSymbols;i++)
    {
        delete symbolImages.at(0);
    }
}

const QBitArray Font::getSymbolImage(int i) const
{
    if(i<countOfSymbols)
    {
        return *symbolImages[i];
    }
    return QBitArray();
}

quint16 Font::getSymbolCode(int i) const
{
    if(i<countOfSymbols)
    {
        return uc.at(i);
    }
    return 0;
}

void Font::setSymbolImage(int i, const QBitArray &image)
{
    if(i<countOfSymbols)
    {
        int countOfPixels = symbolHeight * symbolWidth;
        if(image.count()>=countOfPixels)
        {
            for(int j=0;j<countOfPixels;j++)
            {
                symbolImages[i]->setBit(j,image.at(j));
            }
        }
    }
}

void Font::setSymbolCode(int i, quint16 code)
{
    if(i<countOfSymbols)
    {
        uc[i] = code;
    }
}

quint16 Font::correctionCode(quint16 input)
{
    for(int i=0;i<countOfSymbols;i++)
    {
        if(uc.at(i)==input) return input;
    }
    return 0x00;    // unknown symbol
}

int Font::getSymbolWidth() const
{
    return symbolWidth;
}

int Font::getSymbolHeight() const
{
    return symbolHeight;
}

int Font::getCountOfSymbols() const
{
    return symbolImages.count();
}
