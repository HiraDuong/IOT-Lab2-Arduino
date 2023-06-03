// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// DS1307
#include "RTClib.h"

RTC_DS1307 rtc;
// DS1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Start



void setup() {
  Serial.begin(115200);

  // Khởi tạo giao tiếp I2C
  Wire.begin();

  // Khởi tạo màn hình SSD1306
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Màn hình SSD1306 không kết nối được."));
    while (true);
  }

  // Khởi tạo DS1307
  if (!rtc.begin()) {
    Serial.println(F("DS1307 không kết nối được."));
    while (true);
  }

  // Xóa màn hình
  display.clearDisplay();
}

void loop() {
  // Đọc ngày, tháng, năm từ DS1307
  DateTime now = rtc.now();
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
  if (day > 9){
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
    display.print(sec);
  }
  else{
    display.print("0");
    display.print(sec);
  }
  display.display();



  // Đợi 1 giây trước khi cập nhật lại ngày tháng năm
  delay(1000);
}
