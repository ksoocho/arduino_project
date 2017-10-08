
float ocr2a;                                         // valor de OCR2A ( que depèn de la velocitat màxima del Stepper )
int ocr2aIni;                                        // valor inicial de OCR2A donada l'acceleració
int downCounter;                                     // el nombre d'Steps que falten fins acabar la desacceleració
int upCounter;                                       // el nombre d'Steps que falten fins acabar l'acceleració  
int accelDuring = 10;                                // Accelera i desaccelera durant 20 Steps
// el temps que triga aquests 20 steps s'ha de
// restar del maxim delay del Timer2
//boolean cncRunning = false;


void initCNC(){
	int accelDelta = 1;                                  // valor en que varia OCR2A durant l'acceleració 
	// ( p.e. si ha de pasar de 255 a 19 en 10 steps decremeta de 23 en 23 i começa a 249 )
	float ocr2aX;
	float ocr2aY;
	float ocr2aZ;
	//                   init(byte    , byte   , byte     , byte     , float       , float    , int    , float, float, float, float         , int         )
	//                   init(lStepPin, lDirPin, ltopePin1, ltopePin2, lstepsPerRev, lrevPermm, lmyHIGH, lV   , lL   , liMax, lmicroStepping, laccelDuring)
	ocr2aX = motor[eixX].init( 6      ,  7     , 14       , 15       , 200         , 1.25     , LOW    , 12   , 70   , 0.135, 8             , accelDuring );  // M5 --> 0,8 mm/Volta --> 1.25 Voltes/mm
	ocr2aY = motor[eixY].init( 8      ,  9     , 16       , 17       , 200         , 1.25     , LOW    , 12   , 70   , 0.135, 8             , accelDuring );
	ocr2aZ = motor[eixZ].init(10      , 11     , 19       , 18       , 200         , 1.25     , HIGH   , 12   , 70   , 0.135, 8             , accelDuring ); 

	ocr2a = max( ocr2aX, max( ocr2aY, ocr2aZ) );

	accelDuring = min( accelDuring, 254-ocr2a );
	ocr2aIni = ocr2a + accelDelta * accelDuring;       // calcula el OCR2A inicial donada l'acceleració

	downCounter = accelDuring;                         // el nombre d'Steps que falten fins acabar la desacceleració
	upCounter = accelDuring;                           // el nombre d'Steps que falten fins acabar l'acceleració

	setTimer2(ocr2aIni);                               // inicia el Timer2 per anar a la màxima velocitat

	//allMotorsHome();
}////



void runNextPoint(){
//    boolean lrunning =false;
	float mm[3] = {  0.0, 0.0, 0.0  };
	boolean labsolutes, lrapidMotion;
	int lspindle;
	float lfeedRate;
	boolean lsetCoordinates;
	boolean ldelay;

	if(!cncStepsLeft){
		if( punts.popPunt(mm, &labsolutes, &lrapidMotion, &lspindle, &lfeedRate, &lsetCoordinates, &ldelay) ){     // Si hi ha un nou punt a la cua
			programStarted = true;
			sendToHost(PLY);
			if(ldelay){
				delay(2000);
			}
			sendTargeted();
			target.set(mm, labsolutes, lrapidMotion, lspindle, lfeedRate, lsetCoordinates, ldelay);
			/*sendNewTarget();*/

			if(busy){ 
				busy = false;  
				busyChanged = true;  
			}                   // pot rebrer més dades

			if(lsetCoordinates){
				setCoordinates(mm);
			}else{
				if(labsolutes)                                                    // si el punt és en coordenades absolutes
					aCoordenadesRelatives(mm);                                      // només enten coordenades relatives

				calculaSalts(lrapidMotion, mm, lfeedRate);                                   // sincronitza els motors (si cal)

				sendNewTarget();
				goTo(mm);                                                         // ves al nou punt
			}
		}else{					// el programa ha acabat
			if( programStarted ) {
				sendToHost(END);	// li ho diu al host
				programStarted = false;
			}
		}
	}else{
		sendActualPos();
	}
}////



