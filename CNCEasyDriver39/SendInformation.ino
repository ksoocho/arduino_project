
long milli = 0;



void sendActualPos(){                                      // comanda @   (les coodenades actuals)
    milli=millis();
    char s[12];
    String toSend = "@";                                  // comanda @
    dtostrf(motor[eixX].mmPosActual(), 4, 2, s);  
    toSend += " "+(String)s;  // largada mínima de la cadena '0.23' 4 caracters incloent el punt

    dtostrf(motor[eixY].mmPosActual(), 4, 2, s);  
    toSend += " "+(String)s;

    dtostrf(motor[eixZ].mmPosActual(), 4, 2, s);  
    toSend += " "+(String)s;

    toSend += " "+(String)target.spindle;
    sendToHost(toSend);
}////



void sendNewTarget(){                                      // comanda #  (el punt cap al que estem anant
    Serial.print("#Running: "  );                            // comanda #
    Serial.print(target.p[eixX]); 
    Serial.print(" "); 
    Serial.print(target.p[eixY]); 
    Serial.print(" "); 
    Serial.print(target.p[eixZ]); 
    Serial.print(" F"); 
    Serial.print(motor[2].aSaltar); 
    Serial.print( (target.spindle) ? " . " : "   ");
    Serial.print( (target.absolutes) ? " Abs " : " Rel ");
    Serial.print( (target.rapidMotion) ? " Fast  \n" : " Lin  \n");  
}////



void sendNewCoodinates(){
    Serial.print("#Coodinates: "  );                            // comanda #
	Serial.print(motor[eixX].mmPosActual()); 
    Serial.print(" "); 
	Serial.print(motor[eixY].mmPosActual()); 
    Serial.print(" "); 
	Serial.print(motor[eixZ].mmPosActual()); 
    Serial.print(" \n"); 
}


void sendTargeted(){                                     // comanda *  (el darrer punt ja acabat)
    char s[12];                                            // envia punt al host
    String toSend = "*";                                   // comanda *
    dtostrf(target.p[eixX], 4, 2, s);  
    toSend += " "+(String)s;  // largada mínima de la cadena '0.23' 4 caracters incloent el punt

    dtostrf(target.p[eixY], 4, 2, s);  
    toSend += " "+(String)s;

    dtostrf(target.p[eixZ], 4, 2, s);  
    toSend += " "+(String)s;

    toSend += " "+(String)target.spindle;
    sendToHost(toSend);
}////






