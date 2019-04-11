/* Poseidon X-Drive prototype code 
 * Version 1.4
 * Released 4/11/19 X-Drive team, originally by Ben Gillet
 * 
 * OVERVIEW
 * This code is designed to run on an Sparkfun RedBoard, as part of the X-Drive prototype for project Poseidon.
 * The RedBoard (Arduino) is wired to a HobbyKing HK TR6A V2 receiver, and 6 Dynamite DYNM3860 electronic speed controllers (ESCs),
 * which are wired to the motors on the prototype.
 * The ESC's are controlled by PWM, and the reciever outputs PWM on the data pin for each "channel".
 * A transmitter is digitally "bound" to the reciever, and is used to control the prototype. The transmitter has two two-axis joysticks. 
 * (each axis is referred to as a "channel", thus making the transmitter a "four-channel" transmitter.)
 * This code, running on the RedBoard, reads the PWM from the reciever, (manually, using interrupts) processes it to find joystick percents,
 * calculates the speed at which each motor must run to drive the craft in the desired direction,
 * and then controls the motors through the ESC's using PWM.
 * 
 * PIN CONFIG:
 * Input pins (from reciever): (LY, LX, RY, RX) <--> (Throttle, Yaw, Roll, Pitch)
 * 4, 5, 2, 3
 * Output pins (to ESCs): (frontLeft, frontRight, backLeft, backRight, vertical thruster A, vertical thruster B)
 * 9, 6, 8, 7, 10, 11
 */

//Know precisely what the behavior is when it changes, what's happenning
//Work on trying to fix it, so it no longer exhibits that behavior
 
#include <PinChangeInt.h>
#include <Utility.h>
#include <TimedAction.h>
#include <Servo.h>

#define NUM_INPINS 4
#define NUM_OUTPINS 6

//Data types to allow sets of input or output values to be passed between subroutines easily
struct outputset {
  float value[NUM_OUTPINS];
};

struct inputset {
  float value[NUM_INPINS];
};

//GLOBAL VARIABLES

//Balancing constants and cutoff % values

const float cX = 1.0;
//cX is for the x-translation algorithm weight in the main motor control algorithm. Default 1.0
const float cY = 1.0;
//cY is for the y-translation algorithm weight in the main motor control algorithm. Default 1.0
const float cR = 1.0;
//cR is for the rotation algorithm weight in the main motor control algorithm. Default 1.0
const float outCutoffConst = 4.0;
//Zero out values in between +/- n% being sent to the motors (ie, don't ever give a motor 1.5% power (just give it 0%))
const float inCutoffConst = 9.0;
//Same as above, but for input. (ie, don't ever accept an input value of 3% (just call it 0%)) Makes centering the joysticks easier.
const float power = 0.5;
//Global power constant. Means: never give a motor more than (n*100)% power.
const float expoConst = 0.7;
//Constant used in the "Expo" algorithm, ranges from [0, 1.0]. 1.0 => No expo (linear/normal controls), 0 => Maximum (very strong) expo.


//PWM ranges
int minInMicros[NUM_INPINS], maxInMicros[NUM_INPINS]; //Left empty for calibration (dynamic PWM ranging per-channel)
//LY, LX, RY, RX
const int minOutMicros = 700, maxOutMicros = 2000; //Constants for the range of the ESC's input
int neutralValue = ((maxOutMicros - minOutMicros) / 2) + minOutMicros;//1350 is the middle, ie, 0% in [-100%, 100%]

//Input
inputset controlValues;
int inPins[NUM_INPINS] = {4, 5, 2, 3};//4, 5, 2, 3
//LY, LX, RY, RX
//timer variables are used by the interrupt handler for reading PWM
volatile unsigned long timerStart[NUM_INPINS];
volatile unsigned long timeValue[NUM_INPINS];

//Output
//Use a Servo object to control the motors (we like writeMicroseconds()...):
Servo motors[NUM_OUTPINS];
outputset motorValues;
int outPins[NUM_OUTPINS] = {9, 6, 8, 7, 10, 11};
//frontLeft, frontRight, backLeft, backRight, vertical thruster A, vertical thruster B
const float motorConst[NUM_OUTPINS] = {0.8, 0.8, 0.8, 0.8, 1.0, 1.0};
//Individual motor calibration constants to allow for balancing if we have a slightly slower motor, etc...

