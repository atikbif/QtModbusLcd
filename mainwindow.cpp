#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "epsonfont.h"
#include "display.h"
#include "displaydata.h"
#include "lcdwidget.h"

#include <QBoxLayout>
#include <QAction>
#include "varconfigdialog.h"

#include <QMessageBox>
#include "dialogcommonsettings.h"
#include "serialcommunication.h"
#include "communicationdialog.h"

void MainWindow::fillLists(DisplayData *display)
{
    QListWidget* list;

    unsigned char rowsCount = display->getRowCount();
    if(font != nullptr)
    for(int i=1;i<=4;i++)
    {
        switch(i)
        {
            case 1:list = listOfRows1;break;
            case 2:list = listOfRows2;break;
            case 3:list = listOfRows3;break;
            case 4:list = listOfRows4;break;
        }
        if(rowsCount>=i)
        {
            list->clear();
            for(int j=0;j<display->getCountOfLinesInRow(i-1);j++)
            {
                QString text = QString::number(j) + ")" + display->getLine(i-1,j)->getAsString(font);
                list->addItem(text);
            }
        }
        quint16 num = lcd->getCurrentLineNumber(i-1);
        if(num<list->count()) list->setCurrentRow(num,QItemSelectionModel::SelectCurrent);
    }
}

void MainWindow::str1Changed(QModelIndex item)
{
    display->stringChanged(0,item.row());
}

void MainWindow::str2Changed(QModelIndex item)
{
    display->stringChanged(1,item.row());
}

void MainWindow::str3Changed(QModelIndex item)
{
    display->stringChanged(2,item.row());
}

void MainWindow::str4Changed(QModelIndex item)
{
    display->stringChanged(3,item.row());
}

void MainWindow::str1AddBefore()
{
    strAddBefore(1);
}

void MainWindow::str1AddAfter()
{
    strAddAfter(1);
}

void MainWindow::str1Copy()
{
    strCopy(1);
}

void MainWindow::str1Replace()
{
    strPaste(1);
}

void MainWindow::str1Delete()
{
    strDelete(1);
}

void MainWindow::str2AddBefore()
{
    strAddBefore(2);
}

void MainWindow::str2AddAfter()
{
    strAddAfter(2);
}

void MainWindow::str2Copy()
{
    strCopy(2);
}

void MainWindow::str2Replace()
{
    strPaste(2);
}

void MainWindow::str2Delete()
{
    strDelete(2);
}

void MainWindow::str3AddBefore()
{
    strAddBefore(3);
}

void MainWindow::str3AddAfter()
{
    strAddAfter(3);
}

void MainWindow::str3Copy()
{
    strCopy(3);
}

void MainWindow::str3Replace()
{
    strPaste(3);
}

void MainWindow::str3Delete()
{
    strDelete(3);
}

void MainWindow::str4AddBefore()
{
    strAddBefore(4);
}

void MainWindow::str4AddAfter()
{
    strAddAfter(4);
}

void MainWindow::str4Copy()
{
    strCopy(4);
}

void MainWindow::str4Replace()
{
    strPaste(4);
}

void MainWindow::str4Delete()
{
    strDelete(4);
}

void MainWindow::displayCopy(unsigned char rowNum, unsigned int lineNum)
{
    copyLine = *(lcd->getData()->getLine(rowNum,lineNum));
    copyFlag = true;
    statusBar()->showMessage("Строка скопирована в буфер обмена",2000);
}

void MainWindow::displayReplace(unsigned char rowNum, unsigned int lineNum)
{
    if(copyFlag==false)
    {
        QMessageBox::warning(this,"сообщение","Буфер обмена пуст");
        return;
    }
    else
    {
        lcd->getData()->insertLine(copyLine,rowNum,lineNum);
        lcd->setLineNumber(rowNum,lineNum);
        lcd->setPos(0,rowNum);
        display->update();
        fillLists(lcd->getData());
    }
}

void MainWindow::varEdit()
{
    VarConfigDialog dialog;
    dialog.setVars(vars);
    dialog.updateTreeOfVars();
    dialog.exec();
}

void MainWindow::updateVarMessage(const QString &varMessage)
{
    ui->statusBar->showMessage(varMessage);
}

