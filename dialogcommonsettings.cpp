#include "dialogcommonsettings.h"
#include "ui_dialogcommonsettings.h"
#include <QVector>
#include "modbusvar.h"
#include <QSerialPortInfo>
#include <QSerialPort>

DialogCommonSettings::DialogCommonSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCommonSettings)
{
    ui->setupUi(this);
    list = nullptr;
}

DialogCommonSettings::~DialogCommonSettings()
{
    delete ui;
}

void DialogCommonSettings::updateDialog()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo p,portList)
    {
        ui->comboBoxCOM->addItem(p.portName());
    }

    ui->comboBoxSpeed->addItem("9600");
    ui->comboBoxSpeed->addItem("19200");
    ui->comboBoxSpeed->addItem("38400");
    ui->comboBoxSpeed->addItem("57600");
    ui->comboBoxSpeed->addItem("115200");
    ui->comboBoxSpeed->setCurrentText("19200");

    if(list!=nullptr)
    {
        ui->comboBoxL1->addItem("_без привязки к переменной");
        ui->comboBoxL2->addItem("_без привязки к переменной");
        ui->comboBoxL3->addItem("_без привязки к переменной");
        ui->comboBoxL4->addItem("_без привязки к переменной");
        QVector<modbusVar*> varList = list->getAllVars();
        for(int i=0;i<varList.count();i++)
        {
            QString vName = varList[i]->getName();
            ui->comboBoxL1->addItem(vName);
            ui->comboBoxL2->addItem(vName);
            ui->comboBoxL3->addItem(vName);
            ui->comboBoxL4->addItem(vName);
        }
        ui->comboBoxL1->model()->sort(0);
        ui->comboBoxL2->model()->sort(0);
        ui->comboBoxL3->model()->sort(0);
        ui->comboBoxL4->model()->sort(0);
    }
}

QString DialogCommonSettings::getPortName()
{
    return ui->comboBoxCOM->currentText();
}

QString DialogCommonSettings::getPortBaud()
{
    return ui->comboBoxSpeed->currentText();
}

QString DialogCommonSettings::getVarName(unsigned char strNum)
{
    QString vName;
    switch(strNum)
    {
        case 1: vName = ui->comboBoxL1->currentText();break;
        case 2: vName = ui->comboBoxL2->currentText();break;
        case 3: vName = ui->comboBoxL3->currentText();break;
        case 4: vName = ui->comboBoxL4->currentText();break;
    }
    return vName;
}

void DialogCommonSettings::setPortName(const QString &pName)
{
    ui->comboBoxCOM->setCurrentText(pName);
}

void DialogCommonSettings::setPortBaud(const QString &pBaud)
{
    ui->comboBoxSpeed->setCurrentText(pBaud);
}

void DialogCommonSettings::setStringsVar(unsigned char num, const QString &vName)
{
    switch(num)
    {
        case 1:ui->comboBoxL1->setCurrentText(vName);break;
        case 2:ui->comboBoxL2->setCurrentText(vName);break;
        case 3:ui->comboBoxL3->setCurrentText(vName);break;
        case 4:ui->comboBoxL4->setCurrentText(vName);break;
    }
}
