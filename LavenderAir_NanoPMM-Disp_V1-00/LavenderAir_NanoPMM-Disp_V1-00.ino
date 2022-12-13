/* VioletAir NanoPMS Sensor Code
 * (Open-source clone of PurpleAir Sensor)
 * Version 0.10
 * Copyright (C) Brian & Eric Woo-Shem
 * Updated: 2022-12-12
 * 
 *  This program is free software: you can redistribute it and/or modify it under the terms of the 
 *  GNU General Public License version 3 as published by the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 *  even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 *  GNU General Public License at https://www.gnu.org/licenses/gpl-3.0.en.html for more details.
 * 
 * Required Libraries:
 * - Adafruit Unified Sensor
 * - DHTlib
 * These can be installed under Tools -> Manage Libraries
 * 
 * Partially based on the Adafruit_PM25AQI library for Arduino, (C) 2012, Adafruit Industries under BSD License
 * https://github.com/adafruit/Adafruit_PM25AQI
 * 
 * Other Sources:
 * PurpleAir "Using PurpleAir data" Google Doc
 * https://docs.google.com/document/d/15ijz94dXJ-YAZLi9iZ_RaBwrZ4KtYeCy08goGBwnbCU/edit
 * Brendon Baumgartner, 2020 "DIY PurpleAir Sensor," BB's Blog
 * https://blog.brendon.net/diy-purpleair-sensor/
 * Daniel Ross, 2021, "How To Make an Arduino Air Quality Sensor," VueVille
 * https://www.vueville.com/arduino/arduino-air-quality-sensor/
 * Data types - W3Schools - for reducing memory requirement
 * https://www.w3schools.com/java/java_data_types.asp
 * Minuka Thesath Yapa, 2021, "DHT11 & DHT22 Sensors Temperature using Arduino"
 * https://create.arduino.cc/projecthub/MinukaThesathYapa/dht11-dht22-sensors-temperature-using-arduino-b7a8d6
 * Arduino Get Started "Arduino - LCD I2C"
 * https://arduinogetstarted.com/tutorials/arduino-lcd-i2c
 */

#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h>
#include "RTClib.h"

// Settings: Change to false if not using that component
const bool useSerial = true;
const bool useDHT = true;
const bool useDisplayLCD = false;
const bool useDisplayI2C = true;
//const bool useSeparateLED = false; //not implemented yet
const bool useColorLED = true;
const bool debugMode = false;
bool useRTC = true;


// Pin Definitions

// PM Sensor Pins
const byte SET = 2;
const byte TX = 8;

//DHT22 - MUST COMMENT OUT this portion if not using a DHT!
const byte DHT_PIN = 3;
#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22  //Change to DHT11 if using other type
DHT dht(DHT_PIN, DHTTYPE); //// Initialize DHT sensor using library

//RGB LED (needs PWM pins)
const byte redPin = 11;
const byte greenPin = 10;
const byte bluePin = 9;
// Separate LED's
/*
const byte greenLED = 4;
const byte yellowLED = 5;
const byte orangeLED = 6;
const byte redLED = 7;
const byte purpleLED = 12;
*/
//Declare display that uses I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
// Switch with this line if using regular LCD
//LiquidCrystal lcd(9, 8, 5, 4, 3, 2); // Set LCD Screen pins
RTC_DS1307 rtc; // Define RTC (Real Time Clock)

// How often to take readings
const short readInterval = 20000;
// How long to display things for
const short disptime = 4000;

// Time tracking variables
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long tdisp = 0; //Time tracking to change between display screens
byte lcdScreen = 0;

//Current PM2.5 AQI reading
float pm2d5 = 0.0;

//For 10min average PM2.5 AQI
const int numPer10min = int(600000/readInterval);
float pm2d5_10min[numPer10min];
short r = 0;
float pm2d5_10min_avg = 0;

//DHT Data
float tempC = 0;
float tempF = 0;
float hum = 0;

//Starts PMS Sensor read-in serial
#include <SoftwareSerial.h>
SoftwareSerial PMS_raw(TX, 9);

// Create data type to hold input from PMS5003
struct PMS_DATA_RETURN {
  // Since ArduinoIDE code indexes from 0, the length is 15.
  uint16_t framelen;          // [0]
  uint16_t pm1d0_standard;    // [1]
  uint16_t pm2d5_standard;    // [2]
  uint16_t pm10d0_standard;   // [3]
  uint16_t pm1d0_env;         // [4]
  uint16_t pm2d5_env;         // [5]
  uint16_t pm10d0_env;        // [6]
  uint16_t particleCT_0d3;    // [7]
  uint16_t particleCT_0d5;    // [8]
  uint16_t particleCT_1d0;    // [9]
  uint16_t particleCT_2d5;    // [10]
  uint16_t particleCT_5d0;    // [11]
  uint16_t particleCT_10d0;   // [12]
  uint16_t unused;            // [13]
  uint16_t checksum;          // [14]
};

