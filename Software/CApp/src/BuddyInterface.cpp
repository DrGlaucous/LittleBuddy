#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//old string stuff
#include <nfd.h>
#include <string>//new string stuff
#include <vector>

#include "WindowView.h"//for the GetTicks() variables
#include "BuddyInterface.h"
#include "HandleSerial.h"

#define WAIT_TIMEOUT 600//before we call it quits

BUDDY_VARS gBuddy;

void initClicker(void)
{
	memset(&gBuddy, 0, sizeof(BUDDY_VARS));
};

//format command and push it over serial
bool sendCommand(char command, const char* argument)
{

	//format of the commands the iSkipper takes:
	//[COMMAND CHAR]+,+[ARGUMENTS]+'\0'

	std::string compiledString;

	if(argument != NULL)
		compiledString = command + ',' + argument + '\0';
	else
		compiledString = command;//no arguments


	return serialSend((void*)compiledString.c_str(), compiledString.size());

}

//send the connect comand to the clicker, assumes it will be called once every millisecond while waiting
int clickerConnect(void)
{
	static bool waiting = false;//waiting for response from littlebuddy
	static int waitTime;//time left until timeout

	if (!waiting)
	{
		//out-of-range protection handled outside this function

		serialConnect(gBuddy.serialList[gBuddy.selectedPort], BaudRate9600);
		if (!serialIsOpen())//failed to open COM port
			return 3;

		sendCommand(OP_COMFIRM_CONNECTION, NULL);
		waiting = true;
		waitTime = WAIT_TIMEOUT;
	}
	else//wait until we recieve an ACK
	{

		if (serialDataAvalible())
		{
			std::vector<char> inputVec = serialGet();
			
			WriteToTerminal(inputVec.data());

			//the data we got is a confimration packet
			for(int itr = 0; itr < inputVec.size(); ++itr)
				if (inputVec[itr] == (char)RES_COMFIRM_CONNECTION)
				{
					waiting = false;//reset function
					return 0;//success
				}

		}

		//we timed out
		if (--waitTime < 0)
		{
			serialDisconnect();

			waiting = false;//reset function
			return 2;//timout
		}
		else if (waitTime == 500)
		{
			sendCommand(OP_COMFIRM_CONNECTION, NULL);
			WriteToTerminal("send ACK");
		}
	}

	return 1;//still waiting
}

//tell the clicker to software reset (equivalent of a disconnect)
bool clickerReset(void)
{
	return sendCommand(OP_RESET, NULL);
}


















