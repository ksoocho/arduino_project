int firstSensor = 0; // first analog sensor
int secondSensor = 0; // second analog sensor
int thirdSensor = 0; // digital sensor
int inByte = 0;   // incoming serial byte

void setup()
{
 // start serial port at 9600 bps:
 Serial.begin(9600);
 while (!Serial) {
 ; // wait for serial port to connect. Needed for Leonardo only
 }
 pinMode(2, INPUT); // digital sensor is on digital pin 2
 establishContact(); // send a byte to establish contact until receiver
      // responds
}

void loop()
{
     // if we get a valid byte, read analog ins:
     if (Serial.available() > 0) {
         // get incoming byte:
         inByte = Serial.read();
         // read first analog input, divide by 4 to make the range 0-255:
         firstSensor = analogRead(0);
         // delay 10ms to let the ADC recover:
         delay(10);
         // read second analog input, divide by 4 to make the range 0-255:
         secondSensor = analogRead(1);
         // read switch, map it to 0 or 255L
         thirdSensor = map(digitalRead(2), 0, 1, 0, 255);
         // send sensor values:
         Serial.write(firstSensor);
         Serial.write(secondSensor);
         Serial.write(thirdSensor);
     }
}
void establishContact() {
   while (Serial.available() <= 0) {
       Serial.print('A'); // send a capital A
       delay(50);
   }
}
