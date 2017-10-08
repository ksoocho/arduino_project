#include <Servo.h>

#define VERSION "01.00"
#define DATE "130527"

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
  SelfService.phase = 0;
}

void loop()
{
  WatchDogLED();
  MainService();
}

void MainService()
{
  switch (SelfService.phase)
  {
    case 0:
      danceCenter();
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

    Serial.print("\r\n Sensor:"); Serial.print(meanSensorValue[0]);

  }
}

void danceCenter()
{
  int i;
  
  servoMotor1.write(servoMotor1Center);
  servoMotor2.write(servoMotor2Center);
  servoMotor3.write(servoMotor3Center);
  servoMotor4.write(servoMotor4Center);  
  delay(100);
  
  for (i = 0; i < 4; i++)
  {
    servoMotor1.write(servoMotor1Center+10*i);
    delay(100);
  }
  
  for (i = 0; i < 4; i++)
  {
    servoMotor1.write(servoMotor1Center+10*i);
    delay(100);
  }
  
}

void  WatchDogLED()
{
  if (GetETime(liveLEDMillis) > 500)
  {
    liveLEDMillis = millis();
    digitalWrite(LIVE_LED, liveLEDDirection);
    liveLEDDirection = ~liveLEDDirection;
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

