#include "listofrequests.h"

ListOfRequests::ListOfRequests()
{
}

void ListOfRequests::addRequest(QString typeR, quint16 netAddr, quint16 amountD, quint16 memAddr)
{
    netAddress.insert(typeR,netAddr);
    amountOfData.insert(typeR,amountD);
    memAddress.insert(typeR,memAddr);
}

quint16 ListOfRequests::getAmountOfReqByType(QString typeR)
{
    quint16 res=0;
    if(netAddress.contains(typeR)) res = netAddress.count(typeR);
    return res;
}

quint16 ListOfRequests::getNetAddress(QString typeR, quint16 num)
{
    quint16 res=0;
    if(netAddress.contains(typeR))
    {
        if(netAddress.count(typeR)>num) res = netAddress.values(typeR).at(num);
    }
    return res;
}

quint16 ListOfRequests::getAmount(QString typeR, quint16 num)
{
    quint16 res=0;
    if(amountOfData.contains(typeR))
    {
        if(amountOfData.count(typeR)>num) res = amountOfData.values(typeR).at(num);
    }
    return res;
}

quint16 ListOfRequests::getMemAddress(QString typeR, quint16 num)
{
    quint16 res=0;
    if(memAddress.contains(typeR))
    {
        if(memAddress.count(typeR)>num) res = memAddress.values(typeR).at(num);
    }
    return res;
}