//Other
//Program state booleans
bool printingIn = true, printingOut = true, suspendOutput = false;
float controlPoints = 0.0; //global for easy debugging. See below for control point definition.
int changeMax[6] = {0,0,0,0,0,0}; //change Maximum flag
int changeMin[6] = {0,0,0,0,0,0}; //Change minimum flag
bool changedFlag = false;  //Change the value if the value changes 

//MATH SUBROUTINES
/*
 * Takes a value from -100 to 100, applies "expo" (decreased sensitivity in the middle of the range,
 * increased sensitivity towards the edges), and returns the new value in the same range.
 * The equation for expo is
 * y = (1 - n)x^3 + nx
 * where n is a slope constant in [0, 1].
 */
float calcExpo(float num)
{
  float x = num / 100.0; // map to -1 to 1
  x = ((1.0 - expoConst) * pow(x, 3)) + (expoConst * x);
  x = x * 100.0; // map back to -100 to 100
  return x;
}


//COMMAND IO ROUTINES
/*
 * These functions check for input on the serial line (from a computer connected over USB), interpret it, and set a boolean value
 * for the correspondng output function.
 */
void checkCommands()
{
  if(Serial.available())
  {
    String testString = Serial.readString();
    //Sending a command inverts the state. If printingIn is off (false), sending "in" will turn it on (true), and vice versa...
    if(testString == "in")
    {
      printingIn = !printingIn;
    }
    else if(testString == "out")
    {
      printingOut = !printingOut;
    }
    else
    {
      Serial.println("Command not recognized.");
    }
  }
}

//Check for either output boolean flag, and output the necessary data. This subroutine is called ever second by the protothreader.
void outputTelemetry()
{
  
  //Telemetry output
  if(printingIn)
  {
    Serial.println("----INPUT----");
    Serial.print("Control points: ");
    Serial.print(controlPoints);
    Serial.print("\nLY: ");
    Serial.print(controlValues.value[0]);
    Serial.print("%\nLX: ");
    Serial.print(controlValues.value[1]);
    Serial.print("%\nRY: ");
    Serial.print(controlValues.value[2]);
    Serial.print("%\nRX: ");
    Serial.print(controlValues.value[3]);
    Serial.println("%\n----------");
  }
  if(printingOut)
  {

    
    //Serial.println("----OUTPUT----");
    Serial.println("-----");
    if(suspendOutput)
    {
      Serial.print("Output suspended! Writing ");
      Serial.print(neutralValue);
      Serial.println(" to all motors.");
    }
      if (changedFlag == true)
      {
        Serial.print("\nFrontLeft: ");
        Serial.print(maxInMicros[0]);
        Serial.print("\nFrontRight: ");
        Serial.print(maxInMicros[1]);
        Serial.print("\nBackLeft: ");
        Serial.print(maxInMicros[2]);
        Serial.print("\nBackRight: ");
        Serial.print(maxInMicros[3]);
        Serial.println("\n---max-------");
        Serial.print("\nFrontLeft: ");
        Serial.print(minInMicros[0]);
        Serial.print("\nFrontRight: ");
        Serial.print(minInMicros[1]);
        Serial.print("\nBackLeft: ");
        Serial.print(minInMicros[2]);
        Serial.print("\nBackRight: ");
        Serial.print(minInMicros[3]);
        Serial.println("\n-----min-----");
        Serial.print("Transx");
        Serial.print(controlValues.value[3]);
        Serial.print("Transy");
        Serial.print(controlValues.value[2]);
        Serial.print("Rotation");
        Serial.print(controlValues.value[1]);
        Serial.print("vertical");
        Serial.print(controlValues.value[0]);
        changedFlag = false;
      }
        Serial.println("Transx");
        Serial.println(controlValues.value[3]);
        Serial.println("Transy");
        Serial.println(controlValues.value[2]);
        Serial.println("Rotation");
        Serial.println(controlValues.value[1]);
        Serial.println("vertical");
        Serial.println(controlValues.value[0]);
      Serial.print("Motor value:");
      Serial.println(motorValues.value[0]);
      Serial.println(motorValues.value[1]);
      Serial.println(motorValues.value[2]);
      Serial.println(motorValues.value[3]);
    
  }
}


//RECIEVER INPUT ROUTINES
/*
 * The interrupt#() functions are bound to any pin change events on the pins specified in the inPins array.
 * They all call the same readPWM() subroutine with their channel number (index number of their pin value
 * in the inPins array.
 */

