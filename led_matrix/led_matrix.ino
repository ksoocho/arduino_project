#include <SPI.h>

#define RowA 2
#define RowB 3
#define RowC 4
#define RowD 5
#define OE 6
#define R1 7
#define CLK 9
#define STB 10

byte row=0;
byte hz[]=
{
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00,
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00, 
  0x00, 0x00,
  0xFF, 0xFF, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0x80, 0x01, 
  0xFF, 0xFF

  /*
  0x00, 0x00, 0x04, 0x80, 0x04, 0x80, 0x05, 0xFE, 0x09, 0x02, 0x0A, 0x04, 0x18, 0x20, 0x28, 0x20,
  0x08, 0xA8, 0x08, 0xA4, 0x09, 0x24, 0x09, 0x22, 0x0A, 0x22, 0x08, 0x20, 0x08, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x04, 0x80,0x00, 0x00, 0x04, 0x80,
  0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x04, 0x80,0x00, 0x00, 0x04, 0x80
  */
};

void setData(byte r){

  digitalWrite(RowA, (r & 0x01));
  digitalWrite(RowB, (r & 0x02));
  digitalWrite(RowC, (r & 0x04));
  digitalWrite(RowD, (r & 0x08));
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RowA, OUTPUT);
  pinMode(RowB, OUTPUT);
  pinMode(RowC, OUTPUT);
  pinMode(RowD, OUTPUT);
  
  pinMode(OE, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(STB, OUTPUT);

  SPI.begin();
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (row=0; row<16; row++) {
    
     for ( int i=0; i<2; i++) {
        SPI.transfer(~(hz[i*32+row*2]));
        SPI.transfer(~(hz[i*32+row*2+1]));
     }
     
     digitalWrite(OE, HIGH);
     
     setData(row);
     
     digitalWrite(STB, LOW);
     digitalWrite(STB, HIGH);
  
     delayMicroseconds(500);
     
     digitalWrite(OE, LOW);
  
     delayMicroseconds(500);
  }

}