#ifndef REQUESTSCREATOR_H
#define REQUESTSCREATOR_H

#include "listofvars.h"
#include "modbusvar.h"
#include "listofrequests.h"
#include <QVector>

class RequestsCreator
{
    static void makeHoldReqs(ListOfVars* vars,ListOfRequests* reqs);
    static void makeInpReqs(ListOfVars* vars,ListOfRequests* reqs);
    static void makeCoilReqs(ListOfVars* vars,ListOfRequests* reqs);
    static void makeDiReqs(ListOfVars* vars,ListOfRequests* reqs);
    static QVector<modbusVar*> getVarsByType(ListOfVars *vars,const QString& typeVars);
    static void getMinMaxMemAddresses(QList<modbusVar*> mVars,quint16 &minAddr, quint16 &maxAddr);
    static QVector<bool> getTableOfUsableVars(QList<modbusVar*> mVars,const quint16 &minAddr, const quint16 &maxAddr);
    static void createReqs(QVector<bool> isVarUsable,const quint16 minAddr,const quint16 netAddr, const QString& typeReq,const quint16 maxEmptySpace,ListOfRequests *reqs);
    static void makeRequest(ListOfVars* vars,ListOfRequests* reqs,const QString& varType, const QString& reqType, const quint16 maxFreeSpace);
public:
    RequestsCreator();
    static void makeRequests(ListOfVars* vars,ListOfRequests* reqs);
};

#endif // REQUESTSCREATOR_H
