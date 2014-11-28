#include "serialchannel.h"

SerialChannel::SerialChannel(int baudrate): countOfStopBits(1), countOfDataBits(8)
{
    if((baudrate>= BAUD_UNKNOWN)||(baudrate<0)) this->baudrate = BAUD_19200;
    else this->baudrate = baudrate;
}


int SerialChannel::getBaudrate() const
{
    return baudrate;
}

void SerialChannel::setCountOfStopBits(int value)
{
    if((value==1)||(value==2)) countOfStopBits = value;
    else countOfStopBits = 1;
}

int SerialChannel::getCountOfStopBits() const
{
    return countOfStopBits;
}

void SerialChannel::setParity(bool value)
{
    parityBit = value;
}

bool SerialChannel::getParity() const
{
    return parityBit;
}

