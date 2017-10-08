#include "Arduino.h"
#include "Stepper2D.h"

Stepper2D::Stepper2D (int pinXStep,int pinXDir,int pinYStep,int pinYDir)
{
   currentX =0;
   currentY =0;
   
  _pinXStep =pinXStep;
  _pinXDir=pinXDir;
  _pinYStep=pinYStep;
  _pinYDir =pinYDir;
 
   pinMode(_pinXStep, OUTPUT);
   pinMode(_pinXDir, OUTPUT);
   pinMode(_pinYStep, OUTPUT);
   pinMode(_pinYDir, OUTPUT);
   
   currentTime = 0;
   _distance = 0;
   moveType = MOVENONE;
}

void Stepper2D::goUp(unsigned long distance)
{
  digitalWrite(_pinYDir, LOW);
  _distance = distance;
  currentX = currentY =0;
  moveType = MOVEY;
}

void Stepper2D::goDown(unsigned long distance)
{
  digitalWrite(_pinYDir, HIGH);
  _distance = distance;
  currentX = currentY = 0;
  moveType = MOVEY;
}

void Stepper2D::goLeft(unsigned long distance)
{
  digitalWrite(_pinXDir, HIGH);
  _distance = distance;
  currentX = currentY = 0;
  moveType = MOVEX;
}

void Stepper2D::goRight(unsigned long distance)
{
  digitalWrite(_pinXDir, LOW);
  _distance = distance;
  currentX = currentY = 0;
  moveType = MOVEX;
}

void Stepper2D::goTo(int xt, int yt)
{
  moveType =MOVEXY;
  
  int deltax = abs(xt - currentX);        // The difference between the x's
  int deltay = abs(yt - currentY);
  
   x = currentX;                       // Start x off at the first pixel
   y = currentY;                       // Start y off at the first pixel
   
   // The x-values are increasing
   // The x-values are decreasing
    if (xt >= currentX) {  
        xinc1 = 1;
        xinc2 = 1;
    }    else   {     
        xinc1 = -1;
        xinc2 = -1;
    }
   
   // The y-values are increasing
   // The y-values are decreasing
    if (yt >= currentY)    {
        yinc1 = 1;
        yinc2 = 1;
    }    else   {
        yinc1 = -1;
        yinc2 = -1;
    }
   
   // There is at least one x-value for every y-value
    // There is at least one y-value for every x-value
    if (deltax >= deltay)  {
        xinc1 = 0;                  // Don't change the x when numerator >= denominator
        yinc2 = 0;                  // Don't change the y for every iteration
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         // There are more x-values than y-values
    }    else   {
        xinc2 = 0;                  // Don't change the x for every iteration
        yinc1 = 0;                  // Don't change the y when numerator >= denominator
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         // There are more y-values than x-values
    }
}

void Stepper2D::updateTime(unsigned long timeStep)
{
  currentTime += timeStep;
  
  if(currentTime <= speed)  {
    return;
  }
  
  currentTime=0;

  if (moveType == MOVENONE)  {

  }  else if (moveType ==MOVEX)  {
    digitalWrite( _pinXStep, HIGH); 
    delayMicroseconds(1) ;
    digitalWrite( _pinXStep, LOW); 
    _distance--;
    
    if (_distance==0) {
      Serial.write('x');
      moveType =MOVENONE;
    }
    
  }  else if (moveType ==MOVEY)  {
    digitalWrite( _pinYStep,HIGH); 
    delayMicroseconds(1) ;
    digitalWrite( _pinYStep, LOW); 
    
    _distance--;
    
    if (_distance==0) {
      Serial.write('y');
      moveType =MOVENONE;
    }
  }  else if (moveType ==MOVEXY) {
    
       stepX = (x-currentX);
       stepY = (y-currentY);  
       currentX += stepX;     
       currentY += stepY;
      
       if (stepX == -1) {
          stepX=1;
          digitalWrite(_pinXDir, HIGH);
       }  else if (stepX == 1) {
          digitalWrite(_pinXDir, LOW);
       }
      
       if (stepY == -1) {
          stepY=1;
          digitalWrite(_pinYDir, HIGH);
       } else if (stepY == 1) {
          digitalWrite(_pinYDir, LOW  );
       }     
      
       if (stepY==1) digitalWrite(_pinYStep, HIGH);
       if (stepX==1)  digitalWrite(_pinXStep, HIGH); 
      
       delayMicroseconds(1) ;
      
      if (stepY==1)   digitalWrite(_pinYStep, LOW);
      if (stepX==1)  digitalWrite(_pinXStep, LOW); 
     
        num += numadd;             
      if (num >= den)            
      {
         num -= den;              
         x += xinc1;              
         y += yinc1;              
       }
       
       x += xinc2;                
       y += yinc2;
      
       numpixels--;
       
       if (numpixels==0)
       {
           moveType =MOVENONE;
           Serial.write('l');
       }
  }
}

int Stepper2D::getCurrentX()
{
  return currentX;
}

int Stepper2D::getCurrentY()
{
  return currentY;
}

void Stepper2D::setCurrentX(int _cur_x)
{
  currentX = _cur_x;
}

void Stepper2D::setCurrentY(int _cur_y)
{
  currentY = _cur_y;
}
