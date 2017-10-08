void imReady(){  

	if( busyChanged ){  

		busyChanged = false;  

		if(!busy) sendToHost(RDY);           // i am ready again. Informa al Host

	}  

}////




// Is anybody out there?  Pregunta al Host

void areYouThere(){  

	if( millis()- millLastDialog > SERIAL_TIMEOUT ){                 // Si pasat SERIAL_TIMEOUT el host no ha dit res

		hostFound = false;                                             // Potser ja no hi és

	}  

	if( !hostFound ) {                                               // Si el host no hi és

		sendToHost(IAH);                                               // li diem hola

		if(!busy) sendToHost(RDY);                                     // i am ready

		millLastDialog = millis();

	}

}////




void serialEvent() {                        // s'executa si hi s'han rebut bytes

	char inChars[SERIAL_BUFFER_LENGTH];

	int nChars;

	// LLegeix fins LF, inChars char *, 25 buffer size (tamany de inChars)

	nChars = Serial.readBytesUntil(LF, inChars, SERIAL_BUFFER_LENGTH);   // despres de timeout retorna 0

	if(nChars>0){

		if( checksumMatch(inChars, nChars) ){

			inChars[nChars-2] = NUL;  // Final d'string. Eliminem els caracters del checksum (readBytesUntil ja ha eliminat el LF)   

			parseCommand(inChars);

			millLastDialog = millis();

		}

	}

}////




boolean checksumMatch(char *inChars, int nChars){  // calcula l'XOR dels bytes rebuts i comprova si coincideix amb el rebut

	String checkSumHex = "        "; 

	checkSumHex = checkSum(inChars, nChars-2);  // -2 per calcular el checkSum treiem els caracters del checksum rebut


	if( inChars[nChars-2] == checkSumHex[0] && inChars[nChars-1] == checkSumHex[1] ) return(true);

	else Serial.print(checkSumHex); 

	return(false);

}////




String checkSum(char *inChars, int nChars){

	byte checksum = 0;

	String checksumHex = "        ";

	 

	for(int n=0; n<nChars; n++){  

		checksum ^= (byte)inChars[n];  

	}  // calcula checksum amb XOR


	checksumHex = String(checksum, HEX);     // convertei checksum a string hexdecimal

	checksumHex.toUpperCase();  

	checksumHex = (checksumHex.length() == 1) ? '0'+checksumHex : checksumHex;  // 0xD ==> 0x0D . esperem 2 caracters de checksum 


	return(checksumHex); 

}////




void sendToHost(String toSend){

	String checkSumHex = "        ";


       char* cTemp = new char[toSend.length() + 1]; // '+ 1' is for trailing NULL character.

       strcpy(cTemp, toSend.c_str());


	checkSumHex = checkSum(cTemp, toSend.length());  // per .buffer Cal modificar arduino\hardware\arduino\cores\\arduino\WString.h i WString.cpp 

	Serial.print(toSend);

	Serial.print(checkSumHex); 

	Serial.print((char)LF);


        delete[] cTemp;


}////




void sendToHost(char toSend){

	String checkSumHex = "        ";

	checkSumHex = checkSum(&toSend, 1);

	Serial.print(toSend);

	Serial.print(checkSumHex); 

	Serial.print((char)LF);

}///
