//Set Monitor to "newline and 9600 baud
//Enter "LEDON" and "LEDOFF" as commands
int MAX_CMD_LENGTH = 10;
char cmd[10];
int cmdIndex;
char incomingByte;

void setup()
{



  //Setup Serial Port with baud rate of 9600
  Serial.begin(9600);


  cmdIndex = 0;
  pinMode(13, OUTPUT);
}

void loop()
{
  if (incomingByte = Serial.available() > 0) {

    char byteIn = Serial.read();
    cmd[cmdIndex] = byteIn;

    if (byteIn == '\n') {
      //command finished
      cmd[cmdIndex] = '\0';
      Serial.println(cmd);
      cmdIndex = 0;

      if (strcmp(cmd, "LEDON")  == 0) {
        Serial.println("Command received: LEDON");
        digitalWrite(13, HIGH);
      } else if (strcmp(cmd, "LEDOFF")  == 0) {
        Serial.println("Command received: LEDOFF");
        digitalWrite(13, LOW);
      } else {
        Serial.println("Command received: unknown!");
      }

    } else {
      if (cmdIndex++ >= MAX_CMD_LENGTH) {
        cmdIndex = 0;
      }
    }
  }

}
