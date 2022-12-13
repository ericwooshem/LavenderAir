/* LavenderAir NanoPMM-SD-LED Sensor Code
 * (Open-Source Particulate Matter and PM2.5 Air Quality Index Monitor)
 * Copyright (C) Brian & Eric Woo-Shem
 * 
 * Version 1.00
 * Updated: 2022-12-12
 * 
 * === Instructions ===
 * See the GitHub page:
 * 
 * Look for the model/version "NanoPMM-SD-LED"
 * 
 * === LICENSE ===
 *  This program is free software: you can redistribute it and/or modify it under the terms of the 
 *  GNU General Public License version 3 as published by the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 *  even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 *  GNU General Public License at https://www.gnu.org/licenses/gpl-3.0.en.html for more details.
 * 
 * === Required Libraries ===
 * - DHTlib
 * - RTClib
 * These can be installed under Tools -> Manage Libraries
 * 
 * === References ===
 * Partially based on the Adafruit_PM25AQI library for Arduino, (C) 2012, Adafruit Industries under BSD License
 * https://github.com/adafruit/Adafruit_PM25AQI
 * 
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
 * ElectroPeak, 2019 "SD Card Module with Arduino" Arduino Project Hub
 * https://create.arduino.cc/projecthub/electropeak/sd-card-module-with-arduino-how-to-read-write-data-37f390
 */

#include <Wire.h>
#include <DHT.h>
#include "RTClib.h"
#include <SD.h>

// Settings
const bool useDHT = true;
const bool useColorLED = true;
bool useRTC = true;
const bool useSD = true;


// Pin Definitions

// PM Sensor
const byte SET = 2;
const byte TX = 8;
//DHT22 - MUST COMMENT OUT this portion if not using a DHT!
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22  //Change to DHT11 if using other type
DHT dht(4, DHTTYPE); //// Initialize DHT sensor using library

//RGB LED (needs PWM pins)
const byte redPin = 6;
const byte greenPin = 5;
const byte bluePin = 3;

RTC_DS1307 rtc; // Define RTC (Real Time Clock)

File dataFile;
#define CS_PIN 10

// How often to take readings
const short readInterval = 8192;

// Time tracking variables
unsigned long t1 = 0;
unsigned long t2 = 0;

float pm2d5 = 0.0;

float tempC = 0;
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

  //Set color LED pins if used
  if (useColorLED){
    //declaring the color pins for the RGB LED
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
  }

  if(useDHT){
    dht.begin();
  }

  //Set PMS5003 serial read and pins
  pinMode(SET, OUTPUT);
  digitalWrite(SET, HIGH); //temporary workaround
  PMS_raw.begin(9600);
  
  if (rtc.begin()) { 
    //DateTime time = rtc.now();
  }
  else{ // If fail to begin the RTC (Real Time Clock)
    //abort(); // Stop program
    useRTC = false;
  }

  if(useSD){
    if (SD.begin(CS_PIN)){ // Start SD card reader/writer
      dataFile = SD.open("PMM_Data.txt", FILE_WRITE); // Create or open a the file with name specified above as filename
      //Create CSV file header
      // Abbreviating because these strings have a large effect on the Global Variables dynamic memory
      // PC = Particle Count [Particles/0.1L]
      // Sta = Standard
      // Env = Environmental
      dataFile.print("Date,Time,PC <0.3um [/0.1L],PC <0.5um [/0.1L],PM1.0_Sta,PM1.0_Env,PC <1.0um [/0.1L],PM2.5,PM2.5_Sta,PM2.5_Env,PC <2.5um [/0.1L],PC <5.0um [/0.1L],PM10_Sta,PM10_Env,PC <10.0um [/0.1L]"); 
      if (useDHT){
        dataFile.print(",Temp [°C],RH [%]");
      }
      dataFile.print("\n");
      dataFile.close(); //Need to close file to save
    }
    else{
      if (useColorLED){
        RGB_color(255,0,0);
      }
    }
  }
}

void loop() {
  // Bug workaround for time, discovered by others. delay() causes sensor to crash.
  t2 = millis(); //milliseconds since startup of Arduino
  if (t2 - t1 > readInterval){
    if (PMS2struct(&PMS_raw)){
      // Compute PM2.5 value using function
      pm2d5 = calc_pm2d5_AQI(pd.particleCT_2d5);

      if (useDHT){
        hum = dht.readHumidity();
        tempC= dht.readTemperature();
      }
      
      if (useSD){
        dataFile = SD.open("PMM_Data.txt", FILE_WRITE); // Create or open a the file with name specified above as filename
        if (useRTC){
          DateTime time = rtc.now();
          dataFile.print(time.timestamp(DateTime::TIMESTAMP_DATE) + "," + time.timestamp(DateTime::TIMESTAMP_TIME) + ",");
        }
        else{
          dataFile.print(" ," + String(t2) + ",");
        }
        dataFile.print(String(pd.particleCT_0d3)+ String(pd.particleCT_0d5)+ "," +String(pd.pm1d0_standard)+ "," +String(pd.pm1d0_env)+ "," +String(pd.particleCT_1d0)+ "," +String(pm2d5)+ "," +String(pd.pm2d5_standard)+ "," +String(pd.pm2d5_env)+ "," +String(pd.particleCT_2d5)+ "," +String(pd.particleCT_5d0)+ "," +String(pd.pm10d0_standard)+ "," +String(pd.pm10d0_env)+ "," +String(pd.particleCT_10d0));
        if (useDHT){
          dataFile.print("," + String(tempC) + "," + String(hum));
        }
        dataFile.print("\n");
        dataFile.close(); //Close each time to save output
      }

      //Set Color LED
      if (useColorLED){
        colorAQI(pm2d5);
      }
      
      //Set time so it waits before getting the next reading
      t1 = t2;
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
float AQI_EQN (float pm2d5_raw, float aqi_range_max, float aqi_range_min, float pm_range_max, float pm_range_min){
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
