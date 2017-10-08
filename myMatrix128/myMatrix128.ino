
/*
  Simple demo for <myMatrix> 16x32 Red Green LED Matrix Panel . 

  The circuit:
  A   connect to digital pin 2
  B   connect to digital pin 3
  C   connect to digital pin 4
  D   connect to digital pin 5
  OE  connect to digital pin 6
  R   connect to digital pin 7
  G   connect to digital pin 8
  CLK connect to digital pin 9
  STB connect to digital pin 10
  GND connect to GND
*/

#include "myMATRIX.h"

#define RowA_Pin 2
#define RowB_Pin 3
#define RowC_Pin 4
#define RowD_Pin 5 
#define OE_Pin 6  
#define Red_Pin 7  
#define Green_Pin 8  
#define CLK_Pin 9  
#define STB_Pin 10 

void setup () 
{
     myMatrix.Init(Red_Pin,Green_Pin,CLK_Pin,RowA_Pin,RowB_Pin,RowC_Pin,RowD_Pin,OE_Pin,STB_Pin); 
}    

void loop()
{
 myMatrix.clearScreen();

  myMatrix.fillRectangle(0,0,95,15,red);
 delay(1000);

 myMatrix.fillRectangle(0,0,95,15,green);
 delay(1000);

 myMatrix.fillRectangle(0,0,95,15,yellow);
 delay(1000);

 myMatrix.drawRectangle(0,0,95,15,red); 
 delay(1000);

 myMatrix.fillRectangle(10,3,21,12,green);
 delay(1000); 

 
}


