   
    const int sizeOfArray=80; 
    
    unsigned int timeArray[sizeOfArray];
    unsigned int voltageArray[sizeOfArray];
    unsigned int currentArray[sizeOfArray];
        
    int timer=0;
    int counter=0;
    
    void setup(){
              
       Serial.begin(9600); //2400, 4800, 9600, 19200, 38400, 57600 and 115200
 
       delay(100);    
      
    }
    
    void loop(){
    
    
    long int startTime = micros();
    long unsigned int time=0;

    int position =analogRead(0);
    int magnitude =analogRead(1);
    
/*  =================================================================
      Put some values into the arrays
    ================================================================= */
  
    for(int x=0; x<sizeOfArray; x++ ){
      
      time=(micros()-startTime);

      timeArray[x]=time;     
      
      voltageArray[x]=position;
      currentArray[x]=magnitude;

    }
   

   PlottArray(1,timeArray,voltageArray); 
   PlottArray(4,timeArray,currentArray); 
   
   int Time=millis()/10, Volts=analogRead(1);
   
   // test to see how long it takes to send these 3 values
   timer=micros();
   SendData(20, Time, Volts );
   timer=micros()-timer;
    
   SendData(21, Time, Volts );  // timer usually <= 0.58 ms
  
   delay(50);
  }
    
    
 

