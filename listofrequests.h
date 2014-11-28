#ifndef LISTOFREQUESTS_H
#define LISTOFREQUESTS_H

#include <QList>
#include <QMultiMap>
#include <QString>
#include "modbusvar.h"

class ListOfRequests
{
    //typeOfRequest - the key
    QMultiMap<QString, quint16> netAddress;
    QMultiMap<QString, quint16> amountOfData;
    QMultiMap<QString, quint16> memAddress;

public:
    ListOfRequests();
    void addRequest(QString typeR, quint16 netAddr, quint16 amountD, quint16 memAddr);
    quint16 getAmountOfReqByType(QString typeR);
    quint16 getNetAddress(QString typeR, quint16 num);
    quint16 getAmount(QString typeR, quint16 num);
    quint16 getMemAddress(QString typeR, quint16 num);
};

#endif // LISTOFREQUESTS_H