void goTo(float mm[]){
	OCR2A = ocr2aIni;                                                     // reinicialitza el contador (permet acceleració)
	upCounter = accelDuring;
	downCounter = accelDuring;      
	unsigned long lstepsLeft = 0;
	
	for(int i = 0; i < 3; i++){  
		motor[i].run(mm[i]);                                                // go to punt
		lstepsLeft += motor[i].stepsLeft;
	}
	cncStepsLeft = (lstepsLeft == 0) ? false : true;
	//cncRunning = true;
}////



void aCoordenadesRelatives(float mm[]){
	for(int i = 0; i < 3; i++){  
		mm[i] = mm[i] - motor[i].mmPosActual();                             // transforma a coodenades relatives
	} 
}////



void calculaSalts(boolean lrapidMotion, float mm[], float lfeedRate){
	if(lrapidMotion){                                                     // si és un punt de posicionament (G0)
		for(int i = 0; i < 3; i++){
			motor[i].aSaltar = 0;                                             // tots els motors a velocitat màxima
		}
	}else{                                                                // si no és G0 el moviment ha de ser linial
		// p.e. G1 x100 y175 . hem de saltar 175-100 = 75 steps de x . 
		// 75/100 = 0.75 hem de saltar 0.75 steps per cada step played.
		// s'ha de calcula en pasos ( ja que els motors poden ser diferents entre si )
		float dist[3] = {  
			motor[eixX].remainingStepsToAdvanceMm(mm[eixX]),							// igual que mmToSteps, però si no pot avançar, retorna 0
			motor[eixY].remainingStepsToAdvanceMm(mm[eixY]), 
			motor[eixZ].remainingStepsToAdvanceMm(mm[eixZ])          };                  // la distància que s'ha de moure en cada eix en pasos

		float distMax = max( max(dist[eixX], dist[eixY]), dist[eixZ] );
		for(int i = 0; i < 3; i++){  
			motor[i].aSaltar = (dist[i]!=0) ? (distMax-dist[i])/(dist[i]) : 0;// Sincronitza els motors
			if(lfeedRate > 0 && lfeedRate < motor[i].maxFeedRate) 
				if(motor[i].aSaltar != 0) motor[i].aSaltar = motor[i].aSaltar * motor[i].maxFeedRate / lfeedRate;	// si maxFeedRare = 175 i feedRate = 10 he de saltar 17,5 vegades més steps
				else					  motor[i].aSaltar = motor[i].maxFeedRate / lfeedRate;                       // ha de saltar 17,5 steps per step played
		}
	}
}////



void setCoordinates(float mm[]){
	for(int i = 0; i < 3; i++){
		motor[i].posActual = motor[i].mmToSteps(mm[i]);
	}
	sendNewCoodinates();
}////



void halt()  {  
	for(int n = 0; n<3; n++) motor[n].halt();    
}////
void unHalt(){  
	for(int n = 0; n<3; n++) motor[n].unHalt();  
}//// 

void allMotorsHome(){
	unsigned long lstepsLeft = 0;

	Serial.print("#Homing... "  );
	for(int i = 0; i < 2; i++){
		motor[i].aSaltar = 0;      
		motor[i].run(-100000);
		lstepsLeft += motor[i].stepsLeft;
	}
	cncStepsLeft = (lstepsLeft == 0) ? false : true;
	//cncRunning = true;
}////




ISR(TIMER2_COMPA_vect){                                                   // interrupció Compara A del Timer2
	unsigned long lstepsLeft = 0;
	
	if(cncStepsLeft){
		for(int i = 0; i < 3; i++){  
			if(motor[i].stepsLeft)  motor[i].playStep();  
			lstepsLeft += motor[i].stepsLeft;
		}
		cncStepsLeft = (lstepsLeft == 0) ? false : true;                         // si tots els motors han acabat pot processar el següent punt

		if(lstepsLeft == downCounter){                                        // ha de desaccelerar
			downCounter--;
			OCR2A ++;                                                         // incrementa el temps entre Steps en CS22:0 us (el proper Step es produirà 8 us mes tard)
		}
		else if(upCounter >0){                                                // ha d'accelerar
			upCounter--;
			OCR2A --;                                                         // decrementa el temps entre Steps en CS22:0 us (el proper Step es produirà 8 us abans)
		}
	}
}////