void interrupt0()
{
  readPWM(0);
}
void interrupt1()
{
  readPWM(1);
}
void interrupt2()
{
  readPWM(2);
}
void interrupt3()
{
  readPWM(3);
}

/*
 * readPWM() is called whenever a pin change event occurs. It has two behaviors, one for when the specific
 * pin goes HIGH, and one for when it goes LOW. The HIGH behavior simply records the current millis() for 
 * that channel. The LOW behavior subtracts the value recorded when the pin went HIGH from the current
 * millis() to obtain a duration of the pulse in milliseconds. Mapping and ranging operations are then
 * performed on the value to find the percent value for the control stick corresponding to the pin.
  */

void readPWM(int channel)
{
  unsigned long now = micros();
  
  if(digitalRead(inPins[channel]) == HIGH)
  {
    timerStart[channel] = now;
  } 
  else if(digitalRead(inPins[channel]) == LOW)
  {
    timeValue[channel] = now - timerStart[channel];//the pulse width
  }
}
/*
boolean checkPWM()
{
  if(timeValue[i] > 700 && timeValue[i] < 2100) //prevent out-of-range values from entering the algorithm completely
  {
    return true
  }
  return false
}*/

void calcPWM()
{
  for(int i = 0; i < NUM_INPINS; i++)
  {
    if(timeValue[i] > 700 && timeValue[i] < 2100) //prevent out-of-range values from entering the algorithm completely was 700, 2100
    {
      //Calibration
      if(timeValue[i] > maxInMicros[i])
      {
          changedFlag = true;
          maxInMicros[i] = timeValue[i];
      }
      
      if(timeValue[i] < minInMicros[i])
      {
          changedFlag = true;
          minInMicros[i] = timeValue[i];
      }
      
      //Mapping:
      //Fist, we map from -100 to 100:
      controlValues.value[i] = map(timeValue[i], minInMicros[i], maxInMicros[i], -100, 100);
      if(abs(controlValues.value[i]) < inCutoffConst)
      {
        //zero out values less than the allowed constant right here (to make it easier to "center" the sticks)
        controlValues.value[i] = 0.0;
      }
      //Then, we calculate the expo:
      controlValues.value[i] = calcExpo(controlValues.value[i]);
    }
    else
    {
      Serial.println(i);
      Serial.println(timeValue[i]);
      Serial.println("Out of range!!!!!!!");
    }
  }
}


//ALGORITHM ROUTINES
/*
 * These functions calculate a set of motor values based exclusively on a single channel.
 * Those values are then weighted and combined in calcMotors(). See table in project documentation for further details.
 */
 
outputset calcTransX()
{
  outputset temporaryValues; 
  float val = controlValues.value[3]; //RX
  temporaryValues.value[0] = val;
  temporaryValues.value[1] = -val;
  temporaryValues.value[2] = val;
  temporaryValues.value[3] = -val;
  temporaryValues.value[4] = 0;
  temporaryValues.value[5] = 0;
  return temporaryValues;
}

outputset calcTransY()
{
  outputset temporaryValues; 
  float val = controlValues.value[2]; //RY
  temporaryValues.value[0] = -val;
  temporaryValues.value[1] = -val;
  temporaryValues.value[2] = val;
  temporaryValues.value[3] = val;
  temporaryValues.value[4] = 0;
  temporaryValues.value[5] = 0;
  return temporaryValues;
}

outputset calcRotation()
{
  outputset temporaryValues; 
  float val = controlValues.value[1]; //LX
  temporaryValues.value[0] = -val;
  temporaryValues.value[1] = val;
  temporaryValues.value[2] = val;
  temporaryValues.value[3] = -val;
  temporaryValues.value[4] = 0;
  temporaryValues.value[5] = 0;
  return temporaryValues;
}

outputset calcVertical()
{
  outputset temporaryValues; 
  float val = controlValues.value[0]; //LY
  temporaryValues.value[0] = 0;
  temporaryValues.value[1] = 0;
  temporaryValues.value[2] = 0;
  temporaryValues.value[3] = 0;
  temporaryValues.value[4] = val;
  temporaryValues.value[5] = val;
  return temporaryValues;
}

/*
 * This subroutine takes the sets of channel-exclusive motor values and combines them with a weighted
 * algorithm. A 'control point' is an amount of any direction imposed on a joystick. (if a stick is pushed to
 * 45%, it is worth 45 control points.) These values are absolute, so negative control points are not defined.
 * Each set of values is assigned a weight equal to the proportion of control points exerted on its exclusive
 * channel to the total amount of control points exerted at that time. After calculating the weight, the
 * subroutine builds a master outputset (temporaryValues) with those weights, one motor at a time. It then 
 * overwrites the global outputset with the new one.
 */

