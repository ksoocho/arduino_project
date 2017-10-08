
const int dirPin = 2;
const int stepPin = 3;

int v_speed = 100; // step per second
int v_steps = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  Serial.begin(9600);

} //setup()

void loop() {
  // put your main code here, to run repeatedly:
  
  if ( Serial.available()) {
     char ch = Serial.read();
     
     if (isDigit(ch)) {
        v_steps = v_steps * 10 + ch - '0';
       Serial.println(v_steps);
     } else if (ch == '+' ) {
       Serial.println(v_steps);
       step(v_steps);
       v_steps = 0;
     } else if ( ch == '-' ) {
       Serial.println(v_steps);
       step (-v_steps);
       v_steps = 0;
     } else if ( ch == 's' ) {
       v_speed = v_steps;
       Serial.print("Setting speed to ");
       Serial.println(v_steps);
       v_steps = 0;
     }
  } // Serial available

} // loop
  
void step(int v_steps)
{
    int stepDelay = 1000 / v_speed;
    int stepsLeft ;
    
    if ( v_steps > 0 ){
       digitalWrite(dirPin, HIGH);
       stepsLeft = v_steps;
    }
    
    if ( v_steps < 0 ){
       digitalWrite(dirPin, LOW);
       stepsLeft = -v_steps;
    }
    
    while ( stepsLeft > 0)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1);
      digitalWrite(stepPin, LOW);
      delay(stepDelay);
      stepsLeft--;
    }
    
} //step()


