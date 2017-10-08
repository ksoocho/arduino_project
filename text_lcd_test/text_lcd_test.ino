#include <SoftwareSerial.h> 

#define txPin 3
#define rxPin -1

#define CLEAR 0x01
#define CURSOR_MOVE_RIGHT 0x14
#define CURSOR_MOVE_LEFT 0x10
#define CURSOR_BOX_BLINK_ON 0x0D
#define CURSOR_BOX_BLINK_OFF 0x0C
#define CURSOR_POS_LINE1 128
#define CURSOR_POS_LINE2 192

SoftwareSerial lcd = SoftwareSerial(rxPin, txPin);

void setup()
{
// set controller pins
pinMode(txPin, OUTPUT);
// set serial mode
Serial.begin(9600);
// sparkfun default port speed
lcd.begin(9600);
// clear the display first
sendCmd(CLEAR);
// set position to first character
sendCmd(CURSOR_POS_LINE1);
}

void loop()
{
// read data if incoming...
if (Serial.available() > 0) {
  sendCmd(CLEAR);
  lcd.write(Serial.read());
 }
}

void sendCmd(byte cmd) {
lcd.write(0xFE);
lcd.write(cmd);
}