// Initialize data structure to hold most recent reading
struct PMS_DATA_RETURN pd;

void setup() {
  // Serial to computer. 
  if (useSerial){
    // Using a different baud rate (NOT 9600) so it's not conflicting with the sensor.
    Serial.begin(19200);
    Serial.println("VioletAir Particulate Matter Sensor\nFirmware v0.02\n(C) 2022 Eric Woo-Shem & Brian Woo-Shem\n");
    Serial.print("Initializing... ");
  }

  //Set color LED pins if used
  if (useColorLED){
    //declaring the color pins for the RGB LED
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    // LED Circuit test for debugging
    if (debugMode){ 
      RGB_color(255,0,0);
      delay(1000);
      RGB_color(0,255,0);
      delay(1000);
      RGB_color(0,0,255);
      delay(1000);
      RGB_color(0,0,0);
    }
  }

  if(useDHT){
    dht.begin();
  }

  if(useDisplayI2C){
    lcd.init(); // initialize the lcd
    lcd.backlight();
  }
  if(useDisplayLCD){
    digitalWrite(7, HIGH); // Set LCD brightness, you can change it if you want to
    analogWrite(6, 120); // Setting LCD contrast, you can change it if you want to
  }
  //Startup screens
  if(useDisplayI2C||useDisplayLCD){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VioletAir PM Sensor");
    lcd.setCursor(0,1);
    lcd.print("Firmware V0.02");
    delay(1234);
    lcd.setCursor(0,0);
    lcd.print("(C)Eric Woo-Shem");
    lcd.setCursor(0,1);
    lcd.print("& Brian Woo-Shem");
    delay(1234);
  }

  //Set PMS5003 serial read and pins
  pinMode(SET, OUTPUT);
  digitalWrite(SET, HIGH); //temporary workaround
  PMS_raw.begin(9600);
  
  if (rtc.begin()) { 
    DateTime time = rtc.now();
    //Need to reduce num characters by 2 or 3 to show seconds
    if (useSerial){
      Serial.print(" @" + time.timestamp(DateTime::TIMESTAMP_DATE) + " " + time.timestamp(DateTime::TIMESTAMP_TIME) + " ... ");
    }
  }
  else{ // If fail to begin the RTC (Real Time Clock)
    if (useSerial){
      Serial.println("Warning: Couldn't find RTC"); // Print warning via serial
    }
    //abort(); // Stop program
    useRTC = false;
  }

  //Set so that we can detect later which values are used 
  for(int i=0; i<numPer10min; i = i+1){
    pm2d5_10min[i] = -1;
  }
  
  if (useSerial){
    Serial.println("Ready!\n");
  }
}

