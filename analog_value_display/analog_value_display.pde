import processing.serial.*;    

Serial myPort;                                         
int width = 800;                                       
int height = 400;                                      
int val = 0;

int[] data1 = new int[width];                          
int[] data2 = new int[width];                    
int[] data3 = new int[width];
int newDataPoint1 = 0;                                 
int newDataPoint2 = 0;
int newDataPoint3 = 0;

int[] serialInArray = new int[3]; // Where we'll put what we receive
int serialCount = 0;     // A count of how many bytes we receive
boolean firstContact = false;  // Whether we've heard from the microcontroller

void setup()                                           
{
  size(800, 400);
  frameRate(100);                                      
  smooth();  
  
  //println(Serial.list());                            
  myPort = new Serial(this, Serial.list()[0], 9600); 
}

void draw()                                            
{
  
  background(255,255,255);                             
  stroke(0,0,0);                                       
  strokeWeight(2);   
  
  line(0,height/2,width,height/2);                     
  line(width/4,0,width/4,height);                      
  
  for(int i = 0; i < width-1; i++)                     
  {
    data1[i] = data1[i+1];
    data2[i] = data2[i+1];
    data3[i] = data3[i+1];
  }
  
  data1[width-1] = newDataPoint1;                      
  data2[width-1] = newDataPoint2;
  data3[width-1] = newDataPoint3;
  
  strokeWeight(2);                                     
  
  for(int i = width-1; i > 0; i--)                     
  {
    stroke(255,0,0);
    line(i,data1[i-1], i+1, data1[i]);
    
    stroke(0,255,0);
    line(i,data2[i-1], i+1, data2[i]);
    
    stroke(0,0,255);
    line(i,data3[i-1], i+1, data3[i]); 
  }
  
} 

void serialEvent(Serial myPort) {

  // read a byte from the serial port:
 int inByte = myPort.read();

 if (firstContact == false) {
 
   if (inByte == 'A') {
      myPort.clear();   // clear the serial port buffer
      firstContact = true;  // you've had first contact from the microcontroller
      myPort.write('A');  // ask for more
   }
   
 } else {
   
   // Add the latest byte from the serial port to array:
   serialInArray[serialCount] = inByte;
   serialCount++;
   
   // If we have 3 bytes:
     if (serialCount > 2 ) {
       
      newDataPoint1 = serialInArray[0]+100;
      newDataPoint2 = serialInArray[1]+100;
      newDataPoint3 = serialInArray[2];

     println(newDataPoint1 + "\t" + newDataPoint2 + "\t" + newDataPoint3);
     
      // Send a capital A to request new sensor readings:
      myPort.write('A');
      
      // Reset serialCount:
      serialCount = 0;
     }
   }
}