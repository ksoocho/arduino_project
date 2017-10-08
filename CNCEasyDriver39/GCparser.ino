
void mParse(String inString){  // procesa comandes de l'estil 'G01 X55.9319 Y197.2523' en UpperCase!!!!!!!!
	int spacePos;
	int gcsc;                    // GCode Sub Command. a G90 , conté el 90
	String gcParam ="                                        ";          // GCode parameters. a G1 X10.123 Y45.789 , conté X10.123 Y45.789 

	spacePos = inString.indexOf(' ');                                 // funciona perque els parametres de substring son unsigned; i per tant finalment -1=length()
	gcsc = strToInt(  inString.substring(1, spacePos )  );            // GCode Sub Command. p.e. a G90 conté el 90
	gcParam = (spacePos > 0) ?inString.substring(spacePos+1) : "";    // GCode Parameters. p.e. a G01 X12.3 conté X12.3

	switch(gcsc){
	case  2: 
		spindle = SP_STOP;  
		sendToHost(ACK);
		sendToHost(HLT);
		break;  // M2 end Program
	case  3: 
		spindle = SP_CW;    
		sendToHost(ACK);  
		break;  // M3 Turn spindle clockwise
	case  4: 
		spindle = SP_CCW;   
		sendToHost(ACK);  
		break;  // M4 Turn spindle counterclockwise 
	case  5: 
		spindle = SP_STOP;  
		sendToHost(ACK);  
		break;  // M5 Stop spindle 
	case 100:
		cncDelay = true;
		sendToHost(ACK);  
		break;										// comanda inventada provoca un delay de 1 segon abans de executar la seguent comanda (M100 delay de 1 s)
	case 101:
		cncDelay = false;
		sendToHost(ACK);  
		break;										// comanda inventada anula el dalay (M101 anula el delay de 1 s)
	default:                     
		sendToHost(ACK);  
		break;
	}
}////



void gParse(String inString){  // procesa comandes de l'estil 'G01 X55.9319 Y197.2523' en UpperCase!!!!!!!!
	int spacePos;
	int gcsc;                    // GCode Sub Command. a G90 , conté el 90
	String gcParam ="                                        ";          // GCode parameters. a G1 X10.123 Y45.789 , conté X10.123 Y45.789 

	spacePos = inString.indexOf(' ');                                 // funciona perque els parametres de substring son unsigned; i per tant finalment -1=length()
	gcsc = strToInt(  inString.substring(1, spacePos )  );            // GCode Sub Command. p.e. a G90 conté el 90
	gcParam = (spacePos > 0) ?inString.substring(spacePos+1) : "";    // GCode Parameters. p.e. a G01 X12.3 conté X12.3

	switch(gcsc){
	case  0: 
		rapidMotion = true;   
		procesaXYZ("G0", gcParam, false);  
		break;  // G0 tots els motors a màxima velocitat (serveix per posicionament)
	case  1: 
		rapidMotion = false;  
		procesaXYZ("G1", gcParam, false);  
		break;  // G1 moviment amb sincronització linial
	case  2: 
		rapidMotion = false;  
		procesaXYZ("G2", gcParam, false);  
		break;  // G2 comanda no acceptada (l'ha de processar el host)
	case  3: 
		rapidMotion = false;  
		procesaXYZ("G3", gcParam, false);  
		break;  // G3 comanda no acceptada (l'ha de processar el host)
	case 21:                       
		sendToHost(ACK);      
		break;
	case 49:                       
		sendToHost(ACK);      
		break;
	case 90: 
		absolutes = true;      
		sendToHost(ACK);      
		break;  // G90 cooredenades absolutes // comanda acceptada
	case 91: 
		absolutes = false;     
		sendToHost(ACK);      
		break;  // G91 coordenades relatives  // comanda acceptada
	case 92:
		procesaXYZ("G92", gcParam, true);
		break;	// G92 Coordinate System Offset
	default:                       
		sendToHost(ACK);      
		break;
	}
}////



void procesaXYZ(String gc, String params, boolean lsetCoordinates){  // entra el punt a la cua
	int spacePos, posX, posY, posZ, posF;
	float mm[3] = {    0.0, 0.0, 0.0     };
	float feedRate = 0;						// feed rate = 0 implica màxima velocitat

	posX = params.indexOf('X');  
	spacePos = params.indexOf(' ', posX);  
	if(posX >= 0)	mm[eixX] = strToFloat(params.substring(posX+1, spacePos));  // Funciona encara que no hi hagi espais (veure GCode)

	posY = params.indexOf('Y');  
	spacePos = params.indexOf(' ', posY);  
	if(posY >= 0)	mm[eixY] = strToFloat(params.substring(posY+1, spacePos));

	posZ = params.indexOf('Z');  
	spacePos = params.indexOf(' ', posZ);  
	if(posZ >= 0)	mm[eixZ] = strToFloat(params.substring(posZ+1, spacePos)); 

	posF = params.indexOf('F');  
	spacePos = params.indexOf(' ', posF);  
	if(posF >= 0)	feedRate = strToFloat(params.substring(posF+1, spacePos)); 

	if( !(posX>=0 || posY>=0 || posZ>=0) ){ 
		sendToHost(ACK); 
	}    // comanda no acceptada no hem trovat ni X ni Y ni Z
	else if( punts.pushPunt(mm, absolutes, rapidMotion, spindle, feedRate, lsetCoordinates, cncDelay) ){		// comanda acceptada

		if(absolutes){
		  for(int n=0;n<3;n++) mm[n] = (mm[n]<0) ? 0: mm[n];			// TODO: no accepta valors negatius
		}
		sendToHost(ACK);
		Serial.print("> "+gc+" ");	Serial.print(mm[0]);	Serial.print(" ");	Serial.print(mm[1]);	Serial.print(" ");	Serial.print(mm[2]);	Serial.print("  \n"); 
	}
	else{																// Ocupat la cua és plena (no s'ha procesat la comanda)
		busy = true;  
		sendToHost(BSY); 
		Serial.print("#Busy  \n");
	}

}////




