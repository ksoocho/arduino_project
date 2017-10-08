
#include "Arduino.h"   // You need a couple of other things in the header file. 
// One is an #include statement that gives you access to
// the standard types and constants of the Arduino language
// (this is automatically added to normal sketches, but not to libraries).
// It looks like this (and goes above the class definition given previously): 

#define TIMER2_INTERVAL 8.0                         // interval del Timer2 ( 8 us )
//#define sbi(sfr,bit)  ( sfr |=   _BV(bit)  )         // a l'antiga manera. cbi() i sbi() son funcions obsoletes
//#define cbi(sfr,bit)  ( sfr &= ~(_BV(bit)) )         // però les faig servir en una part no crítica en velocitat
#define sbi(port, bit) (port) |= (1 << (bit))       // a l'antiga manera. cbi() i sbi() son funcions obsoletes
#define cbi(port, bit) (port) &= ~(1 << (bit))      // però les faig servir en una part no crítica en velocitat

//A better solution is to eliminate the vibrations by decreasing 
//the distance the rotor must cover on a step-by-step basis. 
//Motors are built to accommodate a step resolution. 
//A 200-step motor moves 1.8 degrees per generated step. 
//If somehow we can divide each step into several microsteps, 
//then the distance traveled is less than 1.8 degrees. 
//With smaller step motions, we need less energy to reach the 
//target position and the vibrations should be minimized. 

class StepperMotor{
public:
    unsigned long  posActual;
    float pendentSaltar;        // Serveix per controlar la velocitat relativa al motor més ràpid
    float aSaltar;              // x100y175 . hem de saltar 175-100 = 75 steps de x . 75/100 = 0.75 hem de saltar 0.75 steps per cada step played.
    boolean halted;
    boolean running;
    int myHIGH;                 // valor a enviar a DirPin quan la direccio es positiva
    int myLOW;
    float stepsPermm;
    unsigned long stepsLeft;    // steps que falten per arribar al destí
	float maxFeedRate;          // en mm / min



    StepperMotor(){
    }////



    float init(byte lStepPin, byte lDirPin, byte ltopePin1, byte ltopePin2, float lstepsPerRev, float lrevPermm, int lmyHIGH, float lV, float lL, float liMax, float lmicroStepping, int laccelDuring){
        float ocr2a;
        unsigned int usDelay;                              // temps d'espera entre Steps en us

        pinMode(lStepPin , OUTPUT);
        pinMode(lDirPin  , OUTPUT);
        pinMode(ltopePin1, INPUT);
        pinMode(ltopePin2, INPUT);

        running = false;
        halted = true;
        posActual = 0;
        pendentSaltar = 0;  
        aSaltar = 0;

        StepPin       = lStepPin;
        DirPin        = lDirPin;
        topePin1      = ltopePin1;                         // Tope -
        topePin2      = ltopePin2;                         // Tope +      
        stepsPerRev   = lstepsPerRev;                      // Steps per volta
        revPermm      = lrevPermm;                         // voltes per mm
        V             = lV;
        L             = lL;
        iMax          = liMax;
        microStepping = lmicroStepping;    
        myHIGH        = lmyHIGH;                           // Estat de dirPin per avançar en direcció positiva  
        myLOW         = (myHIGH == HIGH) ? LOW : HIGH;     // Estat de dirPin per avançar en direcció negativa

        stepsPermm = revPermm * stepsPerRev * microStepping;

        //Rev/sec = V/(L*2*Imax)/(steps/rev)  (Imax en A, L en mili Herns) p.e. 12/(46*2*0.33)/200
        usDelay = L * 2000 * iMax / V / microStepping;     // temps d'espera entre steps en microsegons  ( T= L*Imax*2/V (T miliSegons/step en modus FullStep) )
        ocr2a = ceil((float)usDelay / TIMER2_INTERVAL);    // l'enter mes proper donat l'interval del Timer2 ( 8 us )

		maxFeedRate = 60000.0f / (stepsPermm * ocr2a * TIMER2_INTERVAL / 1000.0f)  ;  // si usDelay = 168 maxFeedRate = 178.57

        return(ocr2a);
    }////



