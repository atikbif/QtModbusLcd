
#ifndef EPSONFONT_H
#define EPSONFONT_H

#include "font.h"
#include <QMap>

// Class for work with "Epson Russian" encoding for lcd displays

class EpsonFont: public Font
{
    QMap<quint16,quint16> correctionTable;
    void createTable(void);
public:
    explicit EpsonFont(const QString &fontName="epson", int count = 256, int sWidth = 5, int sHeight = 7);
    quint16 correctionCode(quint16 input);  // преобразование символов при их отсутствии в шрифте
    quint16 invertCorrection(quint16 input);  // преобразование из кодировки шрифта в юникод
};

#endif // EPSONFONT_H
