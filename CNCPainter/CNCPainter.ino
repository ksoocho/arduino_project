#include <SoftwareSerial.h>

#include "Servo.h"
#include "Stepper2D.h"
#include "Pauze.h"

#define txPin 5

unsigned long timePreviuos;
int byteCount = 0;     
byte byteArray[8]; 

boolean dataComplete = false; 
boolean firstByte = true;

Servo servo(12);
Stepper2D stepper (3,4,20,21);  // step/dir//step/dir
Pauze pauze ;

SoftwareSerial LCD = SoftwareSerial(0, txPin);

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
 
  Serial.begin(9600);
  Serial.println("CNC Painter!");
  
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
    
  timePreviuos  =micros();
  servo.setValue(2400);
  
  lcdPosition(0,0);
  LCD.print("wait...");

  lcdPosition(1,0);
  LCD.print("   ");
  
}


// the loop routine runs over and over again forever:
void loop() {
  
   unsigned long timeCurrent=micros();
   unsigned long timeElapsed=timeCurrent-timePreviuos;
      
   timePreviuos =timeCurrent;
  
   servo.updateTime(timeElapsed);
   stepper.updateTime(timeElapsed);
   pauze.updateTime(timeElapsed);
   //clear the first incoming byte, not sure why i get that one

   // get dhe data
   while(Serial.available())
   {
     byte bin = Serial.read(); 

     if (byteCount==0 && bin==255)
     {
     // i have a stray first byte sometimes (connect)
     
     }
     else{
     
        byteArray[byteCount] = bin;
        byteCount++;
        //if (byteCount ==4) display.setLine(1);
        if( byteCount==8) dataComplete = true;
     }
   }
  
  if(dataComplete)
  {
    unsigned int command  = (unsigned int) byteArray[0];
    servo.setValue( (unsigned int) byteArray[1]*5 +1600);
    unsigned int value1 = (unsigned int) byteArray[2]*256 +(unsigned int) byteArray[3];
    unsigned int value2 = (unsigned int) byteArray[4]*256 +(unsigned int) byteArray[5];
    unsigned int speed = (unsigned int) byteArray[6]*10;
    
    stepper.speed  = speed;

     clearLCD();
     lcdPosition(1,0);

    if (command ==0) {
     //pauze
     LCD.print('a');
    
    } else if (command==1) {
      ///up
     LCD.print('1:');
      stepper.goUp(value1);
    }  else if (command==2)  {
      //down
     LCD.print('2:');
      stepper.goDown(value1);
    }  else if (command==3) {
      //left
     LCD.print('3:');
      stepper.goLeft(value1);
    }  else if (command==4)  {
      //right
     LCD.print('4:');
      stepper.goRight(value1);
    } else if (command==5) {
      //moveto
     LCD.print('5:');
      stepper.goTo(value1,value2);
    } else if (command==6) {
        //pauze
       LCD.print('6:');
        pauze.setValue(value1*100);
    }

    byteCount =0;
    dataComplete = false;

  }  
}



