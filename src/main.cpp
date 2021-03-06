/*
dorobit force vodu
*/




#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//JOYSTICK
const int SW_pin = 2;
const int X_pin = A0;
const int Y_pin = A1;
const int dry = 614;
const int wet = 225;
const int solenoid = 5;

//PROGRAM VARIABLES
int humidity_limit = 5;
int humidity = 0;
int screen_num = 0;
int water_amount = 0;
int lcd_off = 0;
int sensorVal = 0;
bool humactive = false;


//------------------------------------------------------------

void setup()

{
// JOYSTICK  // RANGE AXES 1 - 1021, MID X507 Y512

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

// SOLENOID
  pinMode(solenoid, OUTPUT);
  digitalWrite(solenoid, HIGH);

  
// LCD

  lcd.init(); 
  lcd.clear();
  lcd.backlight();
}
 

//------------------------------------------------------------

void loop()
{

// ---> MOISTURE SENSOR

int sensorVal = analogRead(A3);
int humidity = map(sensorVal, wet, dry, 100, 0);

Serial.println("REAL HUM");
Serial.println(humidity);
Serial.println("HUM LIMIT");
Serial.println(humidity_limit);
Serial.println("SOLENOID STATE");
Serial.println(digitalRead(5));
Serial.println("HUMACTIVE");
Serial.println(humactive);
Serial.println("sensorval");
Serial.println(sensorVal);
delay(1000);

// ---> INITIAL SCREEN

  if(screen_num == 0){
    lcd.setCursor(0,0);
    lcd.print("Hello Darling !   ");
    lcd.setCursor(0,1);
    lcd.print("       ->   ");}
    
// ---> MENU MOVE

  if(analogRead(Y_pin) < 400){
    screen_num += 1;
  }
  else if(analogRead(Y_pin) > 600){
    screen_num -= 1;
  }
// ---> HUMIDITY LIMIT

  if(screen_num == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity limit %:");
    lcd.setCursor(0,1);
    lcd.print(humidity_limit);
    delay(200);}
    
  if(screen_num == 1 && analogRead(X_pin) < 400){
    lcd.clear();
    lcd.setCursor(0,0);
    humidity_limit += 5;
    lcd.setCursor(0,1);
    lcd.print(humidity_limit);
    delay(200);}
    
  if(screen_num == 1 && analogRead(X_pin) > 600){
    lcd.clear();
    lcd.setCursor(0,0);
    humidity_limit -= 5;
    lcd.setCursor(0,1);
    lcd.print(humidity_limit);
    delay(200);}

  if(screen_num == 1 && digitalRead(SW_pin) == LOW){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity mode");
    lcd.setCursor(0,1);
    lcd.print("  activated");
    delay(200);
    humactive = true;
  }

 if(humactive == true && humidity_limit > humidity){
      digitalWrite(solenoid, LOW);
      delay(200);}
 else
 {
   digitalWrite(solenoid, HIGH);
 }
 
// ---> WATER

  if(screen_num == 2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Water cl:");
    lcd.setCursor(0,1);
    lcd.print(water_amount);
    delay(200);}

  if(screen_num == 2 && analogRead(X_pin) < 400){
    lcd.clear();
    lcd.setCursor(0,0);
    water_amount += 1;
    lcd.setCursor(0,1);
    lcd.print(water_amount);
    delay(200);}
    
  if(screen_num == 2 && analogRead(X_pin) > 600){
    lcd.clear();
    lcd.setCursor(0,0);
    water_amount -= 1;
    lcd.setCursor(0,1);
    lcd.print(water_amount);
    delay(200);}

// ---> SLEEP

  if(screen_num == 3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sleep");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(200);}
    
  if(screen_num == 3 && digitalRead(SW_pin) == 0 && lcd_off %2 == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutting down...");
    lcd.setCursor(0,1);
    lcd.print("");
    lcd_off += 1;
    delay(2000);
    lcd.noBacklight();
    delay(2000);}

    if(lcd_off%2 != 0 && digitalRead(SW_pin) == 0){
    lcd.clear();
    lcd_off += 1;
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome back !");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(2000);
    }

  if(screen_num == 4){
    screen_num = 1;
    delay(200);}
    
  if(screen_num == -1){
    screen_num = 1;
    delay(200);}
}
