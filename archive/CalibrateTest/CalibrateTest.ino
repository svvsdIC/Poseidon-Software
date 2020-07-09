//http://www.dynamiterc.com/ProdInfo/Files/DYNM3860_Manual_MULTI.pdf
#include <Servo.h>

Servo esc1, esc2, esc3, esc4;
int throttle = 1350;

void setup() {
  pinMode(13, OUTPUT);
  esc1.attach(6);
  esc2.attach(7);
  esc3.attach(8);
  esc4.attach(9);
  Serial.begin(9600);
  Serial.setTimeout(7);
  Serial.println("0 - Auto calibrate\n1 - Full backward\n2 - Neutral\n3 - Full forward\n4 - LED on\n5 - LED off");
}

void loop() {
  // Create a string variable to store input from serial
  String testString;

  // Read a new string from the serial line when the serial port is available
  if (Serial.available())
  {
    testString = Serial.readString();
    Serial.print("serial input read");
    Serial.print(testString);
    Serial.println("hi");
  }
  else
  {
    //Serial.println("No serial input");
  }
  testString.trim();
  Serial.print(testString);

  // Process string read
  if (testString.indexOf("3")>=0)
  {
    digitalWrite(13, HIGH);
    throttle = 2000;
    Serial.print("full");
  }
  if (testString.indexOf("2")>=0)
  {
    digitalWrite(13, LOW);
    throttle = 1350;
    Serial.print("half");
  }
  if (testString.indexOf("1")>=0)
  {
    digitalWrite(13, HIGH);
    throttle = 700;
    Serial.print("none");
  }
  if (testString.indexOf("0")>=0)
  {
    Serial.println("Auto calibrating");
    digitalWrite(13, HIGH);
    throttle = 2000;
    writeThrottle();
    delay(3000);
    throttle = 1350;
    writeThrottle();
    Serial.println("Done! Restart ESC's in NEUTRAL POSITION.");
    digitalWrite(13, LOW);
  }
  if (testString.indexOf("4")>=0)
  {
    digitalWrite(13, HIGH);
    Serial.println("LED ON");
  }
  if (testString.indexOf("5")>=0)
  {
    digitalWrite(13, LOW);
    Serial.println("LED OFF");
  }
  writeThrottle();

  delay(1000);
}
void writeThrottle()
{
  esc1.writeMicroseconds(throttle);
  esc2.writeMicroseconds(throttle);
  esc3.writeMicroseconds(throttle);
  esc4.writeMicroseconds(throttle);
}
