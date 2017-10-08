#include <SoftwareSerial.h>
#include "TimeLib.h"

#define txPin 3
#define TIME_MSG_LEN 11
#define TIME_HEADER 'T'

SoftwareSerial LCD = SoftwareSerial(0, txPin);
// since the LCD does not send data back to the Arduino, we should on ly define the txPin
const int LCDdelay = 10; // conservative, 2 actually works

// wbp: goto with row & column
void lcdPosition(int row, int col) {
  LCD.write(0x1B);   //command flag
  LCD.write(0x4C);   //command flag
  LCD.write(col);    //position
  LCD.write(row);    //position
  delay(LCDdelay);
}
void clearLCD() {
  LCD.write(0x1B);   //command flag
  LCD.write(0x43);   //clear command.
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  LCD.write(0x1B);   //command flag for backlight stuff
  LCD.write(0x42);    //light level.
  delay(LCDdelay);
}
void backlightOff() { //turns off the backlight
  LCD.write(0x1B);   //command flag for backlight stuff
  LCD.write(0x62);     //light level for off.
  delay(LCDdelay);
}
void serCommand() {  //a general function to call the command flag for issuing all other commands
  LCD.write(0x1B);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for time sync message");

  setTime(15, 8, 0, 5, 2, 17);

  pinMode(txPin, OUTPUT);
  LCD.begin(9600);

  clearLCD();
  lcdPosition(0, 0);
  LCD.print("WELCOME");

  lcdPosition(1, 0);
  LCD.print("KSOOCHO CLOCK");

  delay(1000);
}

int number = 0;

void loop()
{

  clearLCD();

  if (Serial.available()) {
    processSyncMsg();
  }

  if (timeStatus() != timeNotSet) {
    digitalClockDisplay();
  }
  delay(1000);
  number++;
}

void digitalClockDisplay() {

  // YYYY/MM/DD
  lcdPosition(0, 0);
  LCD.print(year());
  LCD.print("/");
  printDigit(month());
  LCD.print("/");
  printDigit(day());

  //Week Day
  LCD.print(" ");
  LCD.print(weekDisplay(weekday()));

  //HH:MI:SS
  lcdPosition(1, 0);
  printDigit(hour());
  LCD.print(":");
  printDigit(minute());
  LCD.print(":");
  printDigit(second());
}

void printDigit(int digits) {
  if ( digits < 10 )
  {
    LCD.print(0);
  }
  LCD.print(digits);

}

void processSyncMsg() {
  while ( Serial.available() >= TIME_MSG_LEN ) {
    char c = Serial.read();
    Serial.print(c);
    if ( c == TIME_HEADER ) {
      time_t pctime = 0;
      for (int i = 0; i < TIME_MSG_LEN - 1; i++) {
        c = Serial.read();
        if (isDigit(c)) {
          pctime = (10 * pctime) + (c - '0');
        }
      }
      setTime(pctime);
    }
  }
}

String weekDisplay(int weekDay) {
  if ( weekDay == 1 ) {
    return "SUN";
  } else if ( weekDay == 2 ) {
    return "MON";
  } else if ( weekDay == 3 ) {
    return "TUE";
  } else if ( weekDay == 4 ) {
    return "WEN";
  } else if ( weekDay == 5 ) {
    return "THR";
  } else if ( weekDay == 6 ) {
    return "FRI";
  } else if ( weekDay == 7 ) {
    return "SAT";
  }
}