void loop() {
  // Bug workaround for time, discovered by others. delay() causes sensor to crash.
  t2 = millis(); //milliseconds since startup of Arduino
  if (t2 - t1 > readInterval){
    if (PMS2struct(&PMS_raw)){
      // Compute PM2.5 AQI value using function
      pm2d5 = calc_pm2d5_AQI(pd.particleCT_2d5);
      
      // Compute average PM2.5 over 10 mins
      pm2d5_10min[r] = pm2d5;
      r = r + 1;
      if (r >= numPer10min){
        r = 0;
      }
      short numVals = 0;
      double sum_pm2d5 = 0.0;
      for(short i=0; i<numPer10min; i=i+1){
        if(pm2d5_10min[i] >= 0){
          numVals = numVals+1;
          sum_pm2d5 = sum_pm2d5 + pm2d5_10min[i];
        }
      }
      pm2d5_10min_avg = sum_pm2d5/numVals;

      //Get DHT reading
      if (useDHT){
        hum = dht.readHumidity();
        tempC= dht.readTemperature();
        tempF = C2F(tempC);
      }

      if (useSerial){
        // Display all reading results to the Serial

        //Time header
        if (useRTC){
          DateTime time = rtc.now();
          Serial.println("========== Reading @ time = " + time.timestamp(DateTime::TIMESTAMP_DATE) + "  " + time.timestamp(DateTime::TIMESTAMP_TIME) + " ==========");
        }
        else{ //no RTC, so show ms since start time
          Serial.println("========== Reading @ time = " + String(t1) + " ==========");
        }

        //PM results
        Serial.println("Particle Count <0.3um = " + String(pd.particleCT_0d3) + " [particles/0.1L]");
        Serial.println("Particle Count <0.5um = " + String(pd.particleCT_0d5) + " [particles/0.1L]");
        
        Serial.println("PM1.0_Standard = " + String(pd.pm1d0_standard));
        Serial.println("PM1.0_Env = " + String(pd.pm1d0_env));
        Serial.println("Particle Count <1.0um = " + String(pd.particleCT_1d0) + " [particles/0.1L]");
        
        Serial.println("PM2.5 = " + String(pm2d5));
        Serial.println("PM2.5_Standard = " + String(pd.pm2d5_standard));
        Serial.println("PM2.5_Env = " + String(pd.pm2d5_env));
        Serial.println("Particle Count <2.5um = " + String(pd.particleCT_2d5) + " [particles/0.1L]");
  
        Serial.println("Particle Count <5.0um = " + String(pd.particleCT_5d0) + " [particles/0.1L]");
  
        Serial.println("PM10_Standard = " + String(pd.pm10d0_standard));
        Serial.println("PM10_Env = " + String(pd.pm10d0_env));
        Serial.println("Particle Count <10um = " + String(pd.particleCT_10d0) + " [particles/0.1L]");

        // Temperature & Humidity results if DHT in use
        if (useDHT){
          Serial.println("Temperature = " + String(tempC) + " °C =   " + String(tempF) + " °F");
          Serial.println("Humidity = " + String(hum) + " %");
        }
      }

      //Set Color LED to color matching PM2.5 range
      if (useColorLED){
        colorAQI(pm2d5);
      }
      
      //Set time so it waits before getting the next reading
      t1 = t2;
    }
    else{ //Could not read PMS
      if (useSerial && debugMode){
        Serial.println("Error @ time = " + String(t1) + " : Could not get reading");
      }
    }
  }
  // If using LCD, this handles display changing.
  if(useDisplayI2C||useDisplayLCD){
    //If it has shown same screen for long enough, change the screen
    if ((t2-tdisp) > disptime){
      //Screen 0 is date and time:
      if (lcdScreen == 0){
        lcd.clear();
        lcd.setCursor(0,0);
        if (useRTC){ //Show actual date/time if using RTC
          DateTime time = rtc.now();
          //Show Date and Time
          lcd.print(time.timestamp(DateTime::TIMESTAMP_DATE));
          lcd.setCursor(0,1);
          lcd.print(time.timestamp(DateTime::TIMESTAMP_TIME));
        }
        else { //Otherwise show logical time
          lcd.setCursor(0,0);
          lcd.print("VioletAir PM Sensor");
          lcd.setCursor(0,1);
          lcd.print("t=" + String(t1));
        }
        lcdScreen = 1;
      } //Screen 1 is PM2.5
      else if (lcdScreen == 1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("PM2.5 Now= " + String(pm2d5));
        lcd.setCursor(0,1);
        lcd.print("10min = " + String(pm2d5_10min_avg));
        lcdScreen = 2;
      } //Screen 2: Particle counts 1/2
      else if (lcdScreen == 2){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("<0.3|<0.5|<1.0um");
        lcd.setCursor(0,1);
        lcd.print(String(pd.particleCT_0d3));
        lcd.setCursor(5,1);
        lcd.print(String(pd.particleCT_0d5));
        lcd.setCursor(10,1);
        lcd.print(String(pd.particleCT_1d0));
        lcdScreen = 3;
      } //Screen 3: Particle counts 2/2
      else if (lcdScreen == 3){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("<2.5|<5.0|<10 um");
        lcd.setCursor(0,1);
        lcd.print(String(pd.particleCT_2d5));
        lcd.setCursor(5,1);
        lcd.print(String(pd.particleCT_5d0));
        lcd.setCursor(10,1);
        lcd.print(String(pd.particleCT_10d0));
        lcdScreen = 4;
      } //Temperature and humidity
      else if (lcdScreen == 4){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(String(tempC) + "C = " + String(tempF) + "F");
        lcd.setCursor(0,1);
        lcd.print("RH = " + String(hum) + " %");
        lcdScreen = 0;
      }
      tdisp = t2;
    }
  }
}


