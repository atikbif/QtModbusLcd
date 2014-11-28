#ifndef IMAGEOFVARIABLE_H
#define IMAGEOFVARIABLE_H

#include <QString>

class ImageOfVariable
{
    int maxCountOfCells;
    unsigned char position;
    unsigned char CellsBeforeDot;
    unsigned char CellsAfterDot;
    bool fillFlag;          // if it's true insignificant cells will be filled of zero digit
    QString pattern;
public:
    ImageOfVariable();
    //ImageOfVariable(const ImageOfVariable& image);
    //ImageOfVariable& operator=(const ImageOfVariable&);

    void setFormat(const QString &inputString);
    void setPosition(unsigned char pos);
    unsigned char getPosition(void) const;
    unsigned char getCountOfCellsBeforeDot(void) const;
    unsigned char getCountOfCellsAfterDot(void) const;
    unsigned char getLength(void);
    QString getFormat(void);
};

#endif // IMAGEOFVARIABLE_H
