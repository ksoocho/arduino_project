/*
File	: myMATRIX_Demo.pde
Version	: 1.0
Date	: 14.01.2015
Project	: myMatrix Arduino Library

The MIT License (MIT)

Copyright (c) 2015 Silviu - www.openhardware.ro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


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
  
  /*

   myMatrix.fillRectangle(0,0,15,15,red);
   delay(1000);
  
   myMatrix.fillRectangle(0,0,15,15,green);
   delay(1000);
  
   myMatrix.fillRectangle(0,0,15,15,yellow);
   delay(1000);

   */
  
   myMatrix.drawRectangle(0,0,15,15,red); 
   delay(1000);
  
   myMatrix.fillRectangle(5,3,10,12,green);
   delay(1000); 
   
 myMatrix.clearScreen();
 
 for(int jnx=0; jnx<42; jnx++)
 {
   myMatrix.dispBitman(0,0,red, yellow,jnx);
   myMatrix.dispBitman(8,0,green, yellow,jnx);
   myMatrix.dispBitman(0,8,green, yellow,jnx);
   myMatrix.dispBitman(8,8,red, yellow,jnx);
   delay(1000); 
 }
 
 myMatrix.clearScreen();
 
 for(int jnx=0; jnx<5; jnx++)
 {
   myMatrix.dispGhost(3,0,red, yellow,jnx);
   delay(1000); 
 }
 
 myMatrix.clearScreen();
 
 for(int jnx=0; jnx<11; jnx++)
 {
   myMatrix.dispLobber(4,4,red, yellow,jnx);
   delay(1000); 
 }


}


