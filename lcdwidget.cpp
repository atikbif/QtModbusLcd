#include "lcdwidget.h"
#include <QPainter>
#include "epsonfont.h"
#include "lineofsymbols.h"
#include <QMouseEvent>
#include <QMenu>
#include "dialogvarchoose.h"
#include <QMessageBox>


void LcdWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    if(lcd!=nullptr)
    {
        QPainter  painter(this);
        unsigned char w = font->getSymbolWidth();
        unsigned char h = font->getSymbolHeight();
        painter.setPen(Qt::darkGray);
        painter.drawRect(0,0,20+(w*pointSize+2)*lcd->getCurrentLine(0)->getLength(),20+(h*pointSize+2)*lcd->getCountOfRows());

        painter.setPen(Qt::lightGray);
        for(int y=0;y<lcd->getCountOfRows();y++)
        for(int x=0;x<lcd->getCurrentLine(y)->getLength();x++)
        {
            for(int i=0;i<h;i++)
            for(int j=0;j<w;j++)
            {
                quint16 s;
                bool var = false;
                if(lcd->getCurrentLine(y)->isVarHere(x)) {var=true;}
                s = lcd->getCurrentLine(y)->getSymbol(x);
                s = font->correctionCode(s);
                painter.setBrush(Qt::white);
                if(font->getSymbolImage(s).count()>j*h+i)
                {
                    bool col = font->getSymbolImage(s)[j*h+i];
                    if(col)
                    {
                        if(var) painter.setBrush(QBrush(Qt::darkBlue));
                        else painter.setBrush(QBrush(Qt::darkGray));
                    }
                }
                painter.drawRect(10+j*pointSize+x*(pointSize*w+2),10+i*pointSize+y*(pointSize*h+2),pointSize,pointSize);
            }
        }
        painter.setPen(Qt::NoPen);
        if(lcd->getReplaceMode()) painter.setBrush(Qt::white);
        else painter.setBrush(Qt::darkBlue);
        painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        painter.drawRect(10+lcd->getX()*(pointSize*w+2),10+lcd->getY()*(pointSize*h+2),pointSize*w,pointSize*h);
    }
}

void LcdWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    //if (event->button() == Qt::LeftButton)
    {
        int x = event->x();if(x<10) return;
        int y = event->y();if(y<10) return;
        x-=10;y-=10;
        x = x/(pointSize*font->getSymbolWidth()+2);
        y = y/(pointSize*font->getSymbolHeight()+2);
        lcd->setPos(x,y);
        QString varText;
        if(lcd->getCurrentLine(lcd->getY())->isVarHere(lcd->getX()))
        {
            QString vName = lcd->getCurrentLine(lcd->getY())->getVar(lcd->getX());
            modbusVar *v = vars->getVar(vName);
            if(v != nullptr)
            {
                varText = v->getName() + " (" + v->getComment() + ")";
                emit updateMessage(varText);
            }else emit updateMessage("Переменная была удалена");
        }else emit updateMessage("");
        update();
    }
    //else
    if (event->button() == Qt::RightButton)
    {
        QMenu menu(this);
        menu.addAction(imbedVarAct);
        menu.addAction(editVarAct);
        menu.addAction(delVarAct);
        menu.addSeparator();
        menu.addAction(addBeforeAct);
        menu.addAction(addAfterAct);
        menu.addAction(copyAct);
        menu.addAction(replaceAct);
        menu.exec(event->globalPos());
    }

}

void LcdWidget::keyPressEvent(QKeyEvent *event)
{
    bool cmd = false;
    switch(event->key())
    {
        case Qt::Key_Right: lcd->posRight();update(); cmd=true; break;
        case Qt::Key_Left: lcd->posLeft();update(); cmd=true; break;
        case Qt::Key_Up: lcd->posUp();update(); cmd=true; break;
        case Qt::Key_Down: lcd->posDown();update(); cmd=true; break;
        case Qt::Key_Backspace:if(lcd->getCurrentLine(lcd->getY())->backspace(lcd->getX())) lcd->posLeft();update(); cmd=true; break;
        case Qt::Key_Delete:lcd->getCurrentLine(lcd->getY())->deleteSymbol(lcd->getX()); update(); cmd=true; break;
        case Qt::Key_Insert:lcd->toggleReplaceMode();update();cmd=true;break;
        case Qt::Key_Home:lcd->setPos(0,lcd->getY());cmd=true;update();break;
        case Qt::Key_End:lcd->setPos(lcd->getCurrentLine(lcd->getY())->getLength()-1,lcd->getY());update();cmd=true;break;
        case Qt::Key_Return:
            if(lcd->getX() == lcd->getCurrentLine(lcd->getY())->getLength()-1)
            {
                LineOfSymbols line;
                int i = lcd->getCurrentLineNumber(lcd->getY())+1;
                lcd->getData()->insertLineWithShift(line,lcd->getY(),i);
                lcd->setLineNumber(lcd->getY(),i);
                lcd->setPos(0,lcd->getY());
                update();
                cmd = true;
            }
            break;
    }
    if(cmd==false)
    {
        QString s = event->text();
        if(s.length())
        {
            lcd->getCurrentLine(lcd->getY())->insertSymbol(s[0].unicode(),lcd->getX());
            lcd->posRight();
            update();
        }
    }
    emit updateString(lcd->getData());
}

