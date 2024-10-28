/*
  MAX30105 Breakout: Output all the raw Red/IR/Green readings
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Outputs all Red/IR/Green values.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
*/

#include <Arduino.h>
#include <SPI.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "MAX30105.h"
#include <ESP32_SoftWire.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

SoftWire mpu6050;
TwoWire I2CDisplay = TwoWire(0);
SoftWire I2CSensor;

MAX30105 particleSensor;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CDisplay, OLED_RESET);

#define debug Serial //Uncomment this line if you're using an Uno or ESP
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21

WiFiMulti wifiMulti;

const unsigned char heart_logo [] PROGMEM = {
	0x00, 0x06, 0x00, 0xc0, 0x00, 0x1c, 0x19, 0x55, 0x30, 0x70, 0x23, 0x22, 0xaa, 0x89, 0x88, 0x20, 
	0xd5, 0x55, 0x56, 0x08, 0x20, 0x28, 0x00, 0x28, 0x08, 0x20, 0x27, 0xff, 0xc8, 0x08, 0x20, 0x18, 
	0x00, 0x30, 0x08, 0x20, 0x65, 0xff, 0x4c, 0x08, 0x21, 0x9e, 0xfe, 0xf3, 0x08, 0x22, 0x7f, 0x01, 
	0xfc, 0x88, 0x25, 0x5f, 0x6d, 0xf5, 0x48, 0x4b, 0xbf, 0x6d, 0xfb, 0xa4, 0x47, 0xbf, 0x6d, 0xfb, 
	0xc4, 0x97, 0x7f, 0xef, 0xfd, 0xd2, 0x0f, 0x7f, 0xff, 0xfd, 0xe0, 0x0e, 0xc3, 0xff, 0x86, 0xe0, 
	0x0e, 0xbd, 0xff, 0x7a, 0xe0, 0x08, 0xb8, 0xfe, 0x3a, 0x20, 0x0e, 0xb0, 0xfe, 0x1a, 0xe0, 0x08, 
	0xd0, 0x7c, 0x16, 0x20, 0x0e, 0xd2, 0x7c, 0x96, 0xe0, 0x0e, 0xe8, 0x7c, 0x2e, 0xe0, 0x8e, 0xf0, 
	0xee, 0x1e, 0xe0, 0x9e, 0xff, 0xc7, 0xfe, 0xf2, 0x48, 0x1f, 0xff, 0xf0, 0x24, 0x27, 0x7e, 0x6c, 
	0xfd, 0xc8, 0x27, 0xbe, 0x00, 0xfb, 0xc8, 0x13, 0xbe, 0xba, 0xfb, 0x90, 0x09, 0xde, 0xfe, 0xf7, 
	0x20, 0x04, 0xe7, 0x7d, 0xce, 0x40, 0x02, 0x79, 0x83, 0x3c, 0x80, 0x01, 0x1e, 0x00, 0xf1, 0x00
};
/*
String wifiName = "ASUS";
String wifiPass = "5125#mumbai400063";
String internet = "20.1.1.65:8080";
*/
String wifiName = "Ritik";
String wifiPass = "ritik123";
String internet = "172.20.10.2:8080";
bool screenOn = false;

void setup()
{
  
  mpu6050.begin(5, 6);
  I2CDisplay.begin(20, 10);
  I2CSensor.begin(8,9);

  delay(3000);
  debug.begin(9600);
  
  int screenTime = 3;

  while(screenTime > 0) {
    
    bool begin = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    if(begin){
      screenOn = true;
      break;
    }
    Serial.println(F("SSD1306 allocation failed"));
    delay(1000);
    screenTime--;
  }



  if(screenOn){
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Starting"));
    display.display();
  }
  else{
    Serial.println("Continue without screen");
  }


  debug.println("Setup");
  
  debug.println("MAX30105 Basic Readings Example");
  
  while(particleSensor.begin(I2CSensor, 100000) == false){
    debug.println("MAX30105 was not found. Please check wiring/power. ");
    delay(1000);
  }

  debug.println("Setup sensor");

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive

  mpu6050.beginTransmission(0x68);
  mpu6050.write(0x6B);  // PWR_MGMT_1 register
  mpu6050.write(0);     // set to zero (wakes up the MPU-6050)
  mpu6050.endTransmission(true);

  wifiMulti.addAP(wifiName.c_str(), wifiPass.c_str());
  //wifiMulti.addAP("Ritik", "ritik123");

  xTaskCreatePinnedToCore(
    secondTask,   // Function to implement the task
    "Task2",      // Name of the task
    10000,        // Stack size in words
    NULL,         // Task input parameter
    1,            // Priority of the task
    NULL,         // Task handle
    0);           // Core where the task should run
  
}

float average = 0;
float power = 0.2;
float dist = 7;

float skin = 20000;

int prevMax = 1;

int above = 0;
int below = 0;



bool beat = false;

