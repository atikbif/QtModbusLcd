#include "lineofsymbols.h"
#include <QString>
#include <QChar>
#include <QMessageBox>

LineOfSymbols::LineOfSymbols(): length(20), replaceMode(false)
{
    symbols.fill(0x20,length);
}

void LineOfSymbols::setReplaceMode(bool value)
{
    replaceMode = value;
}

bool LineOfSymbols::isReplaceModeActive()
{
    return replaceMode;
}

bool LineOfSymbols::insertSymbol(quint16 code, unsigned char pos)
{
    if(pos>=length) return 0;
    if(isBeginOfVarHere(pos) || (isVarHere(pos)==false))
    {
        if(replaceMode)
        {
            if(isVarHere(pos)==false) symbols.replace(pos,code);
        }
        else
        {
            if(symbols.at(length-1) == 0x20)
            {
                symbols.insert(pos,code);
                symbols.removeLast();

                // shift variables

                for(int i=0;i<vars.count();i++)
                {
                    unsigned char x = vars[i].view.getPosition();
                    if(x >= pos) vars[i].view.setPosition(x + 1);
                }
            }
        }
    }
    return true;
}

void LineOfSymbols::deleteSymbol(unsigned char pos)
{
    if(pos<length)
    {
        if(isVarHere(pos))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Question);
            msg.setWindowTitle("Подтверждение операции");
            msg.setText("Удалить переменную из строки?");
            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Cancel);
            int ret = msg.exec();
            if(ret==QMessageBox::Ok) deleteVar(pos);
        }
        else
        {
            //shift variables
            for(int i=0;i<vars.count();i++)
            {
                unsigned char x = vars[i].view.getPosition();
                if((x >= pos)&&(x>0)) vars[i].view.setPosition(x -1);
            }

            symbols.remove(pos);
            symbols += 0x20;
        }
    }
}

bool LineOfSymbols::backspace(unsigned char pos)
{
    if((pos>0)&&(pos<length))
    {
        if(isVarHere(pos-1))
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Question);
            msg.setWindowTitle("Подтверждение операции");
            msg.setText("Удалить переменную из строки?");
            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Cancel);
            int ret = msg.exec();
            if(ret==QMessageBox::Ok) deleteVar(pos-1);
            else return false;
        }
        else
        {
            symbols.remove(pos-1);
            symbols += 0x20;

            //shift variables
            for(int i=0;i<vars.count();i++)
            {
                unsigned char x = vars[i].view.getPosition();
                if((x >= pos)&&(x>0)) vars[i].view.setPosition(x -1);
            }
        }
    }
    return true;
}

bool LineOfSymbols::insertVar(const QString &pattern, modbusVar *var, unsigned int pos)
{
    // check free space for variable
    if(isVarHere(pos)) return false;

    ImageOfVariable imageVar;
    imageVar.setFormat(pattern);
    imageVar.setPosition(pos);
    unsigned char allowableLength=0;
    for(unsigned char i=length-1;i>=pos;i--)
    {
       if(isVarHere(i) || (i==0)) break;else allowableLength++;
    }
    unsigned char length = imageVar.getLength();
    if(length>allowableLength) return false;

    // fill space under new variable
    symbols.insert(pos,imageVar.getLength(),0xFF);
    for(int i=0;i<length;i++) symbols.removeLast();

    // shift other variables
    for(int i=0;i<vars.count();i++)
    {
        unsigned char x = vars[i].view.getPosition();
        if(x >= pos) vars[i].view.setPosition(x + length);
    }

    // add variable to list
    QString vName;
    if(var!=nullptr) vName = var->getName();
    variable varDescription {imageVar,vName};
    vars+=varDescription;

    return true;
}