// Code for calculating the sensor readings
// Largely adapted from the Adafruit_PM25AQI.cpp
// https://github.com/adafruit/Adafruit_PM25AQI/blob/master/Adafruit_PM25AQI.cpp
boolean PMS2struct(Stream *pms_datastream) {
  // Initialize data containers in integer byte sizes
  uint8_t buffer[32];
  uint16_t sum = 0;
  
  // Structure is something like '0x42' followed by 32 bytes that are used to extract the various readings.
  
  // Check if available
  if (! pms_datastream->available()) {
    return false;
  }

  // Read bytes one by one to find the '0x42' key
  if (pms_datastream->peek() != 0x42) {
    pms_datastream->read();
    return false;
  }

  // Read all 32 bytes that follow the 0x42
  if (pms_datastream->available() < 32) {
    return false;
  }

  pms_datastream->readBytes(buffer, 32);

  // Check that start byte is correct!
  if (buffer[0] != 0x42) {
    return false;
  }

  // get checksum ready
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }

  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  // put result into the PMS_DATA_RETURN structure (named 'pd')
  memcpy((void *)&pd, (void *)buffer_u16, 30);

  //Check if data is correct. If error, indicate false
  if (sum != pd.checksum){
    return false;
  }

  // If data is correct
  return true;
}


// Adapted from https://docs.google.com/document/d/15ijz94dXJ-YAZLi9iZ_RaBwrZ4KtYeCy08goGBwnbCU/edit
// and https://blog.brendon.net/diy-purpleair-sensor/
float calc_pm2d5_AQI(float pm2d5_raw){
  /*
  CATEGORY                        AQI Score Range       Unused?         PM2.5 Range
    Good                              0 - 50          0.0 - 15.0         0.0 – 12.0
    Moderate                         51 - 100         >15.0 - 40        12.1 – 35.4
    Unhealthy for Sensitive Groups  101 – 150         >40 – 65          35.5 – 55.4
    Unhealthy                       151 – 200         > 65 – 150        55.5 – 150.4
    Very Unhealthy                  201 – 300         > 150 – 250      150.5 – 250.4
    Hazardous                       301 – 400         > 250 – 350      250.5 – 350.4
    Hazardous                       401 – 500         > 350 – 500      350.5 – 500
   */

  //Reversed order to start with lowest first since in most cases, the PM level is on the low end
  if (pm2d5_raw < 0){
    return -99.99;
  }
  else if (pm2d5_raw <= 12.1){
    return AQI_EQN(pm2d5_raw, 50.0, 0.0, 12.0, 0.0);
  }
  else if (pm2d5_raw <= 35.5){
    return AQI_EQN(pm2d5_raw, 100.0, 51.0, 35.4, 12.1);
  }
  else if (pm2d5_raw <= 55.5){
    return AQI_EQN(pm2d5_raw, 150.0, 101.0, 55.4, 35.5);
  }
  else if (pm2d5_raw <= 150.5){
    return AQI_EQN(pm2d5_raw, 200.0, 151.0, 150.4, 55.5);
  }
  else if (pm2d5_raw <= 250.5){
    return AQI_EQN(pm2d5_raw, 300.0, 201.0, 250.4, 150.5);
  }
  else if (pm2d5_raw <= 350.5){
    return AQI_EQN(pm2d5_raw, 400.0, 301.0, 350.4, 250.5);
  }
  else if (pm2d5_raw > 350.5){
    return AQI_EQN(pm2d5_raw, 500.0, 401.0, 500.0, 350.5);
  }
}

// Demystified the calcAQI() method from "Using PurpleAir Data"
float AQI_EQN (float pm2d5_raw, double aqi_range_max, double aqi_range_min, double pm_range_max, double pm_range_min){
  float aqi_range = aqi_range_max - aqi_range_min;
  float pm_range = pm_range_max - pm_range_min;
  float pm_by_min_multiplier = pm2d5_raw - pm_range_min;
  return (aqi_range / pm_range) * pm_by_min_multiplier + aqi_range_min;
}

//Function to display RGB color values
void RGB_color(short red_light_value, short green_light_value, short blue_light_value){
        analogWrite(redPin, red_light_value);
        analogWrite(greenPin, green_light_value);
        analogWrite(bluePin, blue_light_value);
}

//Function for determining RGB color depending on potentiometer
// Max is 255 for any
void colorAQI (short pm2d5){
    if (pm2d5 <= 50){ //green
      // intentionally left less than max brightness because it was blinding
      RGB_color(0,100,0); 
    }
    else if (pm2d5 <= 100){ //yellow
      RGB_color(255,60,0);
    }
    else if(pm2d5 <= 150){ //orange
      RGB_color(255,10,0);
    }
    else if((pm2d5 <= 200)){ //red
      RGB_color(255,0,0); 
    }
    else if((pm2d5 <= 250)){ //violet
      RGB_color(255,0,50); 
    }
    else { //Blueviolet
      RGB_color(255,0,180);
    }
}

//Convert Celsius to Fahrenheit
float C2F(float temp_in_degC){
  return temp_in_degC * 1.8 + 32.0;
}
