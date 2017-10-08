#include <SoftwareSerial.h>

#define txPin 3
#define TIMECTL_MAXTICKS  4294967295L
#define TIMECTL_INIT      0

#define VERSION "01.00"
#define DATE "151125"

#define LIVE_LED 13

#define SENSOR1 A0
#define SENSOR2 A1

double alpha = 0.75;
int period = 50;
double change = 0.0;
double minval = 0.0;

int rawValue01;
int rawValue02;

double value01;
double value02;

long liveLEDMillis = 0;
boolean liveLEDDirection = false;

SoftwareSerial LCD = SoftwareSerial(0, txPin);
// since the LCD does not send data back to the Arduino, we should only define the txPin
const int LCDdelay=10;  // conservative, 2 actually works

unsigned long flashTimeMark=0;  //a millisecond time stamp used by the IsTime() function. initialize to 0
unsigned long int flashTimeInterval=1000;  //How many milliseconds we want for the flash cycle. 1000mS is 1 second.

int IsTime(unsigned long *timeMark, unsigned long timeInterval){
  unsigned long timeCurrent;
  unsigned long timeElapsed;
  int result=false;
  
  timeCurrent=millis();
  if(timeCurrent<*timeMark) {  //Rollover detected
    timeElapsed=(TIMECTL_MAXTICKS-*timeMark)+timeCurrent;  //elapsed=all the ticks to overflow + all the ticks since overflow
  }
  else {
    timeElapsed=timeCurrent-*timeMark;  
  }

  if(timeElapsed>=timeInterval) {
    *timeMark=timeCurrent;
    result=true;
  }
  return(result);  
}
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
  Serial.begin(115200);
  Serial.print("Simple Robot Arm Ver:");
  Serial.write(VERSION);
  Serial.print(" Date:");
  Serial.write(DATE);
  Serial.print("\r\n");
  
  pinMode(LIVE_LED, OUTPUT);
  
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
}

int number = 0;
const int delayMsec = 60; // 100msec per sample

void loop()
{
  
    static double oldValue01 = 0;
    static double oldValue02 = 0;
    
    // read the value from the sensor  
    rawValue01 = analogRead(SENSOR1);
    value01 = alpha * oldValue01 + (1 - alpha) * rawValue01;

    rawValue02 = analogRead(SENSOR2);
    value02 = alpha * oldValue02 + (1 - alpha) * rawValue02;

    Serial.print(rawValue01);
    Serial.print(",");
    Serial.print(value01);
    Serial.print(" / ");
    Serial.print(rawValue02);
    Serial.print(",");
    Serial.println(value02);
    
   //Text LCD Display term - 1000ms
   if(IsTime(&flashTimeMark,flashTimeInterval)) {  

    clearLCD();
      
    lcdPosition(0,0);
    LCD.print("A0-");
    LCD.print(rawValue01);

    lcdPosition(0,6);
    LCD.print("/");
    LCD.print(value01);
     
    lcdPosition(1,0);
    LCD.print("A1-");
    LCD.print(rawValue02);

    lcdPosition(1,6);
    LCD.print("/");
    LCD.print(value02);
    
   }

    oldValue01 = value01;
    oldValue02 = value02;

    delay(period); 
}


