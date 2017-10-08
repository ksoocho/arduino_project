//Author: cantone-electonics
//More information welcome to : http://www.canton-electronics.com 
//Arduino 1.0.4
//Arduino uno R3
//64x16 Matrix LED

#define COL_PIXEL     128
#define ROW_PIXEL     16

int latchPin=10; //LT
int clockPin=9;//SK
int dataPin=7; //R1

int en_74138 = 6;

int la_74138 = 2;
int lb_74138 = 3;
int lc_74138 = 4;
int ld_74138 = 5;
 
unsigned int ROW_xPixel;
unsigned int ROW_num;
unsigned char Col_num_1;
unsigned char Col_num_2;
unsigned char Col_num_3;
unsigned char Col_num_4;
unsigned char Col_num_5;
unsigned char Col_num_6;
unsigned char Col_num_7;
unsigned char Col_num_8;


//Data code: Horizontal modulus ,Bytes reverse order
unsigned char  Bmp1[]=
{
/*------------------------------------------------------------------------------
;  Width X height (pixels): 64X16
;I Love You
------------------------------------------------------------------------------*/
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xff, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0xc0, 0x0, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x0, 0x3, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x30, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x0, 0x3, 0xc0, 0x3, 0x3, 0x30, 0xf0, 0x0, 0xc0, 0xc, 
0xf0, 0xc0, 0x0, 0x3, 0x0, 0xc, 0x0, 0x3, 0x30, 0xc, 0x3, 0x30, 0xc, 0x3, 0x0, 0x3, 0xc, 0xc3, 0x0, 0x3, 0x0, 0xc,
0x0, 0x3, 0xc, 0x30, 0x3, 0x30, 0x3, 0xc, 0x0, 0x3, 0x3, 0xcc, 0x0, 0x3, 0x0, 0xc, 0x0, 0x3, 0xc, 0x30, 0x3, 0x30,
0x3, 0xc, 0x0, 0x3, 0x3, 0xcc, 0x0, 0x3, 0x0, 0xc, 0x0, 0x3, 0xc, 0x30, 0x3, 0x30, 0xf, 0xc, 0x0, 0x3, 0x3, 0xcc, 
0x0, 0x3, 0x0, 0xc, 0x0, 0x3, 0xc, 0x30, 0x3, 0x30, 0xf3, 0x3, 0x0, 0x3, 0x3, 0xcc, 0x0, 0x3, 0x0, 0xc, 0x0, 0x3, 
0xc, 0x30, 0xc, 0xc, 0x3, 0x0, 0x0, 0x3, 0x3, 0xcc, 0x0, 0x3, 0x0, 0xc, 0x0, 0xc3, 0x30, 0xc, 0x30, 0x3, 0x3, 0xc, 
0x0, 0x3, 0xc, 0xc3, 0x0, 0x3, 0xc0, 0xff, 0x0, 0xff, 0xc0, 0x3, 0xc0, 0x0, 0xfc, 0x3, 0x0, 0x3, 0xf0, 0x0, 0xff, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 
};



unsigned char  Bmp2[]=
{
/*------------------------------------------------------------------------------
; Width X height (pixels): 16X16
;WELCOME 64 16
------------------------------------------------------------------------------*/
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3c, 0x30, 0xf0, 0xfc, 0xff, 0x3c, 0x0, 
0xf0, 0x3f, 0xf0, 0xf, 0x3c, 0x0, 0xcf, 0xff, 0x3f, 0x3c, 0xf0, 0xf0, 0xfc, 0xff, 0x3c, 0x0, 0xfc, 0x3f, 0xfc, 0x3f, 0x3c, 
0x0, 0xcf, 0xff, 0x3f, 0x3c, 0xf0, 0xf0, 0x3c, 0x0, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0x3c, 0x0, 0xcf, 0x3, 0x0, 0x3c, 0xfc,
0xf0, 0x3c, 0x0, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0xfc, 0xc0, 0xcf, 0x3, 0x0, 0x3c, 0xfc, 0xf0, 0x3c, 0x0, 0x3c, 0x0, 0xf, 
0x0, 0xf, 0xf0, 0xfc, 0xf0, 0xcf, 0x3, 0x0, 0xfc, 0xfc, 0xfc, 0xfc, 0xff, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0xfc, 0xf3, 0xcf,
0xff, 0x3f, 0xf0, 0xfc, 0x3c, 0xfc, 0xff, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0x3c, 0x3f, 0xcf, 0xff, 0x3f, 0xf0, 0xfc, 0x3c, 
0x3c, 0x0, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0x3c, 0x3f, 0xcf, 0x3, 0x0, 0xf0, 0xfc, 0x3c, 0x3c, 0x0, 0x3c, 0x0, 0xf, 0x0,
0xf, 0xf0, 0x3c, 0x3f, 0xcf, 0x3, 0x0, 0xf0, 0xff, 0x3f, 0x3c, 0x0, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0x3c, 0x3f, 0xcf, 0x3,
0x0, 0xf0, 0xff, 0x3f, 0x3c, 0x0, 0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0x3c, 0x3f, 0xcf, 0x3, 0x0, 0xc0, 0xcf, 0xf, 0x3c, 0x0, 
0x3c, 0x0, 0xf, 0x0, 0xf, 0xf0, 0x3c, 0xf, 0xcf, 0x3, 0x0, 0xc0, 0xcf, 0xf, 0xfc, 0xff, 0xfc, 0xff, 0xfc, 0x3f, 0xfc, 0x3f, 
0x3c, 0xc, 0xcf, 0xff, 0x3f, 0x0, 0x3, 0x3, 0xfc, 0xff, 0xfc, 0xff, 0xf0, 0x3f, 0xf0, 0xf, 0x3c, 0xc, 0xcf, 0xff, 0x3f, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};


