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

 char scrolltext_1[]="     * Happy New Year *      ";
 char scrolltext_2[]="     * Numbers * 1234567890      ";
 char scrolltext_3[]="     * Capital Letters * ABCDEFGHIJKLMNOPQRSTUVXYZ      ";
 char scrolltext_4[]="     * Small Letters * abcdefghijklmnopqrstuvxyz      ";
 
 //myMatrix.clearScreen();
 //myMatrix.printString16(0,0,yellow,black,"Happy New Year");
 //delay(3000);
 
 myMatrix.clearScreen();

 myMatrix.scrollRepLeftDot(green,black);
 delay(1000);

 myMatrix.scrollRepRightDot(green,black);
 delay(1000);
 /*
 
 myMatrix.fillRectangle(0,0,31,15,red);
 delay(1000);
 myMatrix.fillRectangle(0,0,31,15,green);
 delay(1000);
 myMatrix.fillRectangle(0,0,31,15,yellow);
 delay(1000);
 myMatrix.drawRectangle(0,0,31,15,red); 
 delay(1000);
 myMatrix.fillRectangle(10,3,21,12,green);
 delay(1000); 

 myMatrix.clearScreen();
 myMatrix.drawHLine(0,31,7,green);
 myMatrix.drawHLine(0,31,15,green);
 myMatrix.drawVLine(0,0,15,yellow);
 myMatrix.drawVLine(31,0,15,yellow);
 myMatrix.printString(5,8,yellow,black,"Demo");
 myMatrix.hScroll(0,red,black,scrolltext_1);
 myMatrix.hScroll(0,black,green,scrolltext_1);
 myMatrix.hScroll(0,red,black,scrolltext_2);
 myMatrix.hScroll(0,red,black,scrolltext_3);
 myMatrix.hScroll(0,red,black,scrolltext_4);
 */
}


