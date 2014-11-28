#ifndef VAREDITDIALOG_H
#define VAREDITDIALOG_H

#include <QDialog>
#include "modbusvar.h"
#include <QStringList>

namespace Ui {
class VarEditDialog;
}

class VarEditDialog : public QDialog
{
    Q_OBJECT
    modbusVar* var;
    QStringList groups;
    QStringList varsName;
    QString startName;
    bool newVar;
public:
    explicit VarEditDialog(QWidget *parent = 0);
    ~VarEditDialog();
    void setVar(modbusVar* ptr);
    void setGroups(const QStringList &list);
    void setVarsNames(const QStringList &list);
    void setNewVar(bool value) {newVar = value;}
private slots:
    void on_pushButtonOK_clicked();

    void on_radioButtonExistGroup_toggled(bool checked);

private:
    Ui::VarEditDialog *ui;
};

#endif // VAREDITDIALOG_H
