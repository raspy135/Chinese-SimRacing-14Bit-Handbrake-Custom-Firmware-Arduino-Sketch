// Alternative Firmaware for Chinese "14 Bit" Sim Racing Handbrake by Daniel Korgel
//
// The 12 and 14 Bit Version of the chineses SimRacing Handbrake use an
// Arduino Pro Micro and a hall sensor.
// 
// This sketch is desigend as a replacement firmware to allow modifcations
// like connecting more buttons to the 
//
// Arduino Joystick Library by Matthew Heironimus
// Download and place in Arduino/library directory: 
// Repository: https://github.com/MHeironimus/ArduinoJoystickLibrary
// Director Download: https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/master.zip
//--------------------------------------------------------------------

//#define DEBUG // uncomment for debug output

#define AXIS_RESOLUTION   4096 //12 Bits axis resolution
#define PIN_HALL_SENSOR   A2
#define PIN_BUTTON_0      2
#define PIN_BUTTON_1      3

#include <Joystick.h>

int32_t minValue = 1000;//507;
int32_t maxValue = 550;

//#define DEBUG

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  2,                      // Button Count
  0,                      // Hat Switch Count
  false, false, true,     // X and Y, but no Z Axis
  false, false, false,    // No Rx, Ry, or Rz
  false, false,            // No rudder or throttle
  false, false, false);   // No accelerator, brake, or steering

void setup() {
  
#ifdef DEBUG
    Serial.begin(9600);
#endif
  //analogReadResolution(12);
  // Initialize Button Pins
  pinMode(PIN_HALL_SENSOR, INPUT);
  
  pinMode(PIN_BUTTON_0, INPUT_PULLUP);
  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
 
  //analogReadResolution(14); // not supported on Arduino Micro, so 14 Bit readout is not possible. 

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setZAxisRange(0, AXIS_RESOLUTION);

  //self calibration:
  for(int i = 0;i < 20;i++){
    int32_t hallSensorValue = 0;
    hallSensorValue += analogRead(PIN_HALL_SENSOR);
    if(hallSensorValue < minValue){
      minValue = hallSensorValue;
    }
    delay(10);
  }
}


void loop() {

  // Read pin values
  Joystick.setButton(0, digitalRead(PIN_BUTTON_0)==LOW);
  Joystick.setButton(1, digitalRead(PIN_BUTTON_1)==LOW);

  int32_t hallSensorValue = 0;
  for(int i = 0;i < 16;i++)
    hallSensorValue += analogRead(PIN_HALL_SENSOR);
  
  hallSensorValue = hallSensorValue >>4;

if(hallSensorValue > maxValue){
    maxValue = hallSensorValue;
  }
  //5 for dead area
  int baseVal = hallSensorValue - minValue - 5;
  if(baseVal < 0) baseVal = 0;
  float zAxisValue = (baseVal)/(float)(maxValue-minValue-5) * AXIS_RESOLUTION;
#ifdef DEBUG
    Serial.print(hallSensorValue);
    Serial.print(" - ");
    Serial.print(minValue);
    Serial.print(" - ");
    Serial.println(zAxisValue);
#endif
  Joystick.setZAxis(zAxisValue);
#ifdef DEBUG
  delay(100);
#else
  delay(3);
#endif
}
