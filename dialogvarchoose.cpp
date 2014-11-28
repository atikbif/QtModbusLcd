#include "dialogvarchoose.h"
#include "ui_dialogvarchoose.h"
#include "modbusvar.h"
#include <QTreeWidgetItem>
#include <QRegExp>

DialogVarChoose::DialogVarChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVarChoose)
{
    ui->setupUi(this);
    vars = nullptr;
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(treeClicked(QTreeWidgetItem*)));
    QRegExp rx("[*#]{1,10}[.,]?[*#]{0,10}");
    ui->lineEditPattern->setValidator(new QRegExpValidator (rx,this));
}

DialogVarChoose::~DialogVarChoose()
{
    delete ui;
}

void DialogVarChoose::generateTree()
{
    if(vars!=nullptr)
    {
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
}

QString DialogVarChoose::getPattern()
{
    return ui->lineEditPattern->text();
}

QString DialogVarChoose::getName()
{
    return ui->lineEditName->text();
}

void DialogVarChoose::setName(const QString &vName)
{
    bool found = false;
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
                    found = true;
                }
            }
        }
        else
        {
            if(item->text(0)==vName)
            {
                item->setSelected(true);
                found = true;
            }
        }
    }
    if(found)
    {
        ui->lineEditName->setText(vName);
        ui->lineEditName->setCursorPosition(0);
        ui->lineEditName->setToolTip(vName);

        for(int i=0;i<vars->getAllVars().count();i++)
        {
            if(vars->getAllVars().at(i)->getName() == vName)
            {
                QString vComment = vars->getAllVars().at(i)->getComment();
                ui->lineEditComment->setText(vComment);
                ui->lineEditComment->setCursorPosition(0);
                ui->lineEditComment->setToolTip(vComment);
                break;
            }
        }
    }

}

void DialogVarChoose::setPattern(const QString &vPattern)
{
    ui->lineEditPattern->setText(vPattern);
}

void DialogVarChoose::treeClicked(QTreeWidgetItem *ptr)
{
    QString itemName = ptr->text(0);
    modbusVar *var = vars->getVar(itemName);
    if(var != nullptr)
    {
        ui->lineEditName->setText(var->getName());
        ui->lineEditName->setCursorPosition(0);
        ui->lineEditName->setToolTip(var->getName());

        ui->lineEditComment->setText(var->getComment());
        ui->lineEditComment->setCursorPosition(0);
        ui->lineEditComment->setToolTip(var->getComment());

        if(ui->lineEditPattern->text().isEmpty()) ui->lineEditPattern->setText("***");
    }
    else
    {
        ui->lineEditComment->setText("");
        ui->lineEditName->setText("");
        //ui->lineEditPattern->setText("");
    }
}
