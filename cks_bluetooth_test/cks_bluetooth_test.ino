#include <SoftwareSerial.h>

#define txPin 5


SoftwareSerial LCD = SoftwareSerial(0, txPin);
SoftwareSerial BTSerial(2, 3); //Connect HC-06. Use your (TX, RX) settings

// since the LCD does not send data back to the Arduino, we should only define the txPin

const int LCDdelay=10;  // conservative, 2 actually works


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

void setup()  
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
  lcdPosition(0,0);
  LCD.print("Let's chat");

  BTSerial.begin(9600);  // set the data rate for the BT port
}

/* time */
unsigned long prevReceivedTime = 0;
unsigned long curReceivedTime = 0;
int currentLine = 0;  // Display character at 0 or 1 line

/* time */
unsigned long tprevReceivedTime = 0;
unsigned long tcurReceivedTime = 0;
int tcurrentLine = 0;  // Display character at 0 or 1 line
 
void loop()
{
  //--------------------------------     
  // BT –> Data –> Serial
  //--------------------------------     
  int i = 0;
  char  commandbuffer[100];

  if (BTSerial.available()) {
    delay(100);
    while (  BTSerial.available() && i < 99 ){
      commandbuffer[i++] = BTSerial.read();    
    }
    commandbuffer[i++]='\0';
  }
  
  if(i>0){
     
     // Serial Monitor Display
     Serial.print((char*)commandbuffer);
     
     //LCD Display
     lcdPosition(1,0);
     LCD.print((char*)commandbuffer);
     delay(1000);
     lcdPosition(1,0);
     LCD.print("                ");
  }

  //--------------------------------     
  // Serial –> Data –> BT
  //--------------------------------     
  int j = 0;
  char  responsebuffer[100];

  if (Serial.available()) {
    delay(100);
    while (  Serial.available() && j < 99 ){
      responsebuffer[j++] = Serial.read();    
    }
    responsebuffer[j++]='\0';
  }
  
  if(j>0){
    // BT Dispaly
     BTSerial.write((char*)responsebuffer);
     
     //LCD Display
     lcdPosition(0,0);
     LCD.print((char*)responsebuffer);
     delay(1000);
     lcdPosition(0,0);
     LCD.print("                ");
  }

}
