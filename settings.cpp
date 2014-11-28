#include "settings.h"
#include <QFile>
#include <QDataStream>

#include "lineofsymbols.h"
#include "modbusvar.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>

Settings::Settings()
{
}

DisplayData *Settings::readDisplay()
{
    DisplayData* lcd = new DisplayData(4);
    LineOfSymbols l1,l2,l3,l4;

    lcd->insertLine(l1,0,0);
    lcd->insertLine(l2,1,0);
    lcd->insertLine(l3,2,0);
    lcd->insertLine(l4,3,0);
    return lcd;
}

EpsonFont *Settings::readFont()
{
    EpsonFont* font = new EpsonFont("EpsonRussia");
    QFile file("main.kdf");
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_3);
        quint32 codeWord;
        in >> codeWord;
        if(codeWord == 0xa3814f32)
        {
            quint8 vers;
            in >> vers;
            switch(vers)
            {
                case 0x01:

                    quint8 width,height;
                    quint16 count;

                    in >> width;
                    in >> height;
                    in >> count;

                    if(count == 256)
                    {
                        for(int i=0;i<256;i++)
                        {
                            QBitArray s;
                            in >> s;
                            font->setSymbolImage(i,s);
                            quint32 code;
                            in >> code;
                            font->setSymbolCode(i,code);
                        }
                    }
                    break;

                default:;
            }
        }
        file.close();
    }
    return font;
}

ListOfVars *Settings::readVars()
{
    ListOfVars* vars = new ListOfVars;
    return vars;
}

bool Settings::saveData(DisplayData *ptr, ListOfVars *list)
{
    QString fName = QFileDialog::getSaveFileName(nullptr,"Сохранение проекта", "", "(*.pum);");
    if (fName.isEmpty()) return false;

    QFile file(fName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(nullptr,"Ошибка", "Невозможно открыть файл");
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);
    quint32 codeWord = 0x374F80A2;
    out << codeWord;
    quint8 vers = 0x01;
    out << vers;

    QVector<modbusVar*> varList = list->getAllVars();
    quint16 cnt = varList.count();
    out << cnt;

    for(int i=0;i<cnt;i++)
    {
        out << varList[i]->getAddion();
        out << varList[i]->getComment();
        out << varList[i]->getGroupName();
        out << varList[i]->getMemAddress();
        out << varList[i]->getMultiplier();
        out << varList[i]->getName();
        out << varList[i]->getPlcAddress();
        out << varList[i]->getType();
        out << varList[i]->isItEditable();
        out << varList[i]->isVarSign();
    }

    unsigned char rCount = ptr->getRowCount();
    out << rCount;

    for(int i=0;i<rCount;i++)
    {
        quint16 n = ptr->getCountOfLinesInRow(i);
        out << n;
        for(int j=0;j<n;j++)
        {
            LineOfSymbols* line = ptr->getLine(i,j);
            unsigned char length;
            length = line->getLength();
            out << length;
            QVector<int> varPos;
            for(int k=0;k<length;k++)
            {
                out << line->getSymbol(k);
                if(line->isBeginOfVarHere(k)) varPos += k;
            }
            quint8 varsCount = varPos.count();
            out << varsCount;
            for(int k=0;k<varsCount;k++)
            {
                int x = varPos[k];
                QString vName = line->getVar(x);
                QString patternString = line->getPattern(x);

                out << x;
                out << vName;
                out << patternString;
            }
        }
        if(i<strVars.count()) out << strVars.at(i);
    }
    out << portName;
    out << portBaud;

    file.close();
    return true;
}

bool Settings::openData(DisplayData *ptr, ListOfVars *list)
{
    QString fName = QFileDialog::getOpenFileName(nullptr,"Открытие проекта", "", "(*.pum);");
    if (fName.isEmpty()) return false;

    QFile file(fName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(nullptr,"Ошибка", "Невозможно открыть файл");
        return false;
    }

    ptr->clear();
    LineOfSymbols l;
    for(int i=0;i<ptr->getRowCount();i++)
        ptr->insertLine(l,i,0);

    list->deleteAllVars();

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);
    quint32 codeWord;
    in >> codeWord;
    if(codeWord != 0x374F80A2) return false;
    quint8 vers;
    in >> vers;
    if(vers == 1)
    {
        quint16 cnt;
        in >> cnt;

        for(int i=0;i<cnt;i++)
        {
            double varAddion; in >> varAddion;
            QString comment; in >> comment;
            QString grName; in >> grName;
            int memAddress; in >> memAddress;
            double multiplier; in >> multiplier;
            QString name; in >> name;
            int plcAddress; in >> plcAddress;
            QString type; in >> type;
            bool editable; in >> editable;
            bool sign; in >> sign;

            modbusVar* var = new modbusVar();
            var->setComment(comment);
            var->setCorrection(multiplier,varAddion);
            var->setEditable(editable);
            var->setGroupName(grName);
            var->setMemAddress(memAddress);
            var->setName(name);
            var->setPlcAddress(plcAddress);
            var->setSign(sign);
            var->setType(type);
            list->addVar(var);
        }

        unsigned char rCount;
        in >> rCount;
        if(rCount != ptr->getRowCount()) return false;
        strVars.clear();
        for(int i=0;i<rCount;i++)
        {
            quint16 n;
            in >> n;
            for(int j=0;j<n;j++)
            {
                LineOfSymbols l;
                unsigned char length;
                in >> length;
                QVector<quint16> symbols;
                for(int k=0;k<length;k++)
                {
                    quint16 symb;
                    in >> symb;
                    //l.insertSymbol(symb,k);
                    symbols+=symb;
                }
                quint8 varsCount;
                in >> varsCount;
                for(int k=0;k<varsCount;k++)
                {
                    int x;
                    QString vName;
                    QString patternString;

                    in >> x;
                    in >> vName;
                    in >> patternString;

                    modbusVar* v = list->getVar(vName);
                    if(v!=nullptr)
                    {
                        l.insertVar(patternString,v,x);
                    }
                }
                l.setReplaceMode(true);
                for(int k=0;k<symbols.count();k++) l.insertSymbol(symbols[k],k);
                l.setReplaceMode(false);

                ptr->insertLine(l,i,j);
            }
            QString vName;
            in >> vName;
            strVars += vName;
        }
        in >> portName;
        in >> portBaud;
    }
    else return false;
    return true;
}
