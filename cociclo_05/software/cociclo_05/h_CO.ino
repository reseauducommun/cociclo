
void lectureCO(){
  CO = analogRead(sensorPin); // on prends les valeurs ici!!
    
  Serial.print("CO : ");
  Serial.println(CO); 

  maxCO = _max(CO,maxCO); // le "_" devant le max est nécessaire parce que la fonction max interfère avec une autre fonction de c++ sur le esp8266
  totalCO = CO+totalCO; // Calcul de la moyenne 
  moyCO = totalCO/lectures;
  i++;
}