void MainWindow::saveProject()
{
    Settings conf;
    conf.setPortName(portName);
    conf.setPortBaud(portBaud);
    conf.setStrVars(strVars);
    bool res = conf.saveData(lcd->getData(),vars);
    if(res) statusBar()->showMessage("Проект успешно сохранён",2000);
}

void MainWindow::newProject()
{
    int reply = QMessageBox::question(this,"Сохранение изменений","Сохранить текущий проект?",QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(reply == QMessageBox::Cancel) return;
    if(reply == QMessageBox::Yes) saveProject();
    lcd->getData()->clear();
    strVars.clear();
    LineOfSymbols l;
    for(int i=0;i<lcd->getData()->getRowCount();i++)
        lcd->getData()->insertLine(l,i,0);
    lcd->setPos(0,0);
    lcd->setLineNumber(0,0);
    lcd->setLineNumber(1,0);
    lcd->setLineNumber(2,0);
    lcd->setLineNumber(3,0);

    if(lcd->getReplaceMode()) lcd->setReplaceMode(false);

    display->update();
    fillLists(lcd->getData());

    vars->deleteAllVars();
    statusBar()->showMessage("Новый проект успешно создан",2000);
}

void MainWindow::openProject()
{
    int reply = QMessageBox::question(this,"Сохранение изменений","Сохранить текущий проект?",QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(reply == QMessageBox::Cancel) return;
    if(reply == QMessageBox::Yes) saveProject();

     Settings conf;

     bool res = conf.openData(lcd->getData(),vars);
     if(res)
     {
         portName = conf.getPortName();
         portBaud = conf.getPortBaud();
         strVars = conf.getStringVars();
         lcd->setPos(0,0);
         lcd->setLineNumber(0,0);
         lcd->setLineNumber(1,0);
         lcd->setLineNumber(2,0);
         lcd->setLineNumber(3,0);
         if(lcd->getReplaceMode()) lcd->setReplaceMode(false);
         statusBar()->showMessage("Проект успешно открыт",2000);
     }
     display->update();
     fillLists(lcd->getData());
}

void MainWindow::commonConfig()
{
    DialogCommonSettings* dialog = new DialogCommonSettings;
    dialog->setVars(vars);
    dialog->updateDialog();
    dialog->setPortName(portName);
    dialog->setPortBaud(portBaud);
    if(strVars.count()>0) dialog->setStringsVar(1,strVars[0]);
    if(strVars.count()>1) dialog->setStringsVar(2,strVars[1]);
    if(strVars.count()>2) dialog->setStringsVar(3,strVars[2]);
    if(strVars.count()>3) dialog->setStringsVar(4,strVars[3]);
    if(dialog->exec()==QDialog::Accepted)
    {
        portName = dialog->getPortName();
        portBaud = dialog->getPortBaud();
        strVars.clear();
        for(int i=0;i<4;i++) strVars += dialog->getVarName(i+1);
    }
    delete dialog;
}

void MainWindow::aboutWindow()
{
    QMessageBox::about(this,"О программе","<p><b><i>Конфигуратор пультов. Версия 1.0 <a href=\"http://www.kontel.ru\">www.kontel.ru</a></i></b></p>");
}

void MainWindow::dataToPLC()
{
    SerialCommunication link;
    link.setVars(vars);
    CommunicationDialog* dialog = new CommunicationDialog;
    link.setData(lcd->getData());
    link.setFont(font);
    dialog->setMessage("Поиск устройства");
    connect(&link,SIGNAL(searchStep(float)),dialog,SLOT(updatePercent(float)));
    connect(&link,SIGNAL(searchError(QString)),dialog,SLOT(setMessage(QString)));
    connect(&link,SIGNAL(searchOK(QString)),dialog,SLOT(setMessage(QString)));
    connect(&link,SIGNAL(writeStep(float)),dialog,SLOT(updatePercent(float)));
    connect(&link,SIGNAL(writeError(QString)),dialog,SLOT(setMessage(QString)));
    connect(&link,SIGNAL(writeOK(QString)),dialog,SLOT(setMessage(QString)));
    link.searchController();
    dialog->exec();
    delete dialog;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    copyFlag = false;
    ui->setupUi(this);
    setWindowIcon(QIcon(":/hd_32.ico"));

    QAction *newAction = new QAction(QIcon(":/new_32.ico"), tr("Новый проект"), this);
    QAction *saveAction = new QAction(QIcon(":/save_32.ico"), tr("Сохранить проект"), this);
    QAction *openAction = new QAction(QIcon(":/open_32.ico"), tr("Загрузить проект"), this);
    QAction *configAction = new QAction(QIcon(":/opts_32.ico"), tr("Общие настройки"), this);
    //QAction *printAction = new QAction(QIcon(":/edit_32.ico"), tr("Строки и переменные в текстовый файл"), this);
    QAction *plcToAction = new QAction(QIcon(":/impt_32.ico"), tr("Загрузить в пульт"), this);
    QAction *plcFromAction = new QAction(QIcon(":/exp_32.ico"), tr("Прочитать из пульта"), this);
    QAction *varAct = new QAction(QIcon(":/apps_32.ico"), tr("Настройка переменных проекта"), this);
    QAction *aboutAction = new QAction(QIcon(":/about_32.ico"), tr("О программе"), this);
    connect(varAct, SIGNAL(triggered()), this, SLOT(varEdit()));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(saveProject()));
    connect(newAction,SIGNAL(triggered()),this,SLOT(newProject()));
    connect(openAction,SIGNAL(triggered()),this,SLOT(openProject()));
    connect(configAction,SIGNAL(triggered()),this,SLOT(commonConfig()));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(aboutWindow()));
    connect(plcToAction, SIGNAL(triggered()), this, SLOT(dataToPLC()));

    ui->mainToolBar->addAction(newAction);
    ui->mainToolBar->addAction(saveAction);
    ui->mainToolBar->addAction(openAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(configAction);
    ui->mainToolBar->addAction(varAct);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(plcToAction);
    ui->mainToolBar->addAction(plcFromAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(aboutAction);
    //ui->mainToolBar->addSeparator();
    //ui->mainToolBar->addAction(printAction);

    Settings conf;
    vars = conf.readVars();
    font = conf.readFont();
    DisplayData* lcdData = conf.readDisplay();
    lcd = new Display(lcdData);


    QVBoxLayout *layout = new QVBoxLayout;

    display = new LcdWidget(lcd);
    display->setFont(font);
    display->setVars(vars);

    connect(display,SIGNAL(updateMessage(QString)),this,SLOT(updateVarMessage(QString)));
    connect(display,SIGNAL(copyStr(unsigned char,uint)),this,SLOT(displayCopy(unsigned char,uint)));
    connect(display,SIGNAL(pasteStr(unsigned char,uint)),this,SLOT(displayReplace(unsigned char,uint)));

    layout->addWidget(display);

    listOfRows1 = new QListWidget();listOfRows1->setMinimumHeight(display->height()*1.5);
    listOfRows2 = new QListWidget();
    listOfRows3 = new QListWidget();
    listOfRows4 = new QListWidget();

    fillLists(lcdData);

    QHBoxLayout *listLayout = new QHBoxLayout;

    listLayout->addWidget(listOfRows1);
    listLayout->addWidget(listOfRows2);
    listLayout->addWidget(listOfRows3);
    listLayout->addWidget(listOfRows4);

    layout->addLayout(listLayout);

    layout->addStretch();

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    connect(display,SIGNAL(updateString(DisplayData*)),this,SLOT(fillLists(DisplayData*)));
    connect(listOfRows1,SIGNAL(clicked(QModelIndex)),this,SLOT(str1Changed(QModelIndex)));
    connect(listOfRows2,SIGNAL(clicked(QModelIndex)),this,SLOT(str2Changed(QModelIndex)));
    connect(listOfRows3,SIGNAL(clicked(QModelIndex)),this,SLOT(str3Changed(QModelIndex)));
    connect(listOfRows4,SIGNAL(clicked(QModelIndex)),this,SLOT(str4Changed(QModelIndex)));

    QMenu* contextMenu1 = new QMenu(listOfRows1);
    listOfRows1->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* addBefore1 = new QAction(QIcon(":/up_32.ico"),"Добавить строку перед",contextMenu1);
    QAction* addAfter1 = new QAction(QIcon(":/down_32.ico"),"Добавить строку после",contextMenu1);
    QAction* copy1 = new QAction(QIcon(":/copy_32.ico"),"Копировать строку в буфер",contextMenu1);
    QAction* replace1 = new QAction(QIcon(":/paste_32.ico"),"Заменить на строку из буфера",contextMenu1);
    QAction* delete1 = new QAction(QIcon(":/trash_32.ico"),"Удалить строку",contextMenu1);
    listOfRows1->addAction(addBefore1);
    listOfRows1->addAction(addAfter1);
    listOfRows1->addAction(copy1);
    listOfRows1->addAction(replace1);
    listOfRows1->addAction(delete1);
    connect(addBefore1,SIGNAL(triggered()),this,SLOT(str1AddBefore()));
    connect(addAfter1,SIGNAL(triggered()),this,SLOT(str1AddAfter()));
    connect(copy1,SIGNAL(triggered()),this,SLOT(str1Copy()));
    connect(replace1,SIGNAL(triggered()),this,SLOT(str1Replace()));
    connect(delete1,SIGNAL(triggered()),this,SLOT(str1Delete()));

    QMenu* contextMenu2 = new QMenu(listOfRows2);
    listOfRows2->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* addBefore2 = new QAction(QIcon(":/up_32.ico"),"Добавить строку перед",contextMenu2);
    QAction* addAfter2 = new QAction(QIcon(":/down_32.ico"),"Добавить строку после",contextMenu2);
    QAction* copy2 = new QAction(QIcon(":/copy_32.ico"),"Копировать строку в буфер",contextMenu2);
    QAction* replace2 = new QAction(QIcon(":/paste_32.ico"),"Заменить на строку из буфера",contextMenu2);
    QAction* delete2 = new QAction(QIcon(":/trash_32.ico"),"Удалить строку",contextMenu2);
    listOfRows2->addAction(addBefore2);
    listOfRows2->addAction(addAfter2);
    listOfRows2->addAction(copy2);
    listOfRows2->addAction(replace2);
    listOfRows2->addAction(delete2);
    connect(addBefore2,SIGNAL(triggered()),this,SLOT(str2AddBefore()));
    connect(addAfter2,SIGNAL(triggered()),this,SLOT(str2AddAfter()));
    connect(copy2,SIGNAL(triggered()),this,SLOT(str2Copy()));
    connect(replace2,SIGNAL(triggered()),this,SLOT(str2Replace()));
    connect(delete2,SIGNAL(triggered()),this,SLOT(str2Delete()));

    QMenu* contextMenu3 = new QMenu(listOfRows3);
    listOfRows3->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* addBefore3 = new QAction(QIcon(":/up_32.ico"),"Добавить строку перед",contextMenu3);
    QAction* addAfter3 = new QAction(QIcon(":/down_32.ico"),"Добавить строку после",contextMenu3);
    QAction* copy3 = new QAction(QIcon(":/copy_32.ico"),"Копировать строку в буфер",contextMenu3);
    QAction* replace3 = new QAction(QIcon(":/paste_32.ico"),"Заменить на строку из буфера",contextMenu3);
    QAction* delete3 = new QAction(QIcon(":/trash_32.ico"),"Удалить строку",contextMenu3);
    listOfRows3->addAction(addBefore3);
    listOfRows3->addAction(addAfter3);
    listOfRows3->addAction(copy3);
    listOfRows3->addAction(replace3);
    listOfRows3->addAction(delete3);
    connect(addBefore3,SIGNAL(triggered()),this,SLOT(str3AddBefore()));
    connect(addAfter3,SIGNAL(triggered()),this,SLOT(str3AddAfter()));
    connect(copy3,SIGNAL(triggered()),this,SLOT(str3Copy()));
    connect(replace3,SIGNAL(triggered()),this,SLOT(str3Replace()));
    connect(delete3,SIGNAL(triggered()),this,SLOT(str3Delete()));

    QMenu* contextMenu4 = new QMenu(listOfRows4);
    listOfRows4->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* addBefore4 = new QAction(QIcon(":/up_32.ico"),"Добавить строку перед",contextMenu4);
    QAction* addAfter4 = new QAction(QIcon(":/down_32.ico"),"Добавить строку после",contextMenu4);
    QAction* copy4 = new QAction(QIcon(":/copy_32.ico"),"Копировать строку в буфер",contextMenu4);
    QAction* replace4 = new QAction(QIcon(":/paste_32.ico"),"Заменить на строку из буфера",contextMenu4);
    QAction* delete4 = new QAction(QIcon(":/trash_32.ico"),"Удалить строку",contextMenu4);
    listOfRows4->addAction(addBefore4);
    listOfRows4->addAction(addAfter4);
    listOfRows4->addAction(copy4);
    listOfRows4->addAction(replace4);
    listOfRows4->addAction(delete4);
    connect(addBefore4,SIGNAL(triggered()),this,SLOT(str4AddBefore()));
    connect(addAfter4,SIGNAL(triggered()),this,SLOT(str4AddAfter()));
    connect(copy4,SIGNAL(triggered()),this,SLOT(str4Copy()));
    connect(replace4,SIGNAL(triggered()),this,SLOT(str4Replace()));
    connect(delete4,SIGNAL(triggered()),this,SLOT(str4Delete()));

    display->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::strAddBefore(unsigned char strNum)
{
    QListWidget* list;
    list = nullptr;
    switch(strNum)
    {
        case 1:list = listOfRows1;break;
        case 2:list = listOfRows2;break;
        case 3:list = listOfRows3;break;
        case 4:list = listOfRows4;break;
    }
    if(list != nullptr)
    {
        int i = list->currentRow();
        LineOfSymbols line;
        lcd->getData()->insertLineWithShift(line,strNum-1,i);
        lcd->setLineNumber(strNum-1,i);
        lcd->setPos(0,strNum-1);
        display->update();
        fillLists(lcd->getData());
    }
}

void MainWindow::strAddAfter(unsigned char strNum)
{
    QListWidget* list;
    list = nullptr;
    switch(strNum)
    {
        case 1:list = listOfRows1;break;
        case 2:list = listOfRows2;break;
        case 3:list = listOfRows3;break;
        case 4:list = listOfRows4;break;
    }
    if(list != nullptr)
    {
        int i = list->currentRow();
        LineOfSymbols line;
        lcd->getData()->insertLineWithShift(line,strNum-1,i+1);
        lcd->setLineNumber(strNum-1,i+1);
        lcd->setPos(0,strNum-1);
        display->update();
        fillLists(lcd->getData());
    }
}

void MainWindow::strCopy(unsigned char strNum)
{
    QListWidget* list;
    list = nullptr;
    switch(strNum)
    {
        case 1:list = listOfRows1;break;
        case 2:list = listOfRows2;break;
        case 3:list = listOfRows3;break;
        case 4:list = listOfRows4;break;
    }
    if(list != nullptr)
    {
        int i = list->currentRow();
        copyLine = *(lcd->getData()->getLine(strNum-1,i));
        copyFlag = true;
        statusBar()->showMessage("Строка скопирована в буфер обмена",2000);
    }
}

void MainWindow::strPaste(unsigned char strNum)
{
    QListWidget* list;
    list = nullptr;
    if(copyFlag==false)
    {
        QMessageBox::warning(this,"сообщение","Буфер обмена пуст");
        return;
    }
    else
    {
        switch(strNum)
        {
            case 1:list = listOfRows1;break;
            case 2:list = listOfRows2;break;
            case 3:list = listOfRows3;break;
            case 4:list = listOfRows4;break;
        }
        if(list != nullptr)
        {
            int i = list->currentRow();
            lcd->getData()->insertLine(copyLine,strNum-1,i);
            lcd->setLineNumber(strNum-1,i);
            lcd->setPos(0,strNum-1);
            display->update();
            fillLists(lcd->getData());
        }
    }
}

void MainWindow::strDelete(unsigned char strNum)
{
    QListWidget* list;
    list = nullptr;
    switch(strNum)
    {
        case 1:list = listOfRows1;break;
        case 2:list = listOfRows2;break;
        case 3:list = listOfRows3;break;
        case 4:list = listOfRows4;break;
    }
    if(list != nullptr)
    {
        int i = list->currentRow();
        if(list->count()>=2)
        {
            if(i!=0) lcd->setLineNumber(strNum-1,i-1);
            else lcd->setLineNumber(strNum-1,i);
            lcd->getData()->deleteLine(strNum-1,i);

            lcd->setPos(0,strNum-1);
            display->update();
            fillLists(lcd->getData());
        }
        else QMessageBox::warning(this,"Недопустимая операция","Невозможно удалить последнюю строку");
    }
}
