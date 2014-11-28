#include "epsonfont.h"
#include <QList>


EpsonFont::EpsonFont(const QString &fontName, int count, int sWidth, int sHeight):
    Font(fontName,count,sWidth,sHeight)
{
    createTable();
}

void EpsonFont::createTable()
{
    // transform symbol from cyrillic to latin unicode

    correctionTable[0x0410] = 0x0041;   // "A"
    correctionTable[0x0430] = 0x0061;   // "a"
    correctionTable[0x0412] = 0x0042;   // "B"
    correctionTable[0x0434] = 0x00E3;   // "д"
    correctionTable[0x0415] = 0x0045;   // "Е"
    correctionTable[0x0435] = 0x0065;   // "е"
    correctionTable[0x041A] = 0x004B;   // "К"
    correctionTable[0x041C] = 0x004D;   // "М"
    correctionTable[0x041D] = 0x0048;   // "Н"
    correctionTable[0x041E] = 0x004F;   // "О"
    correctionTable[0x043E] = 0x006F;   // "о"
    correctionTable[0x0420] = 0x0050;   // "Р"
    correctionTable[0x0440] = 0x0070;   // "р"
    correctionTable[0x0421] = 0x0043;   // "С"
    correctionTable[0x0441] = 0x0063;   // "с"
    correctionTable[0x0443] = 0x0079;   // "y"
    correctionTable[0x0422] = 0x0054;   // "Т"
    correctionTable[0x0425] = 0x0058;   // "Х"
    correctionTable[0x0445] = 0x0078;   // "х"
    correctionTable[0x042C] = 0x0062;   // "Ь"
    correctionTable[0x0429] = 0x00E2;   // "Щ"

    correctionTable[0x0053] = 0x0053;   // "S"
}

quint16 EpsonFont::correctionCode(quint16 input)
{
    for(int i=0;i<getCountOfSymbols();i++)
    {
        if(getSymbolCode(i)==input) return i;
    }
    for(int i=0;i<correctionTable.size();i++)
    {
        if(correctionTable.contains(input)) return correctionTable.value(input);
    }
    return 0;
}

quint16 EpsonFont::invertCorrection(quint16 input)
{
    QList<quint16> T = correctionTable.keys(input);
    if(T.count()) return T[0];
    return input;
}
