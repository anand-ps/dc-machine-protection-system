#include "ACS712.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "PMButton.h"

bool developer_mode = 0;

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

int temp_read = 0;

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
  button1.dcGap(50);
  button1.holdTime(2000);
  
  lcd.backlight();

  if(!developer_mode)
  {
    lcd.clear();
    lcd.setCursor(0, 0); //col , row
    lcd.print(" DC Motor Protector");
    delay(1000);
    lcd.clear();

    lcd.setCursor(0, 0); //col , row
    lcd.print("By Team,");
    delay(500);

    lcd.setCursor(0, 1); //col , row
    lcd.print("Nesrin");
    delay(500);

    lcd.setCursor(0, 2); //col , row
    lcd.print("Reva");
    delay(500);

    lcd.setCursor(0, 3); //col , row
    lcd.print("Anand");
    delay(1500);
  }

    lcd.clear();
    lcd.setCursor(0, 0); //col , row
    lcd.print("Initilising!");
    delay(1000);

  pinMode(voltage_sensor_pin,   INPUT);
  pinMode(buzzer_pin,           OUTPUT);
  pinMode(relay_pin,            OUTPUT);

  pinMode(clk_pin,              INPUT);
  pinMode(dt_pin,               INPUT);
  pinMode(button_pin,           INPUT);

  if(!developer_mode)
  {
    for(int i=0;i<3;i++)
    {
      digitalWrite(buzzer_pin,HIGH);
      delay(100);
      digitalWrite(buzzer_pin,LOW);
      delay(80);
    }
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
           lcd.setCursor(0, 0); //col , row
           lcd.print("Set-1A");
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set upper cut off I");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("I:");
           lcd.setCursor(2, 2); //col , row
           lcd.print(Iupper);
           lcd.blink();
           delay(1000);
           
           temp_read = myEnc.read();
           
           while(btn_clicked_flag)
            {              
              if(myEnc.read() != temp_read)
              {
                Iupper = (myEnc.read()/4)*0.1;
                lcd.setCursor(2, 2); //col , row
                lcd.print(Iupper);
              }

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
           lcd.setCursor(0, 0); //col , row
           lcd.print("Set-1B");
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set lower cut off I");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("I:");
           
           lcd.setCursor(2, 2); //col , row
           lcd.print(Ilower);
              
           temp_read = myEnc.read();
    
           while(btn_clicked_flag)
            {
              if(myEnc.read() != temp_read)
              {
                Ilower = (myEnc.read()/4)*0.1;
                lcd.setCursor(2, 2); //col , row
                lcd.print(Ilower);
              }
              
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
           lcd.setCursor(0, 0); //col , row
           lcd.print("Set-2A");
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set upper cut off V");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("V:");

           lcd.setCursor(2, 2); //col , row
           lcd.print(Vupper);

           temp_read = myEnc.read(); 
    
           while(btn_clicked_flag)
            {
              if(myEnc.read() != temp_read)
              {
                Vupper = (myEnc.read()/4);
                lcd.setCursor(2, 2); //col , row
                lcd.print(Vupper);
              }
              
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
           lcd.setCursor(0, 0); //col , row
           lcd.print("Set-2B");
           lcd.setCursor(0, 1); //col , row
           lcd.print("Set lower cut off V");
           delay(100);

           lcd.setCursor(0, 2); //col , row
           lcd.print("V:");

           lcd.setCursor(2, 2); //col , row
           lcd.print(Vlower);

           temp_read = myEnc.read();
    
           while(btn_clicked_flag)
            {
              if(myEnc.read() != temp_read)
              {
                Vlower = (myEnc.read()/4);
                lcd.setCursor(2, 2); //col , row
                lcd.print(Vlower);
              }
              
              if(digitalRead(10)==LOW)
              {
               btn_clicked_flag = 0;
               delay(50);
               Serial.println("Breaking");
               break;
              }
            }
            lcd.noBlink();
            lcd.clear();
            delay(500);
            lcd.blink();
            lcd.setCursor(6, 1);
            lcd.print("Settings");
            lcd.setCursor(7, 2);
            lcd.print("Saved!");

            delay(500);
            digitalWrite(buzzer_pin,HIGH);
            delay(500);
            digitalWrite(buzzer_pin,LOW); 
            delay(1500);
            
            lcd.noBlink();
            lcd.clear();
            settings = 0;
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

  if((I>Iupper || I<Ilower || V>Vupper || V<Vlower) && I>0)
  {
    digitalWrite(relay_pin,LOW);
    digitalWrite(buzzer_pin,HIGH);
    delay(500);
    digitalWrite(buzzer_pin,LOW);   
  }

 if(settings>0)
 {
  delay(300);
 }
 else
 {
  delay(600);
 }
}

void useButonCheck()
{
  if(button1.held())
  {
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("Restarting...");
      delay(500);
      
      digitalWrite(buzzer_pin,HIGH);
      delay(500);
      digitalWrite(buzzer_pin,LOW);
      digitalWrite(relay_pin,HIGH);
      lcd.clear();
  
      Serial.println("button1 was held.");
  }
  
  if(button1.clicked())
  {
    settings++; // will reset to zero inside switch(4)
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
