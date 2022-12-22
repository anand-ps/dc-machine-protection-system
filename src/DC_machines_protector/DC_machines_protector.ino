#include "ACS712.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "PMButton.h"

LiquidCrystal_I2C lcd(0x27, 20, 4); 
Encoder myEnc(2, 3);
PMButton button1(10);

int voltage_sensor_pin = A0;
int current_sensor_pin = A1;
int buzzer_pin         = 8;
int relay_pin          = 5;

//Rotory encoder
int clk_pin       = 2;
int dt_pin        = 3;
int button_pin    = 10;

float I = 0;
float V = 0;

float Ilower = 0.1;
float Iupper = 0.8;
float Vlower = 10.0;
float Vupper = 14.0;

int inrush_tolerance = 20000;

bool btn_clicked_flag = 0;

int settings=0;

unsigned long button_clicked_instant = 0;
unsigned long button_released_instant = 0;

ACS712 DC_current_sensor(ACS712_05B, current_sensor_pin);

void setup() 
{
  
  Serial.begin(9600);
  lcd.begin();
  button1.begin();
    
  button1.debounce(20);
  button1.dcGap(100);
  button1.holdTime(1500);
  
  lcd.backlight();
  lcd.print("Initilising!");

  pinMode(voltage_sensor_pin,   INPUT);
  pinMode(buzzer_pin,           OUTPUT);
  pinMode(relay_pin,            OUTPUT);

  pinMode(clk_pin,              INPUT);
  pinMode(dt_pin,               INPUT);
  pinMode(button_pin,           INPUT);

  
  for(int i=0;i<3;i++)
  {
  digitalWrite(buzzer_pin,HIGH);
  delay(100);
  digitalWrite(buzzer_pin,LOW);
  delay(80);
  }

  Serial.println("Calibrating...");
  int zero = DC_current_sensor.calibrate();
  Serial.println("Done!");
  
  delay(1000);
  lcd.setCursor(0, 1); //col , row
  lcd.print("Motor status: OFF");
  delay(1000);

  lcd.setCursor(0, 1); //col , row
  lcd.print("Motor status: ON ");
  delay(100);

  digitalWrite(relay_pin,HIGH);
  delay(1000);

  lcd.clear();
}

void loop() 
{
  if(btn_clicked_flag)
  {
   switch(settings)
   {
    case 1:
           Serial.println("Inside case 1");
           lcd.clear();
           delay(100);
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set upper cut off");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("I:");
    
           while(btn_clicked_flag)
            {
              Iupper = (myEnc.read()/4)*0.1;
              lcd.setCursor(2, 2); //col , row
              lcd.print(Iupper);

              if(digitalRead(10)==LOW)
              {
               btn_clicked_flag = 0;
               delay(50);
               Serial.println("Breaking");
               break;
              }
            }
            lcd.clear();
            break;

     case 2:
           Serial.println("Inside case 2");
           lcd.clear();
           delay(100);
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set lower cut off");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("I:");
    
           while(btn_clicked_flag)
            {
              Ilower = (myEnc.read()/4)*0.1;
              lcd.setCursor(2, 2); //col , row
              lcd.print(Ilower);

              if(digitalRead(10)==LOW)
              {
               btn_clicked_flag = 0;
               delay(50);
               Serial.println("Breaking");
               break;
              }
            }
            lcd.clear();
            break;

      case 3:
           Serial.println("Inside case 3");
           lcd.clear();
           delay(100);
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set upper cut off");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("V:");
    
           while(btn_clicked_flag)
            {
              Vupper = (myEnc.read()/4);
              lcd.setCursor(2, 2); //col , row
              lcd.print(Vupper);

              if(digitalRead(10)==LOW)
              {
               btn_clicked_flag = 0;
               delay(50);
               Serial.println("Breaking");
               break;
              }
            }
            lcd.clear();
            break;

      case 4:
           Serial.println("Inside case 4");
           lcd.clear();
           delay(100);
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set lower cut off");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("V:");
    
           while(btn_clicked_flag)
            {
              Vlower = (myEnc.read()/4);
              lcd.setCursor(2, 2); //col , row
              lcd.print(Vlower);

              if(digitalRead(10)==LOW)
              {
               btn_clicked_flag = 0;
               delay(50);
               Serial.println("Breaking");
               break;
              }
            }
            lcd.clear();
            break;
  
  }
 
 }

  button1.checkSwitch();
  useButonCheck();
  
  I = DC_current_sensor.getCurrentDC();
  I = I* (-1.0);
  if(I<0.25)
  {
    I=0.0;
  }
  V = analogRead(voltage_sensor_pin);
  V = (V*(5.0 / 1023.0))*((47.0+10.0)/10.0);

  if(settings == 0)
  {
    lcd.setCursor(0, 0); //col , row
    lcd.print("Current:");
    lcd.setCursor(9, 0); 
    lcd.print(I);

    lcd.setCursor(0, 1); 
    lcd.print("Voltage:");
    lcd.setCursor(9, 1); 
    lcd.print(V);
  }

  Serial.print("I=");
  Serial.print(I);
  Serial.print(" | V=");
  Serial.println(V);

  if((I>Iupper || I<Ilower || V>Vupper || V<Vlower) && millis()>inrush_tolerance)
  {
    digitalWrite(relay_pin,LOW);
    digitalWrite(buzzer_pin,HIGH);
    delay(500);
    digitalWrite(buzzer_pin,LOW);   
  }
  
}

void useButonCheck()
{
  if(button1.held())
  {
    Serial.println("button1 was held.");
  }
  
  if(button1.clicked())
  {
    settings++;
    if(settings>4)
    {
      settings=0;
    }
    Serial.print("Settings: ");
    Serial.println(settings);
    
    btn_clicked_flag = 1;
    
    Serial.println("button1 Just clicked.");
  }
  
  if(button1.doubleClicked())
  {
    Serial.println("button1 Just double clicked!");
  }
}
