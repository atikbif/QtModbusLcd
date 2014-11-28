#include "serialcommunication.h"
#include <QSerialPort>
#include <QList>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QString>
#include<QStringList>
#include "lineofsymbols.h"
#include "requestscreator.h"


const unsigned char crc16H[256]={
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ;

/* Table of CRC values for low-order byte */

const unsigned char crc16L[256]={
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;

SerialCommunication::SerialCommunication(QObject *parent) :
    QThread(parent)
{
    startSearch = false;searchCmd = false;
    quit = false;
    savePortName = "";
    savePortBaud = 19200;
    dataPtr = nullptr;
    fontPtr = nullptr;
    lVars = nullptr;
}

SerialCommunication::~SerialCommunication()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void SerialCommunication::searchController()
{
    searchCmd = true;
    if (!isRunning()) start(); else cond.wakeOne();
}

void SerialCommunication::writeProject()
{
    if(savePortName.isEmpty())
    {
        emit writeError("Не указан порт для программирования");
        return;
    }
    writeCmd = true;
    if(!isRunning()) start(); else cond.wakeOne();
}

void SerialCommunication::readProject()
{

}

void SerialCommunication::stopWork()
{

}

void SerialCommunication::run()
{
    mutex.lock();
    if(searchCmd) {searchCmd = false;startSearch = true;}
    if(writeCmd) {writeCmd = false;startWrite = true;}
    mutex.unlock();
    while (!quit)
    {
        if(startSearch)
        {
            bool searchIsSuccessful = false;
            startSearch = false;
            QList<QSerialPortInfo> pList = QSerialPortInfo::availablePorts();
            if(pList.count())
            {
                quint16 currentStep = 0;
                foreach(QSerialPortInfo pInfo, pList)
                {
                    QSerialPort serial;
                    serial.setPortName(pInfo.portName());
                    if (serial.open(QIODevice::ReadWrite))
                    {
                        serial.setBaudRate(QSerialPort::Baud19200);
                        serial.setParity(QSerialPort::NoParity);
                        serial.setStopBits(QSerialPort::OneStop);
                        serial.setFlowControl(QSerialPort::NoFlowControl);
                        serial.setDataBits(QSerialPort::Data8);

                        QByteArray req;
                        req += 0x01;
                        req += 0xA0;
                        quint16 crc = getCRC(req);
                        req += crc >> 8;
                        req += crc & 0xFF;

                        serial.write(req);
                        serial.waitForBytesWritten(50);

                        if (serial.waitForReadyRead(100))
                        {
                            QByteArray responseData = serial.readAll();
                            while (serial.waitForReadyRead(10)) responseData += serial.readAll();
                            crc = getCRC(responseData);
                            quint16 respLength = responseData.count();
                            if((crc==0)&&(respLength>4))   // if answer is correct
                            {
                                QString curVersion="";
                                for(int i=2;i<respLength-2;i++) curVersion+=responseData.at(i);
                                emit searchOK("Пульт обнаружен. Порт: " + serial.portName() + " 19200 бит/с (" + curVersion + ")");
                                searchIsSuccessful = true;
                                savePortName = serial.portName();
                                savePortBaud = serial.baudRate();
                                serial.close();
                                writeProject();
                                break;
                            }
                        }
                        serial.close();
                        currentStep++;
                        emit searchStep(((float)currentStep/pList.count())*100);
                    }
                }
                if(!searchIsSuccessful) emit searchError("Устройство не обнаружено");
            }else emit searchError("Не обнаружено свободных COM портов");
        }
        mutex.lock();
        if(searchCmd) {searchCmd = false;startSearch = true;}
        if(writeCmd) {writeCmd = false;startWrite = true;}
        mutex.unlock();
        if(startWrite)
        {
            emit writeStep(0);
            sleep(2);
            emit writeOK("Запись проекта в пульт");
            bool wrError = false;
            startWrite = false;
            QSerialPort serial;
            serial.setPortName(savePortName);
            if (serial.open(QIODevice::ReadWrite))
            {
                serial.setBaudRate(savePortBaud);
                serial.setParity(QSerialPort::NoParity);
                serial.setStopBits(QSerialPort::OneStop);
                serial.setFlowControl(QSerialPort::NoFlowControl);
                serial.setDataBits(QSerialPort::Data8);
                // erase pages

                const quint16 reqByteCount = 64;
                const quint16 pageSize = 1024;



                if(dataPtr!=nullptr)
                {
                    quint16 rowCount = dataPtr->getRowCount();
                    QList<quint16> linesCount;
                    quint32 wrBufSize=0;

                    // create requests
                    ListOfRequests reqL;
                    quint16 holdReqCnt=0;
                    quint16 inpReqCnt=0;
                    quint16 coilReqCnt=0;
                    quint16 dinReqCnt=0;
                    quint16 reqCnt=0;
                    if(lVars != nullptr)
                    {
                        RequestsCreator::makeRequests(lVars,&reqL);
                        holdReqCnt = reqL.getAmountOfReqByType("HOLD REGS");
                        inpReqCnt = reqL.getAmountOfReqByType("INP REGS");
                        coilReqCnt = reqL.getAmountOfReqByType("COILS");
                        dinReqCnt = reqL.getAmountOfReqByType("DINS");
                        reqCnt = holdReqCnt + inpReqCnt + coilReqCnt + dinReqCnt;
                        wrBufSize += reqCnt*6; // 6 bytes for each request
                    }

                    for(int i=0;i<rowCount;i++)
                    {
                        linesCount += dataPtr->getCountOfLinesInRow(i);
                        wrBufSize += 20*linesCount.at(i);
                    }

                    // all vars description
                    const unsigned char vSize = 8; // count of bytes for each variable
                    wrBufSize += vSize * lVars->getAllVars().count();

                    // vars on lcd
                    const unsigned char vSizeLcd = 7;
                    wrBufSize += vSizeLcd * getVarsAmountOnLcd();



                    quint16 pageCount = wrBufSize/pageSize;
                    if(wrBufSize % pageSize) pageCount++;
                    pageCount++;    // for initial data

                    QByteArray wrData;
                    wrData.resize(pageCount * pageSize);
                    wrData[0] = 1;
                    wrData[1] = 0;  // version of data structure
                    wrData[2] = rowCount;
                    wrData[3] = 0;
                    for(int i=0;i<rowCount;i++)
                    {
                        wrData[4+i*2] = linesCount[i] & 0xFF;
                        wrData[5+i*2] = linesCount[i] >> 8;
                    }
                    // count of variables
                    wrData[4+rowCount*2] = lVars->getAllVars().count() & 0XFF;
                    wrData[5+rowCount*2] = lVars->getAllVars().count() >> 8;
                    wrData[6+rowCount*2] = vSize;
                    wrData[7+rowCount*2] = 0;

                    // amount of requests
                    wrData[8+rowCount*2] = reqCnt & 0xFF;
                    wrData[9+rowCount*2] = reqCnt >> 8;

                    quint32 bufPos = pageSize*1;
                    for(int i=0;i<rowCount;i++)
                        for(int j=0;j<linesCount[i];j++)
                        {
                            if(fontPtr!=nullptr)
                            {
                                for(int k=0;k<20;k++)
                                {
                                    quint16 s = dataPtr->getLine(i,j)->getSymbol(k);
                                    s = fontPtr->correctionCode(s);
                                    if(s==0) s=0x20;
                                    wrData[bufPos++]=s;
                                }
                            }
                        }
                    // add requests data
                    for(int i=0;i<holdReqCnt;i++)
                    {
                        quint16 tmp;
                        tmp = reqL.getNetAddress("HOLD REGS",i);
                        wrData[bufPos++] = 0x01;        // type of request
                        wrData[bufPos++] = tmp & 0xFF;
                        tmp = reqL.getAmount("HOLD REGS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                        tmp = reqL.getMemAddress("HOLD REGS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                    }
                    for(int i=0;i<inpReqCnt;i++)
                    {
                        quint16 tmp;
                        tmp = reqL.getNetAddress("INP REGS",i);
                        wrData[bufPos++] = 0x02;        // type of request
                        wrData[bufPos++] = tmp & 0xFF;
                        tmp = reqL.getAmount("INP REGS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                        tmp = reqL.getMemAddress("INP REGS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                    }
                    for(int i=0;i<coilReqCnt;i++)
                    {
                        quint16 tmp;
                        tmp = reqL.getNetAddress("COILS",i);
                        wrData[bufPos++] = 0x03;        // type of request
                        wrData[bufPos++] = tmp & 0xFF;
                        tmp = reqL.getAmount("COILS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                        tmp = reqL.getMemAddress("COILS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                    }
                    for(int i=0;i<dinReqCnt;i++)
                    {
                        quint16 tmp;
                        tmp = reqL.getNetAddress("DINS",i);
                        wrData[bufPos++] = 0x04;        // type of request
                        wrData[bufPos++] = tmp & 0xFF;
                        tmp = reqL.getAmount("DINS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                        tmp = reqL.getMemAddress("DINS",i);
                        wrData[bufPos++] = tmp & 0xFF;
                        wrData[bufPos++] = tmp >> 8;
                    }

                    // all variables


                    int vNum = 0;
                    foreach(modbusVar* v,lVars->getAllVars())
                    {
                        wrData[bufPos] = vNum >> 8; // index in the list
                        wrData[bufPos+1] = vNum & 0xFF;
                        wrData[bufPos+2] = v->getPlcAddress();
                        wrData[bufPos+3] = v->getTypeCode();
                        wrData[bufPos+4] = v->getMemAddress() & 0xFF;
                        wrData[bufPos+5] = v->getMemAddress() >> 8;
                        wrData[bufPos+6] = 0; //multiplier - * 10^(-n)
                        wrData[bufPos+7] = (v->isItEditable()?0x01:0) | (v->isVarSign()?0x02:0);
                        vNum++;
                        bufPos+= vSize;
                    }

                    // variables in lines
                    unsigned int varsOnDisplAmount = 0;     // amount of vars on a display
                    unsigned int lcdVarsPos = bufPos;       // position of vars' amount in buf
                    bufPos += 2;

                    for(int i=0;i<rowCount;i++)
                        for(int j=0;j<linesCount[i];j++)
                        {
                            for(int k=0;k<dataPtr->getLine(i,j)->getLength();k++)
                            {
                                if(dataPtr->getLine(i,j)->isBeginOfVarHere(k))
                                {
                                    QString vName = dataPtr->getLine(i,j)->getVar(k);
                                    if(!vName.isEmpty())
                                    {
                                        // add Variable's information
                                        // a format in a string and an index in the list
                                        varsOnDisplAmount++;
                                        QString pattern = dataPtr->getLine(i,j)->getPattern(k);
                                        pattern.replace('#','*');
                                        pattern.replace(',','.');
                                        QStringList patternParts = pattern.split('.');
                                        // amount of symbols before and after point
                                        if(patternParts.count()==1)// no data after point
                                        {
                                            wrData[bufPos++] = patternParts[0].count();
                                            wrData[bufPos++] = 0;
                                        }
                                        else if(patternParts.count()>=2)
                                        {
                                            wrData[bufPos++] = patternParts[0].count();
                                            wrData[bufPos++] = patternParts[1].count();
                                        }
                                        // coordinates of val on display
                                        wrData[bufPos++] = i;
                                        wrData[bufPos++] = j;
                                        wrData[bufPos++] = k;

                                        // index of Var in List
                                        int varIndex = 0;
                                        for(int n=0;n<lVars->getAllVars().count();n++)
                                        {
                                            if(lVars->getAllVars().at(i)->getName() == vName)
                                            {
                                                varIndex = n;
                                                break;
                                            }
                                        }
                                        wrData[bufPos++] = varIndex & 0xFF;
                                        wrData[bufPos++] = varIndex >> 8;
                                    }
                                }
                            }
                        }
                    wrData[lcdVarsPos] = varsOnDisplAmount & 0xFF;
                    wrData[lcdVarsPos+1] = varsOnDisplAmount >> 8;

                    float writePercent=0;
                    float writeOneStep = (float)100/(pageCount * (pageSize/reqByteCount));

                    for(int i=0;i<pageCount;i++)
                    {
                        for(int j=0;j<pageSize/reqByteCount;j++)
                        {
                            QByteArray req;
                            req += 0x01;
                            req += 0xA2;
                            quint16 crc = getCRC(req);
                            req += crc >> 8;
                            req += crc & 0xFF;

                            serial.write(req);
                            serial.waitForBytesWritten(50);

                            if (serial.waitForReadyRead(100))
                            {
                                QByteArray responseData = serial.readAll();
                                while (serial.waitForReadyRead(10)) responseData += serial.readAll();
                                crc = getCRC(responseData);
                                if((crc)||(responseData.at(2)==0)) {wrError = true;break;}
                                else
                                {
                                    if(j==0)
                                    {
                                        req.clear();
                                        req += 0x01;
                                        req += 0xA1;
                                        req += i >> 8;
                                        req += i & 0xFF;
                                        crc = getCRC(req);
                                        req += crc >> 8;
                                        req += crc & 0xFF;
                                        serial.write(req);
                                        serial.waitForBytesWritten(50);
                                        msleep(300);
                                        if (serial.waitForReadyRead(100))
                                        {
                                            responseData.clear();
                                            responseData = serial.readAll();
                                            while (serial.waitForReadyRead(10)) responseData += serial.readAll();
                                            crc = getCRC(responseData);
                                            if(crc) {wrError = true;break;}
                                        }
                                    }
                                    req.clear();
                                    req += 0x01;
                                    req += 0xA3;
                                    req += i >> 8;
                                    req += i & 0xFF;
                                    req += (j * reqByteCount) >> 8;
                                    req += (j * reqByteCount) & 0xFF;
                                    req += (char)(reqByteCount>>8);
                                    req += reqByteCount & 0xFF;
                                    for(int k=0;k<reqByteCount;k++)
                                    {
                                        req += wrData[i*pageSize + j*reqByteCount + k];
                                    }
                                    crc = getCRC(req);
                                    req += crc >> 8;
                                    req += crc & 0xFF;
                                    serial.write(req);
                                    serial.waitForBytesWritten(100);
                                    if (serial.waitForReadyRead(100))
                                    {
                                        responseData.clear();
                                        responseData = serial.readAll();
                                        while (serial.waitForReadyRead(10)) responseData += serial.readAll();
                                        crc = getCRC(responseData);
                                        if(crc) {wrError = true;break;}
                                    }
                                    writePercent += writeOneStep;
                                    emit writeStep(writePercent);
                                }
                            }else {wrError = true;break;}
                        }
                        if(wrError) {break;}
                    }
                }
                else wrError = true;
                if(wrError) emit writeError("Ошибка записи проекта");
                else emit writeOK("Проект успешно записан");
                serial.close();
            }
            else
            {
                emit writeError("Не удалось открыть порт " + savePortName);
            }
        }
        mutex.lock();
        cond.wait(&mutex);
        if(searchCmd) {searchCmd = false;startSearch = true;}
        mutex.unlock();
    }
}

void SerialCommunication::setData(DisplayData *dataPtr)
{
    this->dataPtr = dataPtr;
}

void SerialCommunication::setFont(EpsonFont *fontPtr)
{
    this->fontPtr = fontPtr;
}

int SerialCommunication::getVarsAmountOnLcd()
{
    int amountOfVars = 0;
    for(int i=0;i<dataPtr->getRowCount();i++)
        for(int j=0;j<dataPtr->getCountOfLinesInRow(i);j++)
        {
            amountOfVars+= dataPtr->getLine(i,j)->getCountOfVars();
        }
    return amountOfVars;
}

quint16 SerialCommunication::getCRC(const QByteArray &data)
{
    quint8 i,j=0;
    quint8 CRCH=0xFF,CRCL=0xFF;
    quint16 cnt = data.count();
    while(cnt--)
    {
        i=CRCH ^ data.at(j++);
        CRCH=CRCL ^ crc16H[i];
        CRCL=crc16L[i];
    }
    return (((quint16)CRCH<<8)|CRCL);
}
