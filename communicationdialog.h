#ifndef COMMUNICATIONDIALOG_H
#define COMMUNICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CommunicationDialog;
}

class CommunicationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommunicationDialog(QWidget *parent = 0);
    ~CommunicationDialog();

private:
    Ui::CommunicationDialog *ui;

public slots:
    void updatePercent(float value);
    void setMessage(const QString &message);
};

#endif // COMMUNICATIONDIALOG_H
