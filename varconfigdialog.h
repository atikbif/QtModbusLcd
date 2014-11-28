#ifndef VARCONFIGDIALOG_H
#define VARCONFIGDIALOG_H

#include <QDialog>
#include "listofvars.h"
#include <QTreeWidgetItem>

namespace Ui {
class VarConfigDialog;
}

class VarConfigDialog : public QDialog
{
    Q_OBJECT
    ListOfVars* vars;
    void selectVar(const QString &vName);
    void expandGroup(const QString &grName);

public:
    explicit VarConfigDialog(QWidget *parent = 0);
    void updateTreeOfVars(void);
    void setVars(ListOfVars* ptr);
    ~VarConfigDialog();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButtonEditVar_clicked();

    void on_DelVarPushButton_clicked();

    void on_AddVarPushButton_clicked();

private:
    Ui::VarConfigDialog *ui;
};

#endif // VARCONFIGDIALOG_H
