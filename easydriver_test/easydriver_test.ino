#include <SoftwareSerial.h>

#include "Servo.h"
#include "Stepper2D.h"
#include "Pauze.h"

#define txPin 2

unsigned long timePreviuos;
int byteCount = 0;    
byte byteArray[8]; // command / 0
boolean dataComplete = false;
boolean firstByte = true;
Servo servo(4);
Stepper2D stepper (6,7,10,11);  // x_step / x_dir / y_step / y_dir
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

  timePreviuos  =micros();
  servo.setValue(2400);
 
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
  clearLCD();
  
  lcdPosition(0,0);
  LCD.print("Dream Maker");
   
}

void loop() {

   unsigned long timeCurrent=micros();
   unsigned long timeElapsed=timeCurrent-timePreviuos;
   
   timePreviuos = timeCurrent;
   
   servo.updateTime(timeElapsed);
   stepper.updateTime(timeElapsed);
   pauze.updateTime(timeElapsed);
   
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

        if( byteCount == 8) dataComplete = true;
     }
   }
   
  if(dataComplete)
  {
    unsigned int v_command = (unsigned int) byteArray[0];
    unsigned int v_servo   = (unsigned int) byteArray[1]*5 +1600;
    unsigned int v_value1  = (unsigned int) byteArray[2]*256 +(unsigned int) byteArray[3];
    unsigned int v_value2  = (unsigned int) byteArray[4]*256 +(unsigned int) byteArray[5];
    unsigned int v_speed   = (unsigned int) byteArray[6]*10;
    
    int v_current_x;
    int v_current_y;

    int v_move_x;
    int v_move_y;
    
    String v_str = "";
    v_str.concat(v_command);
    v_str.concat("/");
    v_str.concat(v_servo);
    v_str.concat("/");
    v_str.concat(v_value1);
    v_str.concat("/");
    v_str.concat(v_value2);
    v_str.concat("/");
    v_str.concat(v_speed);
    
    // Text LCD Display
    lcdPosition(0,0);
    LCD.print("Received Data");
    lcdPosition(1,0);
    LCD.print(v_str);

    // Servo Motor 
    servo.setValue( v_servo );
    
    // Step Motor
    stepper.speed = v_speed;
    v_current_x = stepper.getCurrentX();
    v_current_y = stepper.getCurrentY();
    
    if (v_command == 1) {  //Up
      stepper.goUp(v_value1);
    } else if (v_command == 2) { //Down
      stepper.goDown(v_value1);
    } else if (v_command == 3) { //Left
      stepper.goLeft(v_value1);
    } else if (v_command == 4) { // Right
      stepper.goRight(v_value1);
    } else if (v_command == 5) {  // Go To Position
      v_move_x = v_value1;   // map x position
      v_move_y = v_value2;   // map y position
      stepper.goTo(v_move_x, v_move_y);
    } else if (v_command == 6) {  // WAIT
      //pauze
      pauze.setValue(v_value1*100);
    } else if (v_command == 0) {  // Reset
      stepper.setCurrentX(0);
      stepper.setCurrentY(0);
      Serial.write('r');
    }
      
    byteCount =0;
    dataComplete = false;
 
  }
   
}
