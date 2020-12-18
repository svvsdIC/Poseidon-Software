#include "SysConfig.h"
#if(HAS_MOTOR_CONTROL)
#include "CMotorControl.h"
#include "NCommManager.h"
#include "joystick.h"

/*
Module responsible for driving the thrusters based on a diamond configuration
Body-Fixed coordinate system - X - forward, Y - right, Z - down, right-handed

Serial commands in:

      mtrctl(channel,value);
      channel values will be:  "TRANX, TRANY, TRANZ, YAW, ROLL, PITCH"
      valuse are -100 to 100 as percentage of max forward/reverse thrust
      
      eg. mtrctl(TRANX,-75)   back at 75%    
		      mtrctl(TRANZ, 50)   down at 50%
*/

CMotorControl::CMotorControl()
{
}

void CMotorControl::Initialize()
{
	for (int i = 0; i < NUM_OUTPINS; i++)
	{
		motors[i].attach(outPins[i]);
	}
	
	// TODO: may change startup message
	Serial.println(F("mtrctl:1;"));
}

void CMotorControl::Update( CCommand& commandIn )
{
	// Handle commands
	if( NCommManager::m_isCommandAvailable && commandIn.Equals( "mtrctl"))
	{
      parseCommand(commandIn);
	}

	// TODO: may want to call less often than every time through the loop
	updateMotors();
}

/*
Parses in the commandIn to extract user supplied control values for
TRANSX, TRANSY, TRANSZ, PITCH, ROLL, YAW.  Puts these values into the 
controlValues inputset.
*/
void CMotorControl::parseCommand( CCommand &commandIn) {
	// Print command details for debugging
	commandIn.PrintDebug();

	// Handle commands (if there are any, otherwise this gets skipped)
	if(commandIn.m_arguments[0] != 2){
		Serial.println("mtrctl:invalid_args;"); //error message
		return;
	}

  // TODO: **** *** **** extract values
}

//MOTOR OUTPUT ROUTINE
/*
 * Reads each motor value (in %) from the global motorValues array, maps it to the microsecond range
 * for the ESC (using the minOutMicros and maxOutMicros constants) and assigns those values to be 
 * written to the PWM pins.
 */
void CMotorControl::updateMotors()
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
//ALGORITHM ROUTINES
/*
 * These functions calculate a set of motor values based exclusively on a single channel.
 * Those values are then weighted and combined in calcMotors(). See table in project documentation for further details.
 */
 
outputset CMotorControl::calcTransX()
{
  outputset temporaryValues; 
  float val = controlValues.value[3]; //RX
  temporaryValues.value[0] = val;         //front left
  temporaryValues.value[1] = -val;        //front right
  temporaryValues.value[2] = val;         //back  left
  temporaryValues.value[3] = -val;        //back  right
  temporaryValues.value[4] = 0;           //vertical A
  temporaryValues.value[5] = 0;           //vertical B
  return temporaryValues;
}

outputset CMotorControl::calcTransY()
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

outputset CMotorControl::calcRotation()
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

outputset CMotorControl::calcVertical()
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
// Notes: We do not believe that control points are positive.  Instead from code inspection, we see that the control point range should be [-100, 100]

void CMotorControl::calcMotors()
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
    
    //Bounds checking --     // TODO: line above might cause us to rotate (as per Pablo)
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

#endif /* HAS_MOTOR_COONTROL */
