void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  int incomingByte = 0;
  
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.write(incomingByte);
  }
}
