#ifndef SYMBOLWIDGET_H
#define SYMBOLWIDGET_H

#include <QWidget>
#include <QBitArray>
#include "display.h"
#include "epsonfont.h"
#include <QAction>
#include "listofvars.h"

class LcdWidget : public QWidget
{
    Q_OBJECT
    const unsigned char pointSize;
    Display* lcd;
    EpsonFont* font;

    QAction *imbedVarAct;
    QAction *editVarAct;
    QAction *delVarAct;
    QAction *addBeforeAct;
    QAction *addAfterAct;
    QAction *copyAct;
    QAction *replaceAct;
    ListOfVars *vars;

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    explicit LcdWidget(Display* lcd, QWidget *parent = 0);
    void setFont(EpsonFont *font);
    void stringChanged(int strNum, int curNum);
    void setVars(ListOfVars* ptr) {vars = ptr;}

signals:
    void updateString(DisplayData* ptr);
    void updateMessage(const QString &txt);
    void copyStr(unsigned char rowNum, unsigned int lineNum);
    void pasteStr(unsigned char rowNum, unsigned int lineNum);
public slots:
    void imbedVar(void);
    void editVar(void);
    void delVar(void);
    void addBefore(void);
    void addAfter(void);
    void copyString(void);
    void replaceString(void);

};

#endif // SYMBOLWIDGET_H
