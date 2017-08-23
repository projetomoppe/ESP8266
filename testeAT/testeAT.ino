void setup() {
  Serial.begin(115200);
  Serial2.begin(115200); // Change this to the baudrate used by ESP8266
  delay(1000); // Let the module self-initialize
}              

void loop() {
  Serial.println("Sending an AT command...");
  Serial2.println("AT");
  delay(30);
  while (Serial2.available()){
     String inData = Serial2.readStringUntil('\n');
     Serial.println("Got reponse from ESP8266: " + inData);
  }  
}