void setTimer2(int lusDelay) {
	//http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM
	// The ATmega328P has three timers known as Timer 0, Timer 1, and Timer 2
	// Each timer has two output compare registers 
	// Each of the timers has a prescaler that generates the timer clock by dividing
	//         the system clock by a prescale factor such as 1, 8, 64, 256, or 1024
	//         Note that Timer 2 has a different set of prescale values from the other timers
	// The Arduino has a system clock of 16MHz and the timer clock frequency will be
	//         the system clock frequency divided by the prescale factor
	// The timers can also generate interrupts on overflow and/or match against either output compare register
	//
	// TCCRnA and TCCRnB. The Timer/Counter Control Registers hold the main control bits
	//            for the timer. (Note that TCCRnA and TCCRnB do not correspond to the outputs A and B.) 
	// TCCRnA and TCCRnB hold several groups of bits: 
	//     Waveform Generation Mode bits (WGM): these control the overall mode of the timer.
	//         (These bits are split between TCCRnA and TCCRnB.) 
	//     Clock Select bits (CS): these control the clock prescaler 
	//     Compare Match Output A Mode bits (COMnA): these enable/disable/invert output A 
	//     Compare Match Output B Mode bits (COMnB): these enable/disable/invert output B 
	//
	// OCRnA and OCRnB. The Output Compare Registers set the levels at which outputs A and B will be affected
	//
	// TIMSK2 – Timer2 Interrupt Mask Register
	//     OCIE2B: Timer2 Output Compare Match B Interrupt Enable
	//     OCIE2A: Timer2 Output Compare Match A Interrupt Enable
	//     TOIE2:  Timer2 Overflow Interrupt Enable
	//
	// Timer 1 is a 16-bit timer and has additional modes. Timer 2 has different prescaler values
	//
	// The Arduino uses Timer 0 internally for the millis() and delay() functions
	// OC0A pin  6   OC0B pin  5
	// OC1A pin  9   OC1B pin 10
	// OC2A pin 11   OC2B pin  3 

	cli();                           // plana 10. Bit 7 Clear interrupts

	cbi(TCCR2A, COM2A0 );            // plana 158. Table 17-2. Normal port operation, OC0A disconnected. 
	cbi(TCCR2A, COM2A1 );            // 00 deconecta el pin A del timer2

	cbi(TCCR2A, WGM20  );            // TCCR2A Regitre A de control del timer 2
	sbi(TCCR2A, WGM21  ); 	     // plana 160 i 149. Clear Timer on Compare
	cbi(TCCR2B, WGM22  );            // WGM2 = 010 ==> mode of operation Clear Timer on Compare (CTC)
									 // The counter value TCNT2 increases until TCNT2 == OCR2A, 
									 // and then counter (TCNT2) is cleared
									 // repeteix el cilce 0 --> valor ( valor clocks )

	sbi(TCCR2B, CS22   );            // TCCR2B Regitre B de control del timer 2
	cbi(TCCR2B, CS21   ); 	     // Plana 162
	sbi(TCCR2B, CS20   );            // CS2 = 101 prescaler = 128 ==> 128/16.000.000 = 0.000008 s-1 = 8us-1
									 // màxim temps que es pot contar = 8 * 255 = 2040 us

	sbi(TIMSK2, OCIE2A  );           // Timer2 Output Compare Match A Interrupt Enable. plana 163

	cbi(ASSR, AS2);	             // AS2 = 0 clocked from the I/O clock
									 // plana 164. Asynchronous Status Register
									 // When AS2 is written to zero, Timer/Counter2 is clocked from the I/O clock, clkI/O.
									 // When AS2 is written to one, Timer/Counter2 is clocked from a crystal Oscillator
									 // connected to the Timer Oscillator 1 (TOSC1) pin.    

	OCR2A = lusDelay;                //  si p.e. OCR2A = 21 --> 21*0.000008 = 168us-1  . cridem la funcio un cop cada 168 us. plana 162

	TCNT2 = 0;                       // reset Timer2 counter

	sei();                           // Enable Interrupts

}////



