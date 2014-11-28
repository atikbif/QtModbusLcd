#include "vareditdialog.h"
#include "ui_vareditdialog.h"
#include <QMessageBox>

VarEditDialog::VarEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VarEditDialog)
{
    ui->setupUi(this);
    newVar = false;
}

VarEditDialog::~VarEditDialog()
{
    delete ui;
}

void VarEditDialog::setVar(modbusVar *ptr)
{
    var = ptr;
    if(var!=nullptr)
    {
        ui->lineEditName->setText(var->getName());
        ui->lineEditComment->setText(var->getComment());
        ui->checkBoxVarEdit->setChecked(var->isItEditable());
        QVector<QString> varsTypes = var->getTypes();
        foreach(QString typeName, varsTypes) ui->comboBoxVarsType->addItem(typeName);
        ui->comboBoxVarsType->setCurrentText(var->getType());
        ui->doubleSpinBoxAdd->setValue(var->getAddion());
        ui->doubleSpinBoxCoeff->setValue(var->getMultiplier());
        ui->spinBoxMemAddress->setValue(var->getMemAddress());
        ui->spinBoxNetAddress->setValue(var->getPlcAddress());
        ui->checkBoxSign->setChecked(var->isVarSign());
        foreach(QString grName, groups)
        {
            ui->comboBoxGroups->addItem(grName);
        }
        if(var->isItInGroup())
        {
            ui->comboBoxGroups->setCurrentText(var->getGroupName());
            ui->radioButtonExistGroup->setChecked(true);
        }
        else
        {
            ui->comboBoxGroups->setEnabled(false);
            ui->radioButtonWithoutGroup->setChecked(true);
            ui->comboBoxGroups->setEnabled(false);
        }
        startName = var->getName();
    }
}

void VarEditDialog::setGroups(const QStringList &list)
{
    groups = list;
}

void VarEditDialog::setVarsNames(const QStringList &list)
{
    varsName = list;
}

void VarEditDialog::on_pushButtonOK_clicked()
{
    QString errMessage;
    if(ui->lineEditName->text().isEmpty())
    {
        errMessage += "Не указано имя переменной\r\n";
    }
    if(ui->checkBoxVarEdit->isChecked() && ui->comboBoxVarsType->currentText().contains("INPUT"))
    {
        errMessage += "Тип переменной не допускает изменения с пульта\r\n";
    }
    if(ui->radioButtonNewGroup->isChecked() && ui->lineEditNewGroup->text().isEmpty())
    {
        errMessage += "Необходимо ввести имя новой группы\r\n";
    }
    if(ui->radioButtonNewGroup->isChecked() && groups.contains(ui->lineEditNewGroup->text()))
    {
        errMessage += "Группа с таким именем уже существует\r\n";
    }
    if((ui->lineEditName->text() != startName) && varsName.contains(ui->lineEditName->text()))
    {
        errMessage += "Переменная с таким именем уже существует\r\n";
    }
    if(!errMessage.isEmpty())
    {
        QMessageBox::critical(this,"Error",errMessage);
    }
    else
    {
        var->setComment(ui->lineEditComment->text());
        var->setCorrection(ui->doubleSpinBoxCoeff->value(),ui->doubleSpinBoxAdd->value());
        var->setEditable(ui->checkBoxVarEdit->isChecked());
        QString grName;
        if(ui->radioButtonWithoutGroup->isChecked()) grName="";
        if(ui->radioButtonNewGroup->isChecked()) grName=ui->lineEditNewGroup->text();
        if(ui->radioButtonExistGroup->isChecked()) grName=ui->comboBoxGroups->currentText();
        var->setGroupName(grName);
        var->setMemAddress(ui->spinBoxMemAddress->value());
        var->setPlcAddress(ui->spinBoxNetAddress->value());
        var->setName(ui->lineEditName->text());
        var->setType(ui->comboBoxVarsType->currentText());
        var->setSign(ui->checkBoxSign->isChecked());
        this->accept();
    }
}



void VarEditDialog::on_radioButtonExistGroup_toggled(bool checked)
{
    ui->comboBoxGroups->setEnabled(checked);
}