LcdWidget::LcdWidget(Display *lcd, QWidget *parent) :
    QWidget(parent),pointSize(6)
{
    vars = nullptr;
    this->lcd = lcd;
    this->setFixedSize((pointSize*5+2)*20+21,(pointSize*7+2)*4+21);

    imbedVarAct = new QAction(QIcon(":/add_32.ico"), tr("Вставить переменную"), this);
    editVarAct = new QAction(QIcon(":/apps_32.ico"), tr("Изменить переменную"), this);
    delVarAct = new QAction(QIcon(":/trash_32.ico"), tr("Удалить переменную"), this);
    addBeforeAct = new QAction(QIcon(":/up_32.ico"), tr("Добавить строку перед"), this);
    addAfterAct = new QAction(QIcon(":/down_32.ico"), tr("Добавить строку после"), this);
    copyAct = new QAction(QIcon(":/copy_32.ico"), tr("Копировать строку в буфер"), this);
    replaceAct = new QAction(QIcon(":/paste_32.ico"), tr("Заменить на строку из буфера"), this);
    connect(imbedVarAct, SIGNAL(triggered()), this, SLOT(imbedVar()));
    connect(editVarAct, SIGNAL(triggered()), this, SLOT(editVar()));
    connect(delVarAct, SIGNAL(triggered()), this, SLOT(delVar()));
    connect(addBeforeAct, SIGNAL(triggered()), this, SLOT(addBefore()));
    connect(addAfterAct, SIGNAL(triggered()), this, SLOT(addAfter()));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copyString()));
    connect(replaceAct, SIGNAL(triggered()), this, SLOT(replaceString()));
}

void LcdWidget::setFont(EpsonFont *font)
{
    this->font = font;
}

void LcdWidget::stringChanged(int strNum, int curNum)
{
    if((curNum>=0)&&(strNum>=0))
    {
        if(strNum<lcd->getCountOfRows())
        {
            if(curNum<lcd->getData()->getCountOfLinesInRow(strNum))
            {
                lcd->setLineNumber(strNum,curNum);
                lcd->setPos(0,strNum);
                update();
                setFocus();
            }
        }
    }
}

void LcdWidget::imbedVar()
{
    DialogVarChoose* varDialog = new DialogVarChoose(this);
    varDialog->setVars(vars);
    varDialog->generateTree();
    if(varDialog->exec()==QDialog::Accepted)
    {
        QString varName = varDialog->getName();
        QString varPattern = varDialog->getPattern();
        varPattern.replace('*','#');
        varPattern.replace(',','.');
        modbusVar *v = vars->getVar(varName);
        if(v!=nullptr)
        {
            if(lcd->getCurrentLine(lcd->getY())->insertVar(varPattern,v,lcd->getX()))
            {
                emit updateString(lcd->getData());
                emit updateMessage(v->getName()+" ("+v->getComment()+")");
            }
            else
            {
                QMessageBox::warning(this,"ошибка","невозможно вставить переменную");
            }
        }
    }
    delete varDialog;
}

void LcdWidget::editVar()
{
    if(lcd->getCurrentLine(lcd->getY())->isVarHere(lcd->getX())==false) return;
    DialogVarChoose* varDialog = new DialogVarChoose(this);
    varDialog->setVars(vars);
    varDialog->generateTree();
    QString pattern = lcd->getCurrentLine(lcd->getY())->getPattern(lcd->getX());
    pattern.replace('#','*');
    QString vName = lcd->getCurrentLine(lcd->getY())->getVar(lcd->getX());
    varDialog->setPattern(pattern);
    varDialog->setName(vName);
    if(varDialog->exec()==QDialog::Accepted)
    {
        QString varName = varDialog->getName();
        QString varPattern = varDialog->getPattern();
        varPattern.replace('*','#');
        varPattern.replace(',','.');
        modbusVar *v = vars->getVar(varName);
        if(v!=nullptr)
        {
            if(lcd->getCurrentLine(lcd->getY())->replaceVar(varPattern,v,lcd->getX()))
            {
                emit updateString(lcd->getData());
                emit updateMessage(v->getName()+" ("+v->getComment()+")");
            }
            else
            {
                QMessageBox::warning(this,"ошибка","невозможно изменить переменную с заданными параметрами");
            }
        }
    }
    delete varDialog;
}

void LcdWidget::delVar()
{
    if(lcd->getCurrentLine(lcd->getY())->isVarHere(lcd->getX()))
    {
        lcd->getCurrentLine(lcd->getY())->deleteSymbol(lcd->getX()); update();
        emit updateString(lcd->getData());
    }
}

void LcdWidget::addBefore()
{
    int i = lcd->getCurrentLineNumber(lcd->getY());
    LineOfSymbols line;
    lcd->getData()->insertLineWithShift(line,lcd->getY(),i);
    lcd->setLineNumber(lcd->getY(),i);
    lcd->setPos(0,lcd->getY());
    update();
    emit updateString(lcd->getData());
}

void LcdWidget::addAfter()
{
    int i = lcd->getCurrentLineNumber(lcd->getY())+1;
    LineOfSymbols line;
    lcd->getData()->insertLineWithShift(line,lcd->getY(),i);
    lcd->setLineNumber(lcd->getY(),i);
    lcd->setPos(0,lcd->getY());
    update();
    emit updateString(lcd->getData());
}

void LcdWidget::copyString()
{
    emit copyStr(lcd->getY(),lcd->getCurrentLineNumber(lcd->getY()));
}

void LcdWidget::replaceString()
{
    emit pasteStr(lcd->getY(),lcd->getCurrentLineNumber(lcd->getY()));
}

