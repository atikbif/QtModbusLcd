#ifndef FONT_H
#define FONT_H

#include<QString>
#include <QVector>
#include <QBitArray>

// Base class for operation with lcd font

class Font
{
   QString name;
   int symbolWidth;
   int symbolHeight;
   QVector<QBitArray*> symbolImages;
   QVector<quint16> uc; // unicod value of symbol
   int countOfSymbols;
public:
    explicit Font(const QString &fontName, int count = 256, int sWidth = 5, int sHeight = 7);
    ~Font();
    int getSymbolWidth(void) const;
    int getSymbolHeight(void) const;
    int getCountOfSymbols(void) const;
    const QBitArray getSymbolImage(int i) const;
    quint16 getSymbolCode(int i) const;
    void setSymbolImage(int i, const QBitArray& image);
    void setSymbolCode(int i, quint16 code);
    virtual quint16 correctionCode(quint16 input);  // преобразование символов при их отсутствии в шрифте
};

#endif // FONT_H
