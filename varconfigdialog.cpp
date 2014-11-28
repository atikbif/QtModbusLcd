#include "varconfigdialog.h"
#include "ui_varconfigdialog.h"
#include <QStringList>
#include <QColor>
#include "vareditdialog.h"
#include <QMessageBox>


void VarConfigDialog::updateTreeOfVars()
{
    // root variables
    ui->treeWidget->clear();
    QVector<modbusVar*> rootVars = vars->getRootVars();
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem*> itemsList;
    foreach(modbusVar* var, rootVars)
    {
        if(var != nullptr)
        {
            QStringList sList;
            sList += var->getName();
            QTreeWidgetItem* item = new QTreeWidgetItem(sList);
            itemsList += item;
        }
    }

    // sort items

    ui->treeWidget->addTopLevelItems(itemsList);

    itemsList.clear();
    QVector<QString> grNames = vars->getListOfGroups();

    // sort groups

    QVector<modbusVar*> grVars;
    for(int i=0;i<grNames.count();i++)
    {
        grVars.clear();
        if(!grNames[i].isEmpty())
        {
            grVars = vars->getVarsInGroup(grNames[i]);
        }
        QStringList sList;
        sList += grNames[i];
        QTreeWidgetItem* item = new QTreeWidgetItem(sList);
        item->setTextColor(0, QColor("darkGreen"));
        ui->treeWidget->addTopLevelItem(item);
        if(grVars.count())
        {
            // sort vars

            for(int j=0;j<grVars.count();j++)
            {
                QStringList sList;
                sList += grVars[j]->getName();
                QTreeWidgetItem* son = new QTreeWidgetItem(item,sList);
                ui->treeWidget->addTopLevelItem(son);
            }
        }
    }
}

void VarConfigDialog::selectVar(const QString &vName)
{
    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
        if(item->childCount())
        {
            for(int j=0;j<item->childCount();j++)
            {
                if(item->child(j)->text(0)==vName)
                {
                    ui->treeWidget->expandItem(item);
                    item->child(j)->setSelected(true);
                    on_treeWidget_itemClicked(item->child(j),0);
                }
            }
        }
        else
        {
            if(item->text(0)==vName)
            {
                item->setSelected(true);
                on_treeWidget_itemClicked(item,0);
            }
        }
    }
}

void VarConfigDialog::expandGroup(const QString &grName)
{
    for(int i=0;i<ui->treeWidget->topLevelItemCount();i++)
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
        if(item->text(0) == grName)
        {
            ui->treeWidget->expandItem(item);
            item->setSelected(true);
        }
    }
}

VarConfigDialog::VarConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VarConfigDialog)
{
    ui->setupUi(this);

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::darkBlue);
    ui->lineEditName->setPalette(*palette);

//    ui->lineEditName->setStyleSheet("QLabel { color : darkBlue; }");
    ui->lineEditComment->setPalette(*palette);
    ui->lineEditNetAddress->setPalette(*palette);
    ui->lineEditAdd->setPalette(*palette);
    ui->lineEditCoeff->setPalette(*palette);
    ui->lineEditType->setPalette(*palette);
    ui->lineEditEdit->setPalette(*palette);
    ui->lineEditMemAddr->setPalette(*palette);
    ui->lineEditSign->setPalette(*palette);
    ui->DelVarPushButton->setEnabled(false);
    delete palette;
}

void VarConfigDialog::setVars(ListOfVars *ptr)
{
    vars = ptr;
}

VarConfigDialog::~VarConfigDialog()
{
    delete ui;
}

void VarConfigDialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    QString itemName;
    modbusVar* var;

    if(item!=nullptr)
    {
        itemName = item->text(0);
        var = vars->getVar(itemName);
    }else var = nullptr;
    if(var!=nullptr)
    {
        ui->lineEditName->setText(var->getName());
        ui->lineEditComment->setText(var->getComment());
        ui->lineEditComment->setToolTip(var->getComment());
        ui->lineEditNetAddress->setText(QString::number(var->getPlcAddress()));
        ui->lineEditAdd->setText(QString::number(var->getAddion()));
        ui->lineEditCoeff->setText(QString::number(var->getMultiplier()));
        ui->lineEditType->setText(var->getType());
        ui->lineEditEdit->setText(QString(var->isItEditable()?"Разрешено":"Запрещено"));
        ui->lineEditMemAddr->setText("0x" + QString::number(var->getMemAddress(),16) + "     (" + QString::number(var->getMemAddress()) + ")");
        ui->lineEditMemAddr->setToolTip("шестнадцатеричное и десятичное значения");
        ui->pushButtonEditVar->setEnabled(true);
        ui->DelVarPushButton->setEnabled(true);
        ui->lineEditSign->setText(var->isVarSign()?"Разрешено":"Запрещено");
        update();
    }
    else
    {
        ui->lineEditName->setText("");
        ui->lineEditComment->setText("");
        ui->lineEditNetAddress->setText("");
        ui->lineEditAdd->setText("");
        ui->lineEditCoeff->setText("");
        ui->lineEditType->setText("");
        ui->lineEditEdit->setText("");
        ui->lineEditMemAddr->setText("");
        ui->pushButtonEditVar->setEnabled(false);
        ui->DelVarPushButton->setEnabled(false);
        ui->lineEditSign->setText("");
        update();
    }
}

void VarConfigDialog::on_pushButtonEditVar_clicked()
{

    modbusVar* ptr =  vars->getVar(ui->lineEditName->text());
    if(ptr!=nullptr)
    {
        VarEditDialog* conf = new VarEditDialog();
        QStringList list;
        QVector<QString> groups = vars->getListOfGroups();
        foreach (QString grName, groups) {list += grName;}
        conf->setGroups(list);
        list.clear();
        foreach(modbusVar* v, vars->getAllVars())
        {
            list += v->getName();
        }
        conf->setVarsNames(list);
        conf->setVar(ptr);


        conf->exec();
        delete conf;
        updateTreeOfVars();
        selectVar(ptr->getName());
    }


}

void VarConfigDialog::on_DelVarPushButton_clicked()
{
    QString grName;
    if(!ui->lineEditName->text().isEmpty())
    {
        if(QMessageBox::question(this,"Подтверждение","Удалить переменную?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        {
            modbusVar* v = vars->getVar(ui->lineEditName->text());
            if(v!=nullptr)
            {
                grName = v->getGroupName();
                vars->deleteVar(v->getName());
                on_treeWidget_itemClicked(nullptr,0);
                updateTreeOfVars();
                if(!grName.isEmpty()) expandGroup(grName);
                delete v;
            }
        }
    }
}

void VarConfigDialog::on_AddVarPushButton_clicked()
{
    modbusVar* var = new modbusVar();
    var->setComment("");
    var->setCorrection(1,0);
    var->setEditable(false);
    var->setGroupName("");
    var->setMemAddress(0);
    var->setPlcAddress(1);
    var->setName("");

    VarEditDialog* conf = new VarEditDialog();
    QStringList list;
    QVector<QString> groups = vars->getListOfGroups();
    foreach (QString grName, groups) {list += grName;}
    conf->setGroups(list);
    list.clear();
    foreach(modbusVar* v, vars->getAllVars())
    {
        list += v->getName();
    }
    conf->setVarsNames(list);
    conf->setVar(var);
    conf->setNewVar(true);

    if(conf->exec()==QDialog::Accepted)
    {
        vars->addVar(var);
        updateTreeOfVars();
        selectVar(var->getName());
    }
    delete conf;
}