bool LineOfSymbols::replaceVar(const QString &pattern, modbusVar *var, unsigned int pos)
{
    variable curVar;
    QVector<quint16> curSymbols;

    for(int i=0;i<vars.count();i++)
    {
        unsigned char startPos = vars[i].view.getPosition();
        unsigned char endPos = startPos + vars[i].view.getLength()-1;
        if((pos>=startPos)&&(pos<=endPos))
        {
            curVar = vars[i];
            for(int j=startPos;j<=endPos;j++) curSymbols+=symbols.at(j);
            break;
        }
    }

    for(int i=0;i<vars.count();i++)
    {
        unsigned char startPos = vars[i].view.getPosition();
        unsigned char endPos = startPos + vars[i].view.getLength()-1;
        if((pos>=startPos)&&(pos<=endPos))
        {
            vars.remove(i);
            for(int j=startPos;j<=endPos;j++)
            {
                if(j<symbols.count()) deleteSymbol(startPos);
            }
            bool res = insertVar(pattern,var,startPos);
            if(res==false)
            {
                for(int j=startPos;j<=endPos;j++)
                {
                    insertSymbol(curSymbols[j-startPos],j);
                }
                vars+=curVar;
            }
            return res;
        }
    }
    return false;
}

bool LineOfSymbols::deleteVar(unsigned char pos)
{
    int i;  // index of variable in list
    unsigned char startPos;
    unsigned char endPos;
    // look for variable
    for(i=0;i<vars.count();i++)
    {
        startPos = vars[i].view.getPosition();
        endPos = startPos + vars[i].view.getLength()-1;
        if((pos>=startPos)&&(pos<endPos)) break;
    }
    if(i==vars.count()) return false;
    vars.remove(i); // delete variable
    unsigned char length = endPos - startPos + 1;
    symbols.remove(startPos,length);    // delete symbols under variable
    for(int j=0;j<length;j++) symbols.append(0x20); // add symbols to the end of string
    // shift other variables
    for(int i=0;i<vars.count();i++)
    {
        unsigned char x = vars[i].view.getPosition();
        if((x >= startPos) && (x >= length)) vars[i].view.setPosition(x - length);
    }
    return true;
}

bool LineOfSymbols::isVarHere(unsigned char pos)
{
    for(int i=0;i<vars.count();i++)
    {
        unsigned char startPos = vars[i].view.getPosition();
        unsigned char endPos = startPos + vars[i].view.getLength()-1;
        if((pos>=startPos)&&(pos<=endPos)) return true;
    }
    return false;
}

QString LineOfSymbols::getVar(unsigned char pos)
{
    for(int i=0;i<vars.count();i++)
    {
        unsigned char startPos = vars[i].view.getPosition();
        unsigned char endPos = startPos + vars[i].view.getLength()-1;
        if((pos>=startPos)&&(pos<=endPos)) return vars[i].varName;
    }
    return QString();
}

QString LineOfSymbols::getPattern(unsigned char pos)
{
    for(int i=0;i<vars.count();i++)
    {
        unsigned char startPos = vars[i].view.getPosition();
        unsigned char endPos = startPos + vars[i].view.getLength()-1;
        if((pos>=startPos)&&(pos<=endPos)) return vars[i].view.getFormat();
    }
    return QString();
}

bool LineOfSymbols::isBeginOfVarHere(unsigned char pos)
{
    for(int i=0;i<vars.count();i++)
    {
        unsigned char startPos = vars[i].view.getPosition();
        if(pos==startPos) return true;
    }
    return false;
}

quint16 LineOfSymbols::getSymbol(unsigned char pos)
{
    if(pos<symbols.count())
    {
        if(isVarHere(pos))
        {
            for(int i=0;i<vars.count();i++)
            {
                unsigned char startPos = vars[i].view.getPosition();
                unsigned char endPos = startPos + vars[i].view.getLength()-1;
                if((pos>=startPos)&&(pos<=endPos))
                {
                    //return QChar('#').unicode();
                    QString str = vars[i].view.getFormat();
                    QChar ch = str.at(pos-startPos);
                    return ch.unicode();
                }
            }
        }
        else
        return symbols.at(pos);
    }
    return 0;
}

const QString LineOfSymbols::getAsString(EpsonFont *font)
{
    QString res;
    for(int i=0;i<getLength();i++)
    {
        quint16 un = getSymbol(i);              // code in EpsonFont
        un = font->invertCorrection(un);        // unicode value
        QChar ch = QChar((ushort)un);
        res += ch;
    }
    return res;
}

int LineOfSymbols::getCountOfVars()
{
    return vars.count();
}