    void run(float lmm){
        if(lmm !=0){
            direccio = lmm/abs(lmm);

            pendentSaltar = 0.0;
            digitalWrite(DirPin, (direccio > 0) ? myHIGH : myLOW);
            running = true;

            stepsLeft = abs(lmm) * stepsPermm;   
        }
    }////



    void abort() {                                     // descarta el destí actual  
        stepsLeft = 0;  
        running = false;
        pendentSaltar = 0;
//        aSaltar = 0;  
    }////

	

    void halt()  {  
        halted = true;   
    }//// parada d'emergencia
    void unHalt(){  
        halted = false;  
    }////



    void playStep(){    
        if(stepsLeft > 0 && pucAvansar(direccio) && !halted){    
            if(int(pendentSaltar) > 0){
                pendentSaltar --;
            }
            else{          
                pendentSaltar += aSaltar;     // aSaltar es el nobre d'Steps que s'ha de saltar per CADA Step
                digitalWrite(StepPin, HIGH);  // envia el pols a la EasyDriver per avançar 1 Step
                digitalWrite(StepPin, LOW);   // A low-to-high transition on the STEP input sequences the translator 
                // and advances the motor one increment.
                // Minimum STEP Pulse Width 1.0 μs (plana 7 de 'A3967.pdf')
                // la intruccio NOP triga 1 Clock ( 0,0625 us = 62.5 ns ) per tant
                // la intrucció digitalWrite ha de trigar més de 16 Clocks
                // mirar el codi a 'wiring_digital.c'
                // segons els forums 'digitalWrite()' sembla trigar entre 6 i 9 us
                // depenent de si el port es PWM o no
                // http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1230286016
                posActual += direccio; //(posActual>0 || direccio>0) ? 
                stepsLeft--;
            }
        }
        if( stepsLeft == 0 ){
            running = false;
        }
    }////



    float mmPosActual(){
        return((float)posActual/(float)stepsPermm);
    }////


	long remainingStepsToAdvanceMm(float lmm){						// igual que mmToSteps, però si no pot avançar, retorna 0
        float steps =  pucAvansar(lmm/abs(lmm)) ? abs(lmm * stepsPermm) : 0;  
        return((long)steps);  
    }////

    long mmToSteps(float lmm){  
        float steps =  abs(lmm * stepsPermm);  
        return((long)steps);  
    }////



private:
    byte StepPin;
    byte DirPin;
    byte topePin1;          // Tope -
    byte topePin2;          // Tope +

    int direccio;
    float revPermm;  

    float microStepping;                                 // Valor establert a la easyDriver ( MS1, MS2 )
    float stepsPerRev;                                   // característica del motor
    float L;                                             // = 46 +- 20% (el pitjor dels casos es 55.2) característica del motor
    float V;                                             // característica del motor
    float iMax;                                          // mesurat amb el tester (cal dividir el resultat per 2 (hi ha 2 bobines))
    // amb la EasyDriver podem controlar iMax abm el potenciòmetre


    boolean pucAvansar(int direccio){ //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! descomentar !!!!!!!!!!!!!!!!!!
//        return(true);                 /////////////////////////// esborrar aquesta línia ///////////////////////////
            switch (direccio){
              case -1:
                if(digitalRead(topePin1)) {
//posActual -= stepsLeft;                    
stepsLeft = 0;
//                  running =false;
                  abort();

                  return(false);
                } 
                else return(true);
              case 1:
                if(digitalRead(topePin2)) {
//posActual += stepsLeft;                    
stepsLeft = 0;
//                  running =false; 
                  abort();
                  return(false);
                } 
                else return(true);
            }
    }////


};////////////////////////////// end class StepperMotor




