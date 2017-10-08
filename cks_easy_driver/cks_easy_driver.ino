const int dirPin = 2;
const int stepPin = 3;

int speed = 100;
int steps = 0;

void step(int steps);

void setup() {
  // put your setup code here, to run once:
  
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

 if ( Serial.available()){
   char ch = Serial.read();
   
   if(isDigit(ch)){
     steps = steps * 10 + ch - '0';
   } else if (ch == '+'){
     step(steps);
     steps = 0;
   } else if (ch == '-'){
     step(-steps);
     steps = 0;
   } else if (ch == 's'){
     speed = steps;
     steps = 0;
   } 

   Serial.print("Setting Speed to");
   Serial.println(steps);
 }   
}

void step(int steps)
{
  int stepDelay = 1000 / speed;
  int stepsLeft;
 
  if (steps > 0 ) { 
    digitalWrite(dirPin, HIGH);
    stepsLeft = steps;
 } 
 
 if (steps < 0 ) { 
    digitalWrite(dirPin, LOW);
    stepsLeft = -steps;
 } 
 
 while  (stepsLeft > 0) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(stepPin, LOW);
    delay(stepDelay);
    stepsLeft--;
 }
 
}
