#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

// class describing communication channel properties

class SerialChannel
{
    int baudrate;
    int countOfStopBits;
    int countOfDataBits;
    bool parityBit;

public:
    enum {BAUD_9600, BAUD_19200, BAUD_38400, BAUD_57600, BAUD_115200, BAUD_UNKNOWN};
    SerialChannel(int baudrate = BAUD_19200);
    int getBaudrate(void) const;
    void setCountOfStopBits(int value);
    int getCountOfStopBits(void) const;
    void setParity(bool value);
    bool getParity(void) const;
};

#endif // SERIALCHANNEL_H
