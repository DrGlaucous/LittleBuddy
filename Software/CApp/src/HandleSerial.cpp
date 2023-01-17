#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//old string stuff
#include <nfd.h>
#include <string>//new string stuff
#include <vector>


//test
#include "../CSerialPort/include/CSerialPort/SerialPort.h"
#include "../CSerialPort/include/CSerialPort/SerialPortInfo.h"
using namespace itas109;//for CSerial

#include "SharedUtil.h"
#include "HandleSerial.h"

//holds incoming serial data
class MyListener : public has_slots<>
{
public:
    MyListener(CSerialPort* sp)//constructor, passes serial port to it
        : p_sp(sp)
    {
        serialData.clear();//ensure that vector is empty
    };

    void onReadEvent()//our callback event when we get data in
    {
        // read
        char data[1024] = { 0 };
        int recLen = p_sp->readAllData(data);

        if (recLen > 0)
        {
            recLen = recLen < 1024 ? recLen : 1023; // suppose receive length less than 1024

            //append data to vector
            for (int i = 0; i < recLen; ++i)
            {
                serialData.push_back(data[i]);
            }

            //data[recLen] = '\0';//final value null terminator
            // return receive data
            //p_sp->writeData(data, recLen);
        }
    };

    std::vector<char> serialData;

private:
    CSerialPort* p_sp;
};


std::vector<SerialPortInfo> availablePortsList;// library serial port list object
CSerialPort sp;//library serial port object
MyListener listener(&sp);


//list of important functions:
//serialSend(void*, size);
//serialGet(&void, &size);
//serialList(&array);
//serialConnect(num);
//


//return string array of our avalible ports
std::vector<std::string> serialList(void)
{
    //create port list object
    availablePortsList = CSerialPortInfo::availablePortInfos();

    std::vector<std::string> valueArray;

    //put all port names into the vector
    for (size_t i = 1; i <= availablePortsList.size(); ++i)
        valueArray.push_back(availablePortsList[i - 1].portName);

    return valueArray;
}

//connect to a number in the serial array
bool serialConnect(std::string portName, int BaudRate)
{


    sp.init(portName,
        BaudRate,
        ParityNone,   // parity
        DataBits8,    // data bit
        StopOne      // stop bit
    );
    sp.setReadIntervalTimeout(30); // read interval timeout 30ms
    sp.open();
    //try to open port
    if (!sp.isOpened())
        return false;


    //pass callbacks to the library
    sp.readReady.connect(&listener, &MyListener::onReadEvent);

    return true;

}

bool serialDisconnect(void)
{
    sp.close();

    return sp.isOpened();//TRUE if we failed to close it

}

bool serialIsOpen(void)
{
    return sp.isOpened();
}

//return false on success
bool serialSend(void* data, int size)
{

    return sp.writeData(data, size);

}

bool serialDataAvalible(void)
{
    //listener.onReadEvent();//test
    //int greenM = listener.serialData.size();
    return listener.serialData.size() ? true : false;
}

std::vector<char> serialGet(void)
{
    std::vector<char> output = listener.serialData;//return buffer
    listener.serialData.clear();//clear buffer
    return output;
}
