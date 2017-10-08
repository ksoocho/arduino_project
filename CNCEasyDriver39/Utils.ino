long  strToInt  (String sNum){  
    return( sNum.toInt()   );  
}
//float strToFloat(String sNum){  
//    return( sNum.toFloat() );  
//}*/ // Atenció!!!! per obtenir 'toFloat()'
// Cal modificar arduino\hardware\arduino\cores\\arduino\WString.h i WString.cpp 
// i afagir toFloat(). O bé:
float strToFloat(String sNum){
  float retorn =0;
  String sEnter = "          ", sFracc = "          ";
  int posPunt = sNum.indexOf('.');
  
  if(posPunt >= 0){
    sEnter = sNum.substring(0, posPunt);
    retorn = (float)sEnter.toInt();                     //(float)strToInt(sEnter);
    
    sFracc = sNum.substring(posPunt+1);
    retorn += (float)sFracc.toInt()/pow( 10.0, sFracc.length() );             //(float)strToInt(sFracc)/pow( 10.0, sFracc.length() 

  }else{
    retorn = (float)sNum.toInt();                   //(float)strToInt(sNum);
  }
  return( retorn );
}




