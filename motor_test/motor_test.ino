int A_1A = 6;
int A_1B = 11;
int B_1A = 3;
int B_1B = 5;

int speed = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(A_1A, OUTPUT);
  pinMode(A_1B, OUTPUT);
  pinMode(B_1A, OUTPUT);
  pinMode(B_1B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //모터A 정회전
    analogWrite(A_1A, speed);
    analogWrite(A_1B, 0);
    
    //모터B 정회전
    analogWrite(B_1A, 0);
    analogWrite(B_1B, speed);
}
