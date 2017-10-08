#include <SoftwareSerial.h>

#define txPin 10

SoftwareSerial LCD = SoftwareSerial(0, txPin);
const int LCDdelay = 10; // conservative, 2 actually works
/* L9110s 모터드라이버
   오른쪽모터
   L9110s A_1A 6
   L9110s A_1B 11
   왼쪽모터
   L9110s B_1A 3
   L9110s B_1B 5
*/
int A_1A = 6;
int A_1B = 11;
int B_1A = 3;
int B_1B = 5;
/*
   TCRT5000 라인센서
   왼쪽 라인센서 DO - D2
   오른쪽 라인센서 DO - D3
*/
int leftLineSensor =  7;
int rightLineSensor = 8;

int speed = 100;

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

void setup() {

  pinMode(txPin, OUTPUT);
  LCD.begin(9600);

  clearLCD();
  lcdPosition(0, 0);
  LCD.print("Yongho Tracer");

  lcdPosition(1, 0);
  LCD.print(speed);
  
  //핀을 초기화합니다.
  //L298 모터드라이버의 핀들을 출력으로 변경합니다.
  pinMode(A_1A, OUTPUT);
  pinMode(A_1B, OUTPUT);
  pinMode(B_1A, OUTPUT);
  pinMode(B_1B, OUTPUT);
  
  digitalWrite(A_1A, LOW);
  digitalWrite(A_1B, LOW);
  digitalWrite(B_1A, LOW);
  digitalWrite(B_1B, LOW);

  //TCRT5000 라인센서를 입력으로 변경합니다.
  pinMode(leftLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
}

/**
   함수내 코드가 무한히 실행됩니다.
*/
void loop() {
  //-----------------------------------------------------------
  //양쪽 센서가 모두 선을 감지하지 않았을 경우 
  //전진합니다.
  //-----------------------------------------------------------
  if (!digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {

    lcdPosition(1, 5);
    LCD.print("W - W");
  
    //모터A 정회전
    analogWrite(A_1A, speed);
    analogWrite(A_1B, 0);
    
    //모터B 정회전
    analogWrite(B_1A, speed);
    analogWrite(B_1B, 0);
    
  //-----------------------------------------------------------
  //왼쪽 센서가 선을 감지하지 않았을 경우 
  //오른쪽모터는 정회전, 왼쪽모터는 역회전을 하여 우회전합니다.
  //이때 속도 한쪽을 상대적으로 느리게 설정하는데, 
  //이유는 한쪽은 정지 후 가야하기 때문입니다.
  //-----------------------------------------------------------
  } else if (!digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {

    lcdPosition(1, 5);
    LCD.print("W - B");
    
    //모터A 역회전
    analogWrite(A_1A, 0);
    analogWrite(A_1B, speed);
    
    //모터B 정회전
    //-20을 하는 이유는 전진하다 역회전하는 반대 바퀴보다 가속도가 붙기 때문입니다.
    analogWrite(B_1A, speed - 20);
    analogWrite(B_1B, 0);
 
  //-----------------------------------------------------------
  //오른쪽 센서가 선을 감지하지 않았을 경우 
  //오른쪽모터는 역회전, 왼쪽모터는 정회전을 하여 좌회전합니다.
  //이때 속도 한쪽을 상대적으로 느리게 설정하는데, 
  //이유는 한쪽은 정지 후 가야하기 때문입니다.
  //-----------------------------------------------------------
  } else if (digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {

    lcdPosition(1, 5);
    LCD.print("B - W");
    
    //모터A 정회전
    //-20을 하는 이유는 전진하다 역회전하는 반대 바퀴보다 가속도가 붙기 때문입니다.
    analogWrite(A_1A, speed - 20);
    analogWrite(A_1B, 0);
    
    //모터B 역회전
    analogWrite(B_1A, 0);
    analogWrite(B_1B, speed);

  //-----------------------------------------------------------
  //양쪽모두 선을 감지했을 경우 
  //정지합니다.
  //-----------------------------------------------------------
  } else if (digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {

    lcdPosition(1, 5);
    LCD.print("B - B");

    //모터A 정지
    analogWrite(A_1A, 0);
    analogWrite(A_1B, 0);
    
    //모터B 정지
    analogWrite(B_1A, 0);
    analogWrite(B_1B, 0);
  }
}



