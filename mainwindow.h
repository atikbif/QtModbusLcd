#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lcdwidget.h"
#include <QListWidget>
#include "displaydata.h"
#include <QModelIndex>
#include "listofvars.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    EpsonFont* font;
    Display* lcd;
    ListOfVars* vars;
    LineOfSymbols copyLine;
    bool copyFlag;
    QString portName;
    QString portBaud;
    QStringList strVars;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    LcdWidget* display;
    QListWidget *listOfRows1, *listOfRows2, *listOfRows3, *listOfRows4;

private:
    void strAddBefore(unsigned char strNum);
    void strAddAfter(unsigned char strNum);
    void strCopy(unsigned char strNum);
    void strPaste(unsigned char strNum);
    void strDelete(unsigned char strNum);

public slots:

    void fillLists(DisplayData* display);
    void str1Changed(QModelIndex item);
    void str2Changed(QModelIndex item);
    void str3Changed(QModelIndex item);
    void str4Changed(QModelIndex item);

    void str1AddBefore(void);
    void str1AddAfter(void);
    void str1Copy(void);
    void str1Replace(void);
    void str1Delete(void);

    void str2AddBefore(void);
    void str2AddAfter(void);
    void str2Copy(void);
    void str2Replace(void);
    void str2Delete(void);

    void str3AddBefore(void);
    void str3AddAfter(void);
    void str3Copy(void);
    void str3Replace(void);
    void str3Delete(void);

    void str4AddBefore(void);
    void str4AddAfter(void);
    void str4Copy(void);
    void str4Replace(void);
    void str4Delete(void);

    void displayCopy(unsigned char rowNum, unsigned int lineNum);
    void displayReplace(unsigned char rowNum, unsigned int lineNum);

    void varEdit(void);
    void updateVarMessage(const QString &varMessage);

    void saveProject(void);
    void newProject(void);
    void openProject(void);
    void commonConfig(void);
    void aboutWindow(void);
    void dataToPLC(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
