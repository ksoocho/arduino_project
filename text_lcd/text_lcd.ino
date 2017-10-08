#include <SoftwareSerial.h>
#include <Servo.h>

#define txPin 3
#define TIMECTL_MAXTICKS  4294967295L
#define TIMECTL_INIT      0

#define VERSION "01.00"
#define DATE "151125"

#define MOTOR1  6
#define MOTOR2  9
#define MOTOR3  10
#define MOTOR4  11

#define LIVE_LED 13

#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3

#define on 1
#define OFF 0

long liveLEDMillis = 0;
boolean liveLEDDirection = false;

Servo servoMotor1;
Servo servoMotor2;
Servo servoMotor3;
Servo servoMotor4;

int sensorValue[4][100];
long sensorMillis = 0;
long motorMillis = 0;
int sensorIndex = 0;

struct SERVICE {
  int phase;
  unsigned long times;
};

SERVICE SelfService;

//
int servoMotor1Center = 90;
int servoMotor2Center = 90;
int servoMotor3Center = 90;
int servoMotor4Center = 90;

SoftwareSerial LCD = SoftwareSerial(0, txPin);
// since the LCD does not send data back to the Arduino, we should only define the txPin
const int LCDdelay=10;  // conservative, 2 actually works

int val01 = 0;       //  sensor1
int val02 = 0;       //  sensor2
int val03 = 0;       //  sensor3
int val04 = 0;       //  sensor4

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
  servoMotor1.attach(MOTOR1);
  servoMotor2.attach(MOTOR2);
  servoMotor3.attach(MOTOR3);
  servoMotor4.attach(MOTOR4);

  Serial.begin(115200);
  Serial.print("Simple Robot Arm Ver:");
  Serial.write(VERSION);
  Serial.print(" Date:");
  Serial.write(DATE);
  Serial.print("\r\n");
  
  pinMode(LIVE_LED, OUTPUT);
  SelfService.phase = 1;
  
  pinMode(txPin, OUTPUT);
  LCD.begin(9600);
}

int number = 0;

void loop()
{

   MainService();
  
  //Text LCD Display term - 1000ms
  if(IsTime(&flashTimeMark,flashTimeInterval)) {  

    // read the value from the sensor  
    val01 = map(analogRead(SENSOR1), 0, 1023, 0, 180);
    val02 = map(analogRead(SENSOR2), 0, 1023, 0, 180);
    val03 = map(analogRead(SENSOR3), 0, 1023, 0, 180);
    val04 = map(analogRead(SENSOR4), 0, 1023, 0, 180);
    
    clearLCD();
  
    lcdPosition(0,0);
    LCD.print("M01-");
    LCD.print(val01);

    lcdPosition(0,8);
    LCD.print("M02-");
    LCD.print(val02);
     
    lcdPosition(1,0);
    LCD.print("M03-");
    LCD.print(val03);

    lcdPosition(1,8);
    LCD.print("M04-");
    LCD.print(val04);
  }  

}

void MainService()
{
  switch (SelfService.phase)
  {
    case 0:
      break;
      
    case 1:
      SensorRead();
      MotorControl();
      break;
      
    case 11:
      servoMotor1.write(servoMotor1Center);
      servoMotor2.write(servoMotor2Center);
      servoMotor3.write(servoMotor3Center);
      servoMotor4.write(servoMotor4Center);
      SelfService.phase = 0;
      break;
  }
}

void SensorRead()
{
  if (GetETime(sensorMillis) > 1)
  {
    sensorMillis = millis();
    sensorValue[0][sensorIndex] = map(analogRead(SENSOR1), 0, 1023, 0, 180);
    sensorValue[1][sensorIndex] = map(analogRead(SENSOR2), 0, 1023, 0, 180);
    sensorValue[2][sensorIndex] = map(analogRead(SENSOR3), 0, 1023, 0, 180);
    sensorValue[3][sensorIndex] = map(analogRead(SENSOR4), 0, 1023, 0, 180);
    sensorIndex++;
    if (sensorIndex >= 100)
      sensorIndex = 0;

  }
}

void MotorControl()
{
  long meanSensorValue[4], i;
  
  if (GetETime(motorMillis) > 1)
  {
    motorMillis = millis();
    
    for (i = 0; i < 4; i++)
      meanSensorValue[i] = 0;
      
    for (i = 0; i < 100; i++)
    {
      meanSensorValue[0] += sensorValue[0][i];
      meanSensorValue[1] += sensorValue[1][i];
      meanSensorValue[2] += sensorValue[2][i];
      meanSensorValue[3] += sensorValue[3][i];
    }
    
    for (i = 0; i < 4; i++)
      meanSensorValue[i] = meanSensorValue[i] / 100;
      
    servoMotor1.write(meanSensorValue[0]);
    servoMotor2.write(meanSensorValue[1]);
    servoMotor3.write(meanSensorValue[2]);
    servoMotor4.write(meanSensorValue[3]);

  }
}

unsigned long GetETime(unsigned long referenceTime)
{
  unsigned long returnValue;
  unsigned long currentMillis = millis();
  if (referenceTime > currentMillis)
  {
    returnValue = 4294967295 + (currentMillis - referenceTime);
    returnValue++;
  }
  else
  {
    returnValue = currentMillis - referenceTime;
  }
  return returnValue;
}
