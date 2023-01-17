#include <iostream>

#ifdef _WIN32
#include <windows.h>
#define imsleep(microsecond) Sleep(microsecond) // ms
#else
#include <unistd.h>
#define imsleep(microsecond) usleep(1000 * microsecond) // ms
#endif

#include <vector>

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
using namespace itas109;

std::string char2hexstr(const char *str, int len)
{
    static const char hexTable[] = "0123456789ABCDEF";

    std::string result;
    for (int i = 0; i < len; ++i)
    {
        result += "0x";
        result += hexTable[str[i] / 16];
        result += hexTable[str[i] % 16];
        result += " ";
    }
    return result;
}

int countRead = 0;

class MyListener : public has_slots<>
{
public:
    MyListener(CSerialPort *sp)
        : p_sp(sp)
    {
    };

    void onReadEvent()
    {
        // read
        char data[1024] = {0};
        int recLen = p_sp->readAllData(data);

        if (recLen > 0)
        {
            recLen = recLen < 1024 ? recLen : 1023; // suppose receive length less than 1024
            data[recLen] = '\0';
            std::cout << "Count: " << ++countRead << ", Length: " << recLen << ", Str: " << data << ", Hex: " << char2hexstr(data, recLen).c_str() << std::endl;

            // return receive data
            p_sp->writeData(data, recLen);
        }
    };

private:
    CSerialPort *p_sp;
};

int main()
{
    CSerialPort sp;
    std::cout << "Version: " << sp.getVersion() << std::endl << std::endl;

    MyListener listener(&sp);

    std::vector<SerialPortInfo> m_availablePortsList = CSerialPortInfo::availablePortInfos();

    std::cout << "availableFriendlyPorts: " << std::endl;

    for (size_t i = 1; i <= m_availablePortsList.size(); ++i)
    {
        std::cout << i << " - " << m_availablePortsList[i - 1].portName << " " << m_availablePortsList[i - 1].description << std::endl;
    }

    if (m_availablePortsList.size() == 0)
    {
        std::cout << "No valid port" << std::endl;
    }
    else
    {
        std::cout << std::endl;

        int input = -1;
        do
        {
            std::cout << "Please Input The Index Of Port(1 - " << m_availablePortsList.size() << ")" << std::endl;

            std::cin >> input;

            if (input >= 1 && input <= m_availablePortsList.size())
            {
                break;
            }
        } while (true);

        std::string portName = m_availablePortsList[input - 1].portName;
        std::cout << "Port Name: " << portName << std::endl;

        sp.init(portName,              // windows:COM1 Linux:/dev/ttyS0
                itas109::BaudRate9600, // baudrate
                itas109::ParityNone,   // parity
                itas109::DataBits8,    // data bit
                itas109::StopOne,      // stop bit
                itas109::FlowNone,     // flow
                4096                   // read buffer size
        );
        sp.setReadIntervalTimeout(30); // read interval timeout 30ms

        sp.open();
        std::cout << "Open " << portName << (sp.isOpened() ? " Success" : " Failed") << std::endl;

        // connect for read
        sp.readReady.connect(&listener, &MyListener::onReadEvent);

        // write hex data
        char hex[5];
        hex[0] = 0x31;
        hex[1] = 0x32;
        hex[2] = 0x33;
        hex[3] = 0x34;
        hex[4] = 0x35;
        sp.writeData(hex, sizeof(hex));

        // write str data
        sp.writeData("itas109", 7);

        for (;;)
        {
            imsleep(1);
        }
    }

    return 0;
}