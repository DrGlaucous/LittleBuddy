#pragma once


//send to littlebuddy
enum SERIAL_OUT_SYMBOLS
{

	OP_COMFIRM_CONNECTION = 0x49,//'I'
	OP_INVALID_OPERATION = 0x15,//ASCII NAK
	OP_STOP = 0x73,//'s'
	OP_RESET = 0x52,//'R'
	OP_CHANGE_CHANNEL = 0x63,//'c'
	OP_CAPTURE = 0x43,//'C'
	OP_SUBMIT = 0x53,//'S'
	OP_ATTACK = 0x41,//'A'
	OP_ANSWER = 0x61,//'a'
	OP_REPLACE = 0x72, //'r'


};



//get from littlebuddy
enum SERIAL_IN_SYMBOLS
{
	RES_COMFIRM_CONNECTION = 0x06,//ASCII ACK
	RES_INVALID_OPERATION = 0x15,
	RES_SUCCESS = 0x06,//ACK
	RES_FAIL = 0xFF,
	RES_TIMEOUT = 0xFE,
	RES_STANDBY = 0x05,//ENQ

};

typedef struct BUDDY_VARS
{
	std::vector<std::string> serialList;
	int selectedPort;
	bool connected;

	//for graphing clicker activity
	std::vector<int> clickerInfo;
	int ansChoices[4];//A, B, C, D, total count of each

}BUDDY_VARS;


extern BUDDY_VARS gBuddy;


void initClicker(void);
int clickerConnect(void);



