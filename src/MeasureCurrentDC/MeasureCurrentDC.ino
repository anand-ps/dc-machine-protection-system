#include "ACS712.h"

float I = 0;

ACS712 DC_current_sensor(ACS712_05B, A0);

void setup() 
{
  
  Serial.begin(9600);
  Serial.println("Calibrating...");
  int zero = DC_current_sensor.calibrate();
  Serial.println("Done!");
  Serial.println("Zero point for this sensor = " + zero);
}

void loop() 
{
  
  I = DC_current_sensor.getCurrentDC();
  
  Serial.println(String("I = ") + I + " A");
  
  delay(500);
}
