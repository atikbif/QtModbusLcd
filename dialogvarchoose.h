#ifndef DIALOGVARCHOOSE_H
#define DIALOGVARCHOOSE_H

#include <QDialog>
#include "listofvars.h"
#include <QTreeWidgetItem>
#include <QString>

namespace Ui {
class DialogVarChoose;
}

class DialogVarChoose : public QDialog
{
    Q_OBJECT

    ListOfVars *vars;

public:
    explicit DialogVarChoose(QWidget *parent = 0);
    ~DialogVarChoose();
    void setVars(ListOfVars *ptr) {vars = ptr;}
    void generateTree(void);
    QString getPattern(void);
    QString getName(void);
    void setName(const QString &vName);
    void setPattern(const QString &vPattern);
private:
    Ui::DialogVarChoose *ui;
private slots:
    void treeClicked(QTreeWidgetItem *ptr);
};

#endif // DIALOGVARCHOOSE_H
