#pragma once


#include <string>//new string stuff
#include <vector>

//clone of the one in the library, I'm doing this so I can keep the library confined to only one backend
enum BAUDRATE
{
    BaudRate110 = 110,       ///< 110
    BaudRate300 = 300,       ///< 300
    BaudRate600 = 600,       ///< 600
    BaudRate1200 = 1200,     ///< 1200
    BaudRate2400 = 2400,     ///< 2400
    BaudRate4800 = 4800,     ///< 4800
    BaudRate9600 = 9600,     ///< 9600 recommend (common)
    BaudRate14400 = 14400,   ///< 14400
    BaudRate19200 = 19200,   ///< 19200
    BaudRate38400 = 38400,   ///< 38400
    BaudRate56000 = 56000,   ///< 56000
    BaudRate57600 = 57600,   ///< 57600
    BaudRate115200 = 115200, ///< 115200
    BaudRate921600 = 921600  ///< 921600
};

std::vector<std::string> serialList(void);
bool serialConnect(std::string portName, int BaudRate);
bool serialDisconnect(void);
bool serialIsOpen(void);
bool serialSend(void* data, int size);
bool serialDataAvalible(void);
std::vector<char> serialGet(void);