void calcMotors()
{
  bool lightOn = false;
  outputset temporaryValues;
  outputset xValues = calcTransX();
  outputset yValues = calcTransY();
  outputset rValues = calcRotation();
  outputset vValues = calcVertical();
  float Jr = abs(controlValues.value[1]);
  float Jy = abs(controlValues.value[2]);
  float Jx = abs(controlValues.value[3]);
  float cpR, cpX, cpY;
  controlPoints = Jr + Jx + Jy;
  if(abs(controlPoints) < 0.1)//Prevent divide by 0 (avoiding floating point comparison ( == 0))
  {
    cpR = cpX = cpY = 0.0;
  }
  else
  {
    cpR = (Jr / controlPoints);
    cpX = (Jx / controlPoints);
    cpY = (Jy / controlPoints);
  }
  
  for(int i = 0; i < NUM_OUTPINS; i++)
  {
    //Calculate the value
    
    temporaryValues.value[i] = ((xValues.value[i] * cpX * cX)
    + (yValues.value[i] * cpY * cY)
    + (rValues.value[i] * cpR * cR)
    + (1 * vValues.value[i]))
    * motorConst[i] * motorConst[i];
    
    //Bounds checking
    temporaryValues.value[i] = constrain(temporaryValues.value[i], -100.0, 100.0);
    //Zeroing out and light behavior (if a motor is on, light is on)
    if(abs(temporaryValues.value[i]) < outCutoffConst)
      temporaryValues.value[i] = 0.0;
//    if(i == 2)
//      Serial.print(temporaryValues.value[2]);
    else
      lightOn = true;
  }
  motorValues = temporaryValues;
  if(lightOn)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}


//MOTOR OUTPUT ROUTINE
/*
 * Reads each motor value (in %) from the global motorValues array, maps it to the microsecond range
 * for the ESC (using the minOutMicros and maxOutMicros constants) and assigns those values to be 
 * written to the PWM pins.
 */
void updateMotors()
{
  calcMotors();
  int val = 0;
  for(int i = 0; i < NUM_OUTPINS; i++)
  {
    val = map(motorValues.value[i], -100, 100, minOutMicros, maxOutMicros);
    if(val <= 0)
      //Serial.print(val);
    if(suspendOutput)
      val = neutralValue;
    motors[i].writeMicroseconds(val);
  }
}


//SETUP ROUTINE
/*
 * TimedAction threads are protothreading units (Google it!) The numerical arguments in the constructors
 * is the frequency in milleseconds at which the handler subroutine (pointed to in second argument) should
 * be called. Setup() initializes the pins and attaches the interrupts.
 */
TimedAction commandThread = TimedAction(1, checkCommands);
TimedAction serialThread = TimedAction(1000, outputTelemetry);
TimedAction outputThread = TimedAction(1, updateMotors);
TimedAction inputThread = TimedAction(1, calcPWM);
//Setup
void setup()
{
  //Pin mode initialization, servo and interrupt attachment:
  
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i < NUM_INPINS; i++)
  {
    pinMode(inPins[i], INPUT);
    //neutralValue is a good starting point because we will be sure to see values above and below it, so the dynamic calibration will work.
    minInMicros[i] = neutralValue;
    maxInMicros[i] = neutralValue;
  }
  for(int i = 0; i < NUM_OUTPINS; i++)
  {
    motors[i].attach(outPins[i]);
  }
  //([whichPin], [handler routine], [listen for this]):
  PCintPort::attachInterrupt(inPins[0], interrupt0, CHANGE);
  PCintPort::attachInterrupt(inPins[1], interrupt1, CHANGE);
  PCintPort::attachInterrupt(inPins[2], interrupt2, CHANGE);
  PCintPort::attachInterrupt(inPins[3], interrupt3, CHANGE);
   
  Serial.begin(9600);
  Serial.setTimeout(7);

  suspendOutput = false;
  Serial.println("Remember to calibrate (move sticks around) before turning on ESC's...");

}

/* * * * * * * * * * * 
 *     MAIN LOOP     * 
 * * * * * * * * * * */

void loop()
{
  commandThread.check();
  serialThread.check();
  outputThread.check();
  inputThread.check();
}
