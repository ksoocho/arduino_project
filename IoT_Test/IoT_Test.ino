#include <SoftwareSerial.h>
#define txPin 2
SoftwareSerial LCD = SoftwareSerial(0, txPin);
int count = 0;

// since the LCD does not send data back to the Arduino, we should only define the txPin
const int LCDdelay = 10;  // conservative, 2 actually works

// wbp: goto with row & column
void lcdPosition(int row, int col) {
  LCD.write(0x1B);   //command flag
  LCD.write(0x4C);   //command flag
  LCD.write(col);    //position
  LCD.write(row);    //position
  delay(LCDdelay);
}

void clearLCD(){
  LCD.write(0x1B);   //command flag
  LCD.write(0x43);   //clear command.
  delay(LCDdelay);
}

void backlightOn() {  //turns on the backlight
  LCD.write(0x1B);   //command flag for backlight stuff
  LCD.write(0x42);    //light level.
  delay(LCDdelay);
}

void backlightOff(){  //turns off the backlight
  LCD.write(0x1B);   //command flag for backlight stuff
  LCD.write(0x62);     //light level for off.
   delay(LCDdelay);
}

void serCommand(){   //a general function to call the command flag for issuing all other commands  
  LCD.write(0x1B);
}

void setup() {
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
  lcdPosition(0,0);
  LCD.print("Dream Maker");
}

void loop() {
   lcdPosition(1,0);
    LCD.print(count++);
  delay(1000);   

}
