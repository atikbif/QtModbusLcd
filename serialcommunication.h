#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPort>
#include "displaydata.h"
#include "epsonfont.h"
#include "listofvars.h"
#include <QStringList>

class SerialCommunication : public QThread
{
    Q_OBJECT
public:
    explicit SerialCommunication(QObject *parent = 0);
    ~SerialCommunication();
    void searchController(void);
    void writeProject(void);
    void readProject(void);
    void stopWork(void);
    void run();
    void setData(DisplayData* dataPtr);
    void setFont(EpsonFont* fontPtr);
    void setVars(ListOfVars* listVars) {lVars = listVars;}
    void setStrNumVars(QStringList vNames);
signals:
    void searchStep(float currentStage);
    void searchError(const QString &errMessage);
    void searchOK(const QString &okMessage);
    void writeStep(float currentStage);
    void writeOK(const QString &okMessage);
    void writeError(const QString &errMessage);
public slots:
private:
    int getVarsAmountOnLcd(void);
    ListOfVars* lVars;
    DisplayData* dataPtr;
    EpsonFont* fontPtr;
    quint16 getCRC(const QByteArray& data);
    QString portName;
    QString portSpeed;
    QString plcVersion;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    bool stop;
    bool startSearch,searchCmd;
    bool startWrite,writeCmd;
    QString savePortName;
    quint32 savePortBaud;
    QStringList strNumVars;
};

#endif // SERIALCOMMUNICATION_H
