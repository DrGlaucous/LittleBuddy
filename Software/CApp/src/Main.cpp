// To copy tracks from one ORG program to another
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
//#include <stdlib.h>//originally intended to improve the terminal experience, but dropped in favor of developing a full GUI version of this program instead
#include <cstring>
#include <sstream>
#include <vector>

#include "Main.h"
#include "File.h"
#include "WindowView.h"
#include "HandleSerial.h"

#ifdef _WIN32
#include <windows.h>
#define imsleep(microsecond) Sleep(microsecond) // ms
#else
#include <unistd.h>
#define imsleep(microsecond) usleep(1000 * microsecond) // ms
#endif




int main(int argc, char** argv)
{


	bool ShouldExit = false;

	while (!ShouldExit)
	{
		ShouldExit = TopFunction();
		imsleep(1);
	}

	return 0;


}




