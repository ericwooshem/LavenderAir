# LavenderAir Open-Source DIY Particulate Matter PM2.5 Monitor

The DIY Open-Source Particulate Matter and PM2.5 Air Quality Index Monitor. Inspired by PurpleAir*

CC BY-NC-SA [Eric Woo-Shem](https://github.com/ericwooshem) & [Brian Woo-Shem](https://www.brianwooshem.com)

![LavenderAirSensorNanoPMM](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/20221213_171055_LavenderAir_NanoPMM_1.png)

## Features

- Displays the current Particulate Matter levels so you can quickly check if the current conditions are safe. 
- Optional SD or Bluetooth logging.
- Same accuracy as commercially available devices.
- Costs $40 to $140 (depending on model) - less than half the cost of commercial sensors.
- No soldering required! Just connect wires.

## Introduction: Why measure Particulate Matter?

Particulate Matter (PM) can cause serious health issues including asthma, heart attacks, strokes, and even death [1][2][3]. Smog, wildfires, and industrial pollution can increase PM to unhealthy or dangerous levels, so it is important for the general public to know when this occurs and to move to indoor areas with healthier air [1][3]. The Air Quality Index (AQI) is a numerical conversion used to represent the relative danger of PM and other pollutants [4]. The EPA provides high-quality AQI measurements and AQI forecasts [1], however these are limited to data collected at specific locations and the readings are typically updated only hourly. Low-cost sensors have been used to monitor PM levels at near-instantaneous rates, and at local locations, including in or outside your own home, school, or workplace; and provide fairly good accuracy [5][15]. For example, [PurpleAir*](https://www2.purpleair.com/) supplies air quality monitors in the $200-300 price range [16], and connects them to a publicly available [map](map.purpleair.com). 

The LavenderAir project is developing an open-source PM/AQI monitor that is __low cost__, __easily customizable__, and __privacy-respecting__. LavenderAir monitors offer the __same accuracy__ to the PurpleAir* monitors, as the identical model of sensors is used. DIY assembly, "modular" features, and removal of the internet-connected components reduce the cost to as low as $40. Your data is easily viewable through an LED indicator, LCD display, and/or Bluetooth connection to your computer, can be stored on an SD card, and is __never__ uploaded to the internet.



## Monitor Models & Versions

__NanoPMM-Disp__: Single Sensor + View current readings on LCD Display + Serial Output to ArduinoIDE (Working)

__NanoPMM-SD-LED__: Single Sensor + SD card logging + LED color indicator (Working)

__NanoPMM-BT-LED__: Single Sensor + Bluetooth logging to a computer + LED color indicator. (Work in progress)

__MegaPMM__: The most advanced model. Includes Dual Sensor for increased accuracy + LCD Display to show current readings + SD card logging + LCD color indicator + Bluetooth logging to a computer + Serial output to ArduinoIDE. (Work in progress)


# Instructions

### Parts

This is a *frugal* AQI sensor so most of the parts can be interchanged with similar ones. A detailed Bill of Materials with the exact components used in this version of the sensor is provided as "[LavenderAir_Bill_of_Materials.xlsx](https://github.com/ericwooshem/LavenderAir/blob/main/VioletAir_AQI_Sensor_Bill_of_Materials.xlsx)". Different sheets show suggestions for products to purchase for the lowest cost for the different models above. The quantities and exact cost autocompute if you plug in the number of sensors you want to build. 

**Note:** Product suggestions in Bill of Materials are for convenience only. This project does not receive any incentive, financial or otherwise, for suggesting any particular product.

#### Cost

One sensor can be built for roughly $100, and 10 sensors for about $87 each if all parts are purchased new (As of August 2021).

#### List of Parts

This is a general list of components; see the [Bill of Materials](https://github.com/ericwooshem/LavenderAir/blob/main/VioletAir_AQI_Sensor_Bill_of_Materials.xlsx) for all parts & product suggestions by each PMM model.

| Part Description | QTY per Sensor | Which models?|
|---|---|---|
| Arduino Nano| 1|All NanoPMM|
|Arduino Mega|1|MegaPMM|
|Plantower PMS5003 Particle Concentration Sensor| 1| All; need 2 for the MegaPMM|
|Plantower PMS5003 Particle Concentration Sensor Wire (often included with sensor)|1| All; need 2 for the MegaPMM|
|Breadboard: Mini breadboards recommended, but any size can work|1-2|All|
|Micro SD Card Adapter Module (Or SD Card Adapter Module)|1|NanoPMM-SD & MegaPMM|
|Micro SD (or SD) Card. Small is fine - 1 GB is overkill|1|NanoPMM-SD & MegaPMM|
|DS3231 Real Time Clock Module|1|NanoPMM-SD & MegaPMM|
|CR2032 Battery|1|NanoPMM-SD & MegaPMM|
|DHT22 Temperature & Humidity Sensor (DHT11 also works but inferior)|1|All|
|RGB Tri-Color 4-pin LED|1|All; optional for NanoPMM-Disp|
|Jumper wires|10-20|All|
|USB Power Bank Battery 5V or USB Power supply|1|Optional|
|16x2 I2C LCD Display: |1|NanoPMM-Disp & MegaPMM|
|HC-05 Bluetooth Module|1|NanoPMM-BT & MegaPMM|
|Small Box: Cardboard, 3D printed, etc. |1|All|





## Assembly

![Connection Diagram](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/NanoPMM_Circuit_Schematic.png)

Complete connection diagram.

Different models may require some or all of these steps. Model is noted in (parenthesis) at the start of each step.

1. (NanoPMM-SD-LED and MegaPMM) Insert the CR2032 battery into the RTC, and your memory card into your card reader.

	![Micro SD](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I1.png) ![RTC](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I2.png)


4. (NanoPMM) Place and push in the Arduino Nano into one of the breadboards, as shown.

	![Arduino Nano Breadboard](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I5.png)

	- (For Uno or Mega) The connectors are built in, so this step is not necessary. However, you may need to run a wire from a particular pin to a breadboard row if more than one connection is required.  


7. (All) Connect the Ground on the Arduino to an unused row on the breadboard. Connect the 5V on the Arduino to a different unused row. Below is an example for mini-breadboards.

	![Arduino-Breadboard](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I8.png)

8. (NanoPMM-SD-LED and MegaPMM) Connect the Real Time Clock module using 4 male-to-female wires:

|Sensor Wire|Breadboard Row|
|---|---|
|VCC | 5V row created in step 7|
|GND | the ground row created in step 7|
|SDA | Arduino pin A4|
|SCL | Arduino pin A5|

   ![RTC-Wires](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I9.png)

9. (NanoPMM-SD-LED and MegaPMM) Connect the SD Card Reader using 6 male-to-female wires:

|Sensor Wire|Breadboard Row|
|---|---|
|GND |ground row created in step 7|
|VCC | 5V row created in step 7|
|MISO | Arduino pin D12|
|MOSI | Arduino pin D11|
|SCK | Arduino pin D13|
|CS | Arduino pin D10|

![SD Wires](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I10.png)

10. (All) Connect the PMS5003 sensor wires. These are tricky because they are not always labeled.

|Wire Color | Position (left to right) |Label (if using Adafruit Adapter)  |Arduino Pin|
|---|---|---|---|
|Purple|1|VCC|5V row|
|Orange|2|GND|Ground row|
|White|3| SET|D2|
|Green|5|TX|D8|

![PMS5003 Wires](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/PMS5003_Wires_Colors.png)

11. (NanoPMM-Disp & MegaPMM) Connect the LCD Wires.

|LCD Pin Label|Arduino Pin|
|---|---|
|GND|Ground row|
|VCC|5V row|
|SDA|A4|
|SCL|A5|


11. (NanoPMM-BT-LED & MegaPMM) Connect the Bluetooth module 
|HC-05 Pin Label|Arduino Pin|
|---|---|
|+5V|5V row|
|GND|Ground row|
|RX|RX0|
|TX|TX1|

Congratulations! The sensor is fully assembled. Now connect the Arduino to your computer and upload the program to use it.

## Code Modifications

In each code, there are a series of variables for each feature (Serial, DHT, LCD Display, MulticolorLED, RTC, and Debug mode). Change booleans to `false` for features you are not using, and `true` for features you want to use.

    // Settings: Change to false if not using that component
    const bool useSerial = true;
    const bool useDHT = true;
    const bool useDisplayLCD = false;
    const bool useDisplayI2C = true;
    //const bool useSeparateLED = false; //not implemented yet
    const bool useColorLED = true;
    const bool debugMode = false;
    bool useRTC = true;


## Uploading the Program

12. Connect the Arduino USB cable between the Arduino and your computer.

13. (If not already installed) Download [ArduinoIDE](https://www.arduino.cc/en/software/).


15. Go to Tools>Board and select your Arduino type.   

	![Select board](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I15.png)

16. (Arduino Nano Generic Only) Go to Tools>Processor and select "ATmega328P (Old Bootloader)".   

	![Arduino Nano Processor](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I16.png)

17. Go to Tools>Port, and select the one including the number “x”, equal to the port you plugged the Arduino into. If you are unsure which port, start with the first option and click “Get Board Info". If you get an error, it is the wrong one. Try the next port and repeat until one works. You can also try uploading the code on each port in step 20.

18. Go to Tools>Programmer, and select “AVR ISP”.   

	![AVR ISP](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I18.png)

19. Install the necessary packages:
     1. Go to Tools>Manage Libraries. This may take a minute to load.   

           ![Open Libraries Manager](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I23.png)
	   
     2. Search & install DHTLib, RTClib


20. (If not already set) Follow instructions from [Arduino Get Started](https://arduinogetstarted.com/tutorials/arduino-rtc) to automatically set the RTC.

22. Upload the CO₂ sensor code: Download the appropriate Firmware code and open it in Arduino IDE. 

21. Upload the program to the Arduino by clicking on the right arrow button next to the check mark button. Warning: Do not touch until it says, "Done uploading."   
	![Arduino Upload Code](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/I22.png)

23. Plug the Arduino into a power supply. Depending on your model choice, the sensor system will display the current PM2.5 on the LED, LCD display, and/or save them to a file on the SD card.

# Troubleshooting

[Arduino IDE / Code Upload Issues](https://support.arduino.cc/hc/en-us/articles/4403365313810-Errors-when-uploading-a-sketch)   
[Arduino Nano Select Processor](https://support.arduino.cc/hc/en-us/articles/4401874304274-Select-the-right-processor-for-Arduino-Nano)   
[Wiring Intro for Beginners](https://learn.adafruit.com/breadboards-for-beginners/introduction)    
[Alternative Method to Set Date and Time on RTC](https://create.arduino.cc/projecthub/tittiamo68/clock-set-date-time-0d46a4) Note: Use the LCD wiring from this project instead.  

### Arduino to computer connection not working / code won't upload

- Check [Arduino IDE / Code Upload Issues](https://support.arduino.cc/hc/en-us/articles/4403365313810-Errors-when-uploading-a-sketch)
- Try each port option
- Try each processor option
- Swap the USB cable with another - sometimes the cables break
- Try a [Loopback Test](https://support.arduino.cc/hc/en-us/articles/360020366520-How-to-do-a-loopback-test)

### Most components appear to work, but one does not

- Most likely this is a wiring issue. Check that the wires leading to that component are matched to the correct pins and firmly attached.
- Re-upload the sensor code.
- Try a different power supply. If the current one is too weak, some components may work but others will not have enough power to operate.


# Accessing and Using Collected PM Data

When using the sensor, you can simply read the values from the screen. However, to get a more complete picture of how safe each place is, it is necessary to analyze the data for the entire time you are there. You can upload the data to your computer from the (micro) SD card, put it into a spreadsheet program, and create graphs or analyze it.

1. Remove the (Micro) SD card from the adapter module by pressing firmly and releasing until it springs out, then pulling it out gently. 
2. Connect the (Micro) SD card to your computer, using adapters if necessary.
3. Copy the file on the card “PMM_Data.txt” to your computer. The file is structured as follows:   
	![PMM Data File](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/PMM_Data_CSV_Demo.png)   
4. Typically, we want to import the data to a spreadsheet software like Excel or Google Sheets. We also want the times and the CO2 readings separated for so we can plot the values and compare over time. See the instructions below for your specific software. 

## Data Columns

__Date__: The day, in form `YYYY-MM-DD`. From RTC.  
__Time__: The time of reading, in form `HH:MM:SS`. From RTC.  
__PC <0.3um [/0.1L]__: Particle Count or number of particles of size equal to or less than 0.3µm per 0.1 L of air  
__PC <0.5um [/0.1L]__: Particle Count or number of particles of size equal to or less than 0.5µm per 0.1 L of air  
__PM1.0_Sta__: PM1.0 calculation using the "Standard" method from [13]. We suspect this is calculated incorrectly, so use with caution.  
__PM1.0_Env__: PM1.0 calculation using the "Environmental" method [13]. We suspect this is calculated incorrectly.  
__PC <1.0um [/0.1L]__: Particle Count or number of particles of size equal to or less than 1.0µm per 0.1 L of air  
__PM2.5_Env__: PM2.5 calculation - done correctly, using method in [4][6].  
__PM2.5_Sta__: PM2.5 calculation using the "Standard" method [13]. We suspect this is calculated incorrectly.  
__PM2.5_Env__: PM2.5 calculation using the "Environmental" method [13]. We suspect this is calculated incorrectly.  
__PC <2.5um [/0.1L]__: Particle Count or number of particles of size equal to or less than 2.5µm per 0.1 L of air  
__PC <5.0um [/0.1L]__: Particle Count or number of particles of size equal to or less than 5.0µm per 0.1 L of air  
__PM10.0_Sta__: PM10.0 calculation using the "Standard" method from [13]. We suspect this is calculated incorrectly.  
__PM10.0_Env__: PM10.0 calculation using the "Environmental" method [13]. We suspect this is calculated incorrectly.  
__PC <10.0um [/0.1L]__: Particle Count or number of particles of size equal to or less than 10.0µm per 0.1 L of air  
__Temp [°C]__: Temperature, in degrees Celsius. From DHT.  
__RH [%]__: Relative Humidity, in percentage. From DHT.  


### Text File Modification: Google Sheets or Microsoft Excel (Advanced, almost always works)
1. Open the _.txt_ file in a text editor.  
2. _Optional: Use Find & Replace to separate the hours, minutes, and seconds into three separate columns for doing calculations like time elapsed from the start or to convert it to another date-time format, in addition Find ":" (colon), and replace with "," (comma)._   
4. Change the file extension to _.csv_ (on Windows [see instructions below](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor#windows-changing-file-extension-to-csv))
5. Open with your favorite spreadsheet software. Example results:
	![Example text file modification results in Excel](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/FileImporting/CO2_Excel_EditTextFileResult.png)

### Microsoft Excel Text Import Wizard (Shortcut, sometimes works)
1. Open Excel
2. Go to the "Data" tab and select "From Text"   
	![Excel Text Import 1](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/FileImporting/CO2_Excel_Text_Import_Setup.png)   
3. Follow the Text Import Wizard. Make sure to select "Delimited" on the first step. _Optional: include "Semicolon" on the list of delimiters on the second step._   
	![Excel Text Import Wizard](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/FileImporting/CO2_Excel_Text_Import_Wizard.png)    
4. Data will now be in two columns, one for time and one for the sensor readings.

### LibreOffice
1. Change the file extension to _.csv_ (on Windows [see instructions below](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor#windows-changing-file-extension-to-csv))
2. Open with LibreOffice. You may need to right click then select "Open with" and choose your spreadsheet software if the computer defaults to another application.
3. Use the defaults in the Text Import window, then click OK. The data will be imported.   
	![Data import demo](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/PMM_Import_Data.png)

### Windows - Changing file extension to CSV
1. If the file extension (_.txt, .csv_, etc.) is not visible in File Manager, go to "View" tab and check "File name extensions"    
	![File Name Extensions Windows](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/FileImporting/CO2_Data_Import_FileNameExtension.png)   
2. Rename the file to have _.csv_ at the end instead of _.txt_    
	![File Renaming Windows](https://github.com/ericwooshem/DIY-Frugal-Arduino-CO2-Sensor/blob/main/Graphics/FileImporting/CO2_Data_Import_FileName_Windows_Warning.png)   
3. If you get a warning, select "Yes."


## Comparison to PurpleAir*

A company called [PurpleAir*](https://www2.purpleair.com/) produces AQI Monitors that are ready to use out of the box. Here's how our DIY PMM device compares.

![Comparison table](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/PMM_Comparison_Table_v1.png)


# Project Status & Contributions

LavenderAir is a work in progress. If you create an improvement to this project or complete some of the planned features before we can, please contact us so we can add your contribution and/or feel free to fork this project. 

Currently, a single sensor with SD card logging, LCD display, and LED color indicator modes work independently, but SD and LCD cannot be used simultaneously because the amount of data processing and variables required exceeds the memory available for an Arduino Nano or Uno.

We found these errors when attempting to combine all of the features to a single Arduino Nano device:
![Low memory](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/Arduino_Low_Memory_Warning.png)
![No memory](https://github.com/ericwooshem/LavenderAir/blob/main/Graphics/Arduino_Out_Of_Memory_Error.png)

Development of the MegaPMM version is in-progress. Currently, there is no working prototype available, and specs are based on theoretical features we expect are possible.

## Creators
[Eric Woo-Shem](https://github.com/ericwooshem): Project conceptualization, original design, component selection & original BOM, programming: sensor firmware prototype, bluetooth, RTC.

[Brian Woo-Shem](https://www.brianwooshem.com): Programming: production firmware, SD logging, conversion to AQI readings, debugging; cost comparison analysis for new BOM, research, documentation, data analysis, sensor testing and validation, visualizations.

## License

All text, graphics, and non-code content is licensed [CC BY-NC-SA](https://creativecommons.org/licenses/by-nc-sa/4.0/)    
All code is licensed under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.en.html)

RTClib and Adafruit_PM25AQI are Copyright (c) Adafruit Industries and used under the open source MIT license.

Arduino IDE and related components are open source and licensed under the GNU GPL v2 and/or CC BY-SA Arduino.

*PurpleAir is a trademark of PurpleAir, Inc. 

This project “LavenderAir” is a noncommercial, open-source project that seeks to replicate many features of the PurpleAir sensors. LavenderAir is in no way affiliated nor endorsed by PurpleAir, Inc., and LavenderAir components do not connect to nor interact with the PurpleAir network. LavenderAir monitors are “functional clones” built using off-the-shelf parts and no knowledge of commercial monitors aside from basic specifications made public on vendor websites. The creators of LavenderAir have no intention of any copyright or patent infringement, and to our best knowledge this project abides by all U.S. and international copyright laws. 

## References

[1] United States Environmental Protection Agency, "Particulate Matter (PM) Basics," www.epa.gov/pm-pollution/particulate-matter-pm-basics  
[2] AirNow, "Extremely High Levels of PM2.5: Steps to Reduce Your Exposure," www.airnow.gov/aqi/aqi-basics/extremely-high-levels-of-pm25/  
[3] United States Environmental Protection Agency, "Air Quality Guide for Particle Pollution," https://www.airnow.gov/publications/air-quality-index/air-quality-guide-for-particle-pollution/  
[4] AirNow, "AQI Calculator," https://www.airnow.gov/aqi/aqi-calculator/  
[5] PurpleAir Map: map.purpleair.com  
[6] PurpleAir "Using PurpleAir data" Google Doc. https://docs.google.com/document/d/15ijz94dXJ-YAZLi9iZ_RaBwrZ4KtYeCy08goGBwnbCU/edit   
[7] Brendon Baumgartner, 2020 "DIY PurpleAir Sensor," BB's Blog https://blog.brendon.net/diy-purpleair-sensor/   
[8] Daniel Ross, 2021, "How To Make an Arduino Air Quality Sensor," VueVille https://www.vueville.com/arduino/arduino-air-quality-sensor/   
[9] Data types - W3Schools - for reducing memory requirement. https://www.w3schools.com/java/java_data_types.asp   
[10] Minuka Thesath Yapa, 2021, "DHT11 & DHT22 Sensors Temperature using Arduino" https://create.arduino.cc/projecthub/MinukaThesathYapa/dht11-dht22-sensors-temperature-using-arduino-b7a8d6   
[11] Arduino Get Started "Arduino - LCD I2C" https://arduinogetstarted.com/tutorials/arduino-lcd-i2c   
[12] ElectroPeak, 2019 "SD Card Module with Arduino" Arduino Project Hub. https://create.arduino.cc/projecthub/electropeak/sd-card-module-with-arduino-how-to-read-write-data-37f390   
[13] Adafruit_PM25AQI library for Arduino, (C) 2012, Adafruit Industries under BSD License. https://github.com/adafruit/Adafruit_PM25AQI  
[14] PurpleAir "Map Start-Up Guide" https://community.purpleair.com/t/map-start-up-guide/90   
[15] PurpleAir "How do PurpleAir sensors compare to regulatory particulate matter sensors?" https://community.purpleair.com/t/q-how-do-purpleair-sensors-compare-to-regulatory-particulate-matter-sensors/810   
[16] PurpleAir. https://www2.purpleair.com/

