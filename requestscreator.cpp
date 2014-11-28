#include "requestscreator.h"
#include <QString>
#include <QVector>
#include <QMultiMap>


RequestsCreator::RequestsCreator()
{
}

void RequestsCreator::makeRequests(ListOfVars *vars, ListOfRequests *reqs)
{
    const unsigned short maxHole = 2;
    if((vars!=nullptr)&&(reqs!=nullptr))
    {
        QVector<QString> varTypes = vars->getTypes();
        if(varTypes.count())
        {
            // holding registers search
            QVector<modbusVar*> vHold;
            foreach (QString vType, varTypes)
            {
                if(vType.contains("HOLDING REGISTERS"))
                {
                   vHold  += vars->getVarsByType(vType);
                }
            }
            if(vHold.count())
            {
                QVector<int> plcAddresses;
                // create lists by net adresses
                QMultiMap<quint16,modbusVar*> netVars;
                foreach(modbusVar* var, vHold)
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
                        foreach(modbusVar* v, vList)
                        {
                            int addition = 0; // for double regs = 1
                            if(v->getMemAddress() < minAddress) minAddress = v->getMemAddress();
                            if(v->getType().contains("DOUBLE")) addition = 1;
                            if(v->getMemAddress()+addition > maxAddress) maxAddress = v->getMemAddress() + addition;
                        }
                        quint16 cellsCount = maxAddress - minAddress + 1;
                        QVector<bool> cellsTable;
                        cellsTable.fill(false,cellsCount);
                        // fill cells table
                        foreach (modbusVar* v, vList)
                        {
                            quint8 vLength = 1;
                            if(v->getType().contains("DOUBLE")) vLength = 2;
                            quint16 cellNum = v->getMemAddress() - minAddress;
                            cellsTable.replace(cellNum,true);
                            if(vLength==2) cellsTable.replace(cellNum+1,true);
                        }
                        bool newReq = false;
                        quint16 startAddress = 0;
                        quint16 regCount = 0;
                        quint16 holeCnt = 0;
                        for(int j=0;j<maxHole+1;j++) cellsTable.append(false);
                        for(int j=0;j<cellsTable.count();j++)
                        {
                            if(newReq)
                            {
                                if(cellsTable.at(j))
                                {
                                    holeCnt = 0;
                                    regCount++;
                                }
                                else
                                {
                                    holeCnt++;regCount++;
                                    if(holeCnt>maxHole)
                                    {
                                        // createRequest
                                        reqs->addRequest("HOLD REGS",plcAddresses.at(i),regCount-holeCnt,startAddress);
                                        newReq = 0;
                                        holeCnt = 0;
                                    }
                                }
                            }
                            else
                            {
                                if(cellsTable.at(j))
                                {
                                    newReq = true;
                                    startAddress = minAddress + j;
                                    regCount = 1;
                                    holeCnt = 0;
                                }
                            }
                        }
                    }
                }
            }

            // input registers search
            QVector<modbusVar*> vInput;
            foreach (QString vType, varTypes)
            {
                if(vType.contains("INPUT REGISTERS"))
                {
                   vInput  += vars->getVarsByType(vType);
                }
            }
            if(vInput.count())
            {
                QVector<int> plcAddresses;
                // create lists by net adresses
                QMultiMap<quint16,modbusVar*> netVars;
                foreach(modbusVar* var, vInput)
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
                        foreach(modbusVar* v, vList)
                        {
                            int addition = 0; // for double regs = 1
                            if(v->getMemAddress() < minAddress) minAddress = v->getMemAddress();
                            if(v->getType().contains("DOUBLE")) addition = 1;
                            if(v->getMemAddress()+addition > maxAddress) maxAddress = v->getMemAddress() + addition;
                        }
                        quint16 cellsCount = maxAddress - minAddress + 1;
                        QVector<bool> cellsTable;
                        cellsTable.fill(false,cellsCount);
                        // fill cells table
                        foreach (modbusVar* v, vList)
                        {
                            quint8 vLength = 1;
                            if(v->getType().contains("DOUBLE")) vLength = 2;
                            quint16 cellNum = v->getMemAddress() - minAddress;
                            cellsTable.replace(cellNum,true);
                            if(vLength==2) cellsTable.replace(cellNum+1,true);
                        }
                        bool newReq = false;
                        quint16 startAddress = 0;
                        quint16 regCount = 0;
                        quint16 holeCnt = 0;
                        for(int j=0;j<maxHole+1;j++) cellsTable.append(false);
                        for(int j=0;j<cellsTable.count();j++)
                        {
                            if(newReq)
                            {
                                if(cellsTable.at(j))
                                {
                                    holeCnt = 0;
                                    regCount++;
                                }
                                else
                                {
                                    holeCnt++;regCount++;
                                    if(holeCnt>maxHole)
                                    {
                                        // createRequest
                                        reqs->addRequest("INP REGS",plcAddresses.at(i),regCount-holeCnt,startAddress);
                                        newReq = 0;
                                        holeCnt = 0;
                                    }
                                }
                            }
                            else
                            {
                                if(cellsTable.at(j))
                                {
                                    newReq = true;
                                    startAddress = minAddress + j;
                                    regCount = 1;
                                    holeCnt = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
