void uploadThingspeak(String thingspeakKey, float fields[]) {

  String startCmd = "AT+CIPSTART=\"TCP\",\"" + domain + "\",80";
  Serial.println(startCmd);
  ESP05.println(startCmd);
  if (ESP05.find("error")) // Fehler aufgetreten?
  {
    Serial.println("Verbindungsfehler");
    errorCondition = true;
  }

  String message = "/update?key=" + thingspeakKey;
  for (int i = 0; i < 8; i++) {
    if(!isnan(fields[i])) {
      message = message + "&field" + (i + 1) + "=" + fields[i];
    }
  }
  String sendCmd = "GET " + message + " HTTP/1.0\r\n\r\n\r\n";
  Serial.print("AT+CIPSEND=");
  //ESP05.print();
  Serial.println(sendCmd.length());
  ESP05.println("AT+CIPSEND=" + String(sendCmd.length()));
   
  //Serial.println("Antwort: " + ESP05.readString());
   
  if(ESP05.find('>'))
  {
    Serial.println(sendCmd);
    ESP05.print(sendCmd);
    delay(1000);

    while(ESP05.available())
    {
      String line = ESP05.readStringUntil('\n');
      if(line.length() == 1)
      {
        line = ESP05.readStringUntil('\n');

        Serial.print("Antwort: ");
        Serial.println(line);
      }
    }
  }
  else
  {
    ESP05.println("AT+CIPCLOSE"); // Verbindung schliessen
    Serial.println("Sendefehler");
    errorCondition=true;
  }
}
