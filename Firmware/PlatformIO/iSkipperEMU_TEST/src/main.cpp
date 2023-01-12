#include <iClickerEmulator.h>
#include <RingBufCPP.h>
#include <string.h>
#include <Arduino.h>

/* UPDATE THESE FOR YOUR PARTICULAR BOARD */
#define IS_RFM69HW false //make true if using w version
#define IRQ_PIN PB4 // This is 3 on adafruit feather (DIO0 on the radio)
#define CSN PA4 // This is 8 on adafruit feather (no idea what this is or what it does... I think it's the NSS pin?)
/* END THINGS YOU MUST UPDATE */

#define MAX_BUFFERED_PACKETS 20
#define SEND_ACKS true

iClickerEmulator clicker(CSN, IRQ_PIN, digitalPinToInterrupt(IRQ_PIN), IS_RFM69HW);


// This will flood base station with random answers under random ids
// Use at your own risk...

void setup()
{
    Serial.begin(115200);
    clicker.begin(iClickerChannels::AA); //set channel to AA
    //clicker.dumpRegisters();
}


void loop()
{

  uint8_t id[4];
  iClickerEmulator::randomId(id);
  iClickerAnswer ans = clicker.randomAnswer();


	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	//problem: we're hanging on the wait-for-radio-init function.
	//to troubleshoot, we could: see if the library is too old, see if the pin assignment is wrong, see if we wired the radio incorrectly
	clicker.submitAnswer( id, ans, false, 100);


	digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);



}