void shiftOut(unsigned char dataOut)
{
  
    for(int i=0;i<=7;i++)
   {
      PORTB &=~(1<<(clockPin-8));//equate digitalWrite(clockPin,LOW);
       
     if(dataOut & (0x01<<i))  PORTB |=1<<(dataPin-8); //equate digitalWrite(dataPin,HIGH);
     else  PORTB &=~(1<<(dataPin-8));//equate digitalWrite(dataPin,LOW);
     
     PORTB |=1<<(clockPin-8);//equate digitalWrite(clockPin,HIGH);
     
   }
}

//Combine 2 bits/pixel to 1 bits/pixel 
unsigned char Combine_2BitsTo1Bit(unsigned char num,unsigned char *BMP)
{
  
  unsigned char Col_num_tem_1;
  unsigned char Col_num_tem_2;
  unsigned int Col_num_tem = 0;
  unsigned char i=0;
  unsigned char Col_num_1bit = 0x00;
  
  Col_num_tem_1 = *(BMP+num);
  Col_num_tem_2 = *(BMP+num+1);
  
  Col_num_tem = Col_num_tem_1;
  Col_num_tem |= (Col_num_tem_2 << 8);
  
 for(i=0;i<8;i++)
  {   
    if(Col_num_tem&(0x0003<<i*2)) Col_num_1bit |= (0x01<<i);
  }
  return ~Col_num_1bit;
}

//display one picture 
void display_martix(unsigned char *BMP)
{
  //Display count
  unsigned int dis_cnt=256;
  unsigned int i;
  
   for(i=0;i<dis_cnt*16;i++)
   {
     
     digitalWrite(en_74138, HIGH);//Turn off display
  
     //Col scanning
    shiftOut(Col_num_1);
    shiftOut(Col_num_2);
    shiftOut(Col_num_3);
    shiftOut(Col_num_4);
    shiftOut(Col_num_5);
    shiftOut(Col_num_6);
    shiftOut(Col_num_7);
    shiftOut(Col_num_8);
  
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);
  
    //Row scanning
    // AVR Port Operation 
    PORTD = ((ROW_xPixel << 3 ) & 0X78) | (PORTD & 0X87);//Write PIN 3 4 5 6 la_74138 lb_74138 lc_74138 ld_74138
     
    digitalWrite(en_74138, LOW);//Turn on display
  

     
    if(ROW_xPixel==15) ROW_xPixel=0; else ROW_xPixel++;
  
   /*
   // Single color,1 bits/pixel
   Col_num_1=~BMP[(COL_PIXEL/8)*ROW_xPixel];
   Col_num_2=~BMP[(COL_PIXEL/8)*ROW_xPixel+1];
   Col_num_3=~BMP[(COL_PIXEL/8)*ROW_xPixel+2];
   Col_num_4=~BMP[(COL_PIXEL/8)*ROW_xPixel+3];
   Col_num_5=~BMP[(COL_PIXEL/8)*ROW_xPixel+4];
   Col_num_6=~BMP[(COL_PIXEL/8)*ROW_xPixel+5];
   Col_num_7=~BMP[(COL_PIXEL/8)*ROW_xPixel+6];
   Col_num_8=~BMP[(COL_PIXEL/8)*ROW_xPixel+7];
 */
   
   //Single color,2 bits/pixel 
   Col_num_1 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2,BMP);
   Col_num_2 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+2,BMP);
   Col_num_3 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+4,BMP);
   Col_num_4 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+6,BMP);
   Col_num_5 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+8,BMP);
   Col_num_6 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+10,BMP);
   Col_num_7 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+12,BMP);
   Col_num_8 = Combine_2BitsTo1Bit((COL_PIXEL/8)*ROW_xPixel*2+14,BMP);
  
  // delayMicroseconds(1000); 
   
  }
  
}

void setup()
{
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  
  pinMode(en_74138,OUTPUT);
  pinMode(la_74138,OUTPUT);
  pinMode(lb_74138,OUTPUT);
  pinMode(lc_74138,OUTPUT);
  pinMode(ld_74138,OUTPUT);
  
  
  digitalWrite(en_74138, LOW);
  
  // AVR Port Settings
  DDRD |= 0x78; //Set PIN 3 4 5 6 output  
  
  
  
  
}




void loop()
{

  
 display_martix(Bmp1);

 display_martix(Bmp2); 
 
}



