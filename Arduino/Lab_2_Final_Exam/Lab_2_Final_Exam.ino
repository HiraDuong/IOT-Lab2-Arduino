// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include<string>
#include<iostream>
using namespace std;
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS D6

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */

// DS1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// DS1307
#include "RTClib.h"
RTC_DS1307 rtc;
#include <EEPROM.h>
#include <ctime>
// Start
int address = 0;
void unixtodt(unsigned int unix){
    std::time_t timeValue = static_cast<std::time_t>(unix);
    std::tm* timeinfo = std::localtime(&timeValue);

    // Lấy các thành phần ngày, tháng, năm, giờ, phút, giây từ struct tm
    int year = timeinfo->tm_year + 1900;
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;
    int second = timeinfo->tm_sec;
    //Serial.println(string(day)+"/"+string(month)+"/"+string(year));
    //Serial.println(string(hour)+":"+string(minute)+":"+string(second));
    Serial.println(second);

}
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  EEPROM.begin(512);
  // Start up the library
  sensors.begin();
    // Khởi tạo màn hình SSD1306
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Màn hình SSD1306 không kết nối được."));
    while (true);
  }
  // Start ds1037 
  Wire.begin();
  if (!rtc.begin()) {
      Serial.println(F("DS1307 không kết nối được."));
      while (true);
    }
    
  display.clearDisplay();

}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.println("Requesting temperatures and date time");
  sensors.requestTemperatures(); // Send the command to get temperatures
  DateTime now = rtc.now();
  //Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);
  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  { //write data to EEPROM  
    //Serial.println(now.unixtime());
    if (address >= 512){
      address = 0;
    }
    
    EEPROM.put(address,tempC);
    EEPROM.put(address+sizeof(float),now.unixtime());
    EEPROM.commit();
    // read data
    float temp ;
    EEPROM.get(address,temp);
    unsigned int unixsecs;
    EEPROM.get(address+sizeof(float),unixsecs);
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(unixsecs/86400L);
    unixtodt(unixsecs);
    Serial.println(temp);
    
    address += sizeof(unsigned int);
    
    display.clearDisplay();
    int day = now.day();
    int month = now.month();
    int year = now.year();
    int hour = now.hour();
    int min = now.minute();
    int sec = now.second();
   
    // Hiển thị ngày tháng năm lên màn hình
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
  //print time
    {  if (day > 9){
        display.print(day);
      }
      else{
        display.print("0");
        display.print(day);
      }
      
      display.print(":");
      if (month > 9){
        display.print(month);
      }
      else{
        display.print("0");
        display.print(month);
      }
      display.print(":");
      display.println(year);
      if (hour > 9){
        display.print(hour);
      }
      else{
        display.print("0");
        display.print(hour);
      }
      display.print(":");
      if (min > 9){
        display.print(min);
      }
      else{
        display.print("0");
        display.print(min);
      }
      display.print(":");
      if (sec > 9){
        display.println(sec);
      }
      else{
        display.print("0");
        display.println(sec);
      }
      }
      display.print(tempC);
      display.setTextSize(1);
      display.print("o");
      display.setTextSize(2);
      display.print("C");
      display.display();
      //delay(3000);
    } 
  else
  {
    Serial.println("Error: Could not read temperature data");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Error: Could not read temperature data");
    display.display();
    delay(3000);
  }
  delay(1000);
}
