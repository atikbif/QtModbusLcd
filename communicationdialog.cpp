#include "communicationdialog.h"
#include "ui_communicationdialog.h"

CommunicationDialog::CommunicationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationDialog)
{
    ui->setupUi(this);
}

CommunicationDialog::~CommunicationDialog()
{
    delete ui;
}

void CommunicationDialog::updatePercent(float value)
{
    ui->progressBar->setValue(value);
}

void CommunicationDialog::setMessage(const QString &message)
{
    ui->lineEdit->setText(message);
}
