#include <Key.h>
#include <Keypad.h>

#include <SoftwareSerial.h>

#define txPin 10
#define redLED 11 //define the LED pins
#define greenLED 12

SoftwareSerial LCD = SoftwareSerial(0, txPin);
const int LCDdelay = 10;

char* password = "1383"; //create a password
int pozisyon = 0; //keypad position

const byte rows = 4; //number of the keypad's rows and columns
const byte cols = 4;

char keyMap [rows] [cols] = { //define the cymbols on the buttons of the keypad

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins [rows] =  {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins [cols] =  {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);

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

void setLocked(int locked) {
  if (locked) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  else {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Digital lock key");
  
  pinMode(txPin, OUTPUT);
  pinMode(redLED, OUTPUT);  //set the LED as an output
  pinMode(greenLED, OUTPUT);
  setLocked (true); //state of the password

  LCD.begin(9600);

  clearLCD();
  lcdPosition(0, 0);
  LCD.print("Digital Lock");

  lcdPosition(1, 0);
  LCD.print("by ksoocho");

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  char whichKey = myKeypad.getKey(); //define which key is pressed with getKey

  Serial.print(whichKey);

  lcdPosition(0, 0);
  LCD.print("  Welcome   ");

  lcdPosition(1, 0);
  LCD.print("Enter password");

  if (whichKey == '*' || whichKey == '#' || whichKey == 'A' ||      //define invalid keys
      whichKey == 'B' || whichKey == 'C' || whichKey == 'D') {

    pozisyon = 0;
    setLocked (true);
    
    clearLCD();
    lcdPosition(0, 0);
    LCD.print("Invalid Key!");
    delay(1000);
    clearLCD();
  }

  if(whichKey == password [pozisyon]){
    pozisyon ++;
  }
  
  if(pozisyon == 4){
    setLocked (false);
    
    clearLCD();
    lcdPosition(0, 0);
    LCD.print("*** Verified ***");
    delay(3000);
    
    clearLCD();
  }
  delay(100);
}
