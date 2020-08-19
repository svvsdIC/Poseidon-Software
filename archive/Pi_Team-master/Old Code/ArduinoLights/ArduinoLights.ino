String inbyte;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  if (Serial.available() > 0) {
    inbyte = Serial.read();
  }
  
  if (inbyte == "lights_tpow") {
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  
  else if (inbyte == "Nolights_tpow") {
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }

}