void loop()
{
  
  int val = particleSensor.getRed();

  average = (average * (1 - power) / 1) + power * val;

  
  //Serial.print("AVG: ");
  //Serial.print(average);
  //Serial.print(", VAL: ");
  //Serial.print(val);
  
  //Serial.print(skin);
  //Serial.print(" ");
  //Serial.println(average);

  if(average > skin){

    if(abs(val - average) > dist){

      if(val > average){
        above++;
        below = 0;
        if(above > 0){
          prevMax = 1;
        }
      }
      if(val < average){
        below++;
        above = 0;
        if(below > 0){
          if(prevMax == 1){
            beat = true;
          }
          prevMax = 0;
        }
      }

    }

  }

  
  
  //Serial.print(" BELOW: ");
  //Serial.print(below);
  //Serial.print(" ABOVE: ");
  //Serial.print(above);
  //Serial.println("");
  
  //debug.println();
  
  delay(50);
}

String id = "Jayven Huang";
String name = "Ritik J.";

int trial = 0;

bool firstConnect = true;

int frame = 0;
int meterSize = 19;
int beats[50] = {0,0,0,0,0,0,0,0,0,0};

void secondTask(void * parameter){
  while(true){
if ((wifiMulti.run() == WL_CONNECTED)) {

    if(firstConnect && screenOn){
      firstConnect = false;
      display.clearDisplay();
      display.setRotation(2);
      display.setTextSize(2); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println(name);

      display.setTextSize(1);
      display.setCursor(44, 46);
      display.println(F("HeartForce"));


      display.drawBitmap(
      0,
      27,
      heart_logo, 39, 32, 1);

      display.display();      // Show initial text
    }

    Serial.println("Connected");
    
    HTTPClient http;
    http.setTimeout(5000);

    StaticJsonDocument<500> doc;
    doc["id"] = id;
    doc["beat"] = true;
    doc["force"] = 0;
    String jsonData;
    serializeJson(doc, jsonData);

    StaticJsonDocument<500> activedoc;
    doc["id"] = id;
    doc["beat"] = false;

    Serial.print("[HTTP] begin...\n");
    String jsonDataActive;
    serializeJson(doc, jsonDataActive);

    doc["beat"] = true;

    http.begin("http://" + internet + "/esp32");

    int y = 41;
    int x = 44;
    int deltaX = 4;
    int peak = 17;

    while(true){
      if(screenOn){
        if(frame == meterSize){
          
          //Serial.println("Reset");

          for(int i = 0; i < meterSize; i++){
            if(beats[i] == 0){
              display.drawLine(x + i * deltaX, y, x + (i+1) * deltaX, y, SSD1306_BLACK);
            }
            else{
              display.drawLine(x + i * deltaX, y, x + (i+0.5) * deltaX, y-peak, SSD1306_BLACK);
              display.drawLine(x + (i+0.5) * deltaX, y-peak, x + (i+1) * deltaX, y, SSD1306_BLACK);
              beats[i] = 0;
            }
          }


        }
        else{
          if(beat){
            beats[frame] = 1;
          }
          if(beats[frame] == 0){
            display.drawLine(x + frame * deltaX, y, x + (frame+1) * deltaX, y, SSD1306_WHITE);
          }
          else{
            display.drawLine(x + frame * deltaX, y, x + (frame+0.5) * deltaX, y-peak, SSD1306_WHITE);
            display.drawLine(x + (frame+0.5) * deltaX, y-peak, x + (frame+1) * deltaX, y, SSD1306_WHITE);
          }
        }

        display.display();
      }

      if(beat){

        mpu6050.beginTransmission(0x68);
        mpu6050.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
        mpu6050.endTransmission(false);
        mpu6050.requestFrom(0x68,14,true);  // request a total of 14 registers
        int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
        AcX=mpu6050.read()<<8|mpu6050.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
        AcY=mpu6050.read()<<8|mpu6050.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        AcZ=mpu6050.read()<<8|mpu6050.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        Tmp=mpu6050.read()<<8|mpu6050.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
        GyX=mpu6050.read()<<8|mpu6050.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
        GyY=mpu6050.read()<<8|mpu6050.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
        GyZ=mpu6050.read()<<8|mpu6050.read();
        
        doc["force"] = (int) sqrt(AcY * AcY + AcX * AcX + AcZ * AcZ);
        //Serial.println("GOT FORCE");
        Serial.println(AcZ);
        String jsonData;
        serializeJson(doc, jsonData);

        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonData);
        Serial.println(httpResponseCode);
        http.end();
        beat = false;
      }
      else if(average > skin){
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonDataActive);
        Serial.println(httpResponseCode);
        http.end();
      }

      frame++;
      frame = frame % (meterSize + 1);

      delay(100);

    }
    

    
  }
  else{

    firstConnect = true;

    String connectingStr = "Pairing";

    for(int i = 0; i < 4; i++){
      if(trial % 4 > i){
        connectingStr += ".";
      }
    }
    if(screenOn){
      display.clearDisplay();
      display.setRotation(2);
      display.setTextSize(1.85); // Draw 2X-scale text
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println(connectingStr);

      display.display();      // Show initial text
    }
    Serial.println("CONNECTING...");
    trial++;
  }

  delay(1500);
}
}


