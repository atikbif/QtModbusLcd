#include "requestscreator.h"
#include <QString>
#include <QVector>
#include <QMultiMap>


void RequestsCreator::makeHoldReqs(ListOfVars *vars, ListOfRequests *reqs)
{
    makeRequest(vars,reqs,"HOLDING REGISTERS","HOLD REGS",2);
}

void RequestsCreator::makeInpReqs(ListOfVars *vars, ListOfRequests *reqs)
{
    makeRequest(vars,reqs,"INPUT REGISTERS","INP REGS",2);
}

void RequestsCreator::makeCoilReqs(ListOfVars *vars, ListOfRequests *reqs)
{
    makeRequest(vars,reqs,"COIL","COILS",5);
}

void RequestsCreator::makeDiReqs(ListOfVars *vars, ListOfRequests *reqs)
{
    makeRequest(vars,reqs,"DISCRETE INPUTS","DINS",5);
}

QVector<modbusVar *> RequestsCreator::getVarsByType(ListOfVars *vars, const QString &typeVars)
{
    // holding registers search
    QVector<modbusVar*> varsByType;
    QVector<QString> varTypes = vars->getTypes();
    foreach (QString vType, varTypes)
    {
        if(vType.contains(typeVars))
        {
           varsByType  += vars->getVarsByType(vType);
        }
    }
    return varsByType;
}

void RequestsCreator::getMinMaxMemAddresses(QList<modbusVar *> mVars, quint16 &minAddr, quint16 &maxAddr)
{
    if(mVars.count())
    {
        minAddr = mVars[0]->getMemAddress();
        maxAddr = minAddr;
        foreach(modbusVar* v, mVars)
        {
            int addition = 0; // for double regs = 1
            if(v->getMemAddress() < minAddr) minAddr = v->getMemAddress();
            if(v->getType().contains("DOUBLE")) addition = 1;
            if(v->getMemAddress()+addition > maxAddr) maxAddr = v->getMemAddress() + addition;
        }
    }
}

QVector<bool> RequestsCreator::getTableOfUsableVars(QList<modbusVar *> mVars, const quint16 &minAddr, const quint16 &maxAddr)
{
    quint16 cellsCount = maxAddr - minAddr + 1;
    QVector<bool> cellsTable;
    cellsTable.fill(false,cellsCount);
    // fill cells table
    foreach (modbusVar* v, mVars)
    {
        quint8 vLength = 1;
        if(v->getType().contains("DOUBLE")) vLength = 2;
        quint16 cellNum = v->getMemAddress() - minAddr;
        cellsTable.replace(cellNum,true);
        if(vLength==2) cellsTable.replace(cellNum+1,true);
    }
    return cellsTable;
}

void RequestsCreator::createReqs(QVector<bool> isVarUsable, const quint16 minAddr, const quint16 netAddr, const QString &typeReq, const quint16 maxEmptySpace,ListOfRequests *reqs)
{
    bool newReq = false;
    quint16 startAddress = 0;
    quint16 regCount = 0;
    quint16 holeCnt = 0;
    for(int j=0;j<maxEmptySpace+1;j++) isVarUsable.append(false);
    for(int j=0;j<isVarUsable.count();j++)
    {
        if(newReq)
        {
            if(isVarUsable.at(j))
            {
                holeCnt = 0;
                regCount++;
            }
            else
            {
                holeCnt++;regCount++;
                if(holeCnt>maxEmptySpace)
                {
                    // createRequest
                    reqs->addRequest(typeReq,netAddr,regCount-holeCnt,startAddress);
                    newReq = 0;
                    holeCnt = 0;
                }
            }
        }
        else
        {
            if(isVarUsable.at(j))
            {
                newReq = true;
                startAddress = minAddr + j;
                regCount = 1;
                holeCnt = 0;
            }
        }
    }
}

void RequestsCreator::makeRequest(ListOfVars *vars, ListOfRequests *reqs, const QString &varType, const QString &reqType, const quint16 maxFreeSpace)
{
    QVector<modbusVar*> varsOfType = getVarsByType(vars,varType);
    if(varsOfType.count())
    {
        QVector<int> plcAddresses;
        // create lists by net adresses
        QMultiMap<quint16,modbusVar*> netVars;
        foreach(modbusVar* var, varsOfType)
        {
            quint16 netAddress = var->getPlcAddress();
            if(!netVars.contains(netAddress)) plcAddresses+=netAddress;
            netVars.insert(netAddress,var);
        }
        for(int i=0;i<plcAddresses.count();i++)
        {
            QList<modbusVar*> vList = netVars.values(plcAddresses.at(i));
            if(vList.count())
            {
                quint16 minAddress = vList.at(0)->getMemAddress();
                quint16 maxAddress = minAddress;
                getMinMaxMemAddresses(vList,minAddress,maxAddress);
                QVector<bool> cellsTable = getTableOfUsableVars(vList,minAddress,maxAddress);
                createReqs(cellsTable,minAddress,plcAddresses.at(i),reqType,maxFreeSpace,reqs);
            }
        }
    }
}

RequestsCreator::RequestsCreator()
{
}

void RequestsCreator::makeRequests(ListOfVars *vars, ListOfRequests *reqs)
{
    if((vars!=nullptr)&&(reqs!=nullptr))
    {
        QVector<QString> varTypes = vars->getTypes();
        if(varTypes.count())
        {
            makeHoldReqs(vars, reqs);
            makeInpReqs(vars, reqs);
            makeCoilReqs(vars, reqs);
            makeDiReqs(vars, reqs);
        }
    }
}
