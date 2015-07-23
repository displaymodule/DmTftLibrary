/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include <SPI.h>
#include <DmTftSt7789.h>
#include <DmTouch.h>
#include <utility/DmTouchCalibration.h>

#define TFT_CS  17
#define T_CS    8
#define T_IRQ   9

DmTftSt7789 tft = DmTftSt7789();
DmTouch dmTouch = DmTouch(DmTouch::DM_TFT24_312);
DmTouchCalibration calibration = DmTouchCalibration(&tft, &dmTouch);

uint16_t textRow = 20;

void setup() {
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  pinMode(T_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(T_CS, HIGH);
  
  Serial.begin(9600);
  
  Serial.print(F("Free RAM is: "));
  Serial.println(freeRam());
  
  Serial.println(F("Init TFT drivers"));
  tft.init();
  Serial.println(F("Init Touch drivers"));
  dmTouch.init();
  dmTouch.setCalibrationMatrix(calibration.getDefaultCalibrationData((int)DmTouch::DM_TFT24_312));  
  
  tft.drawString(35, 10, "www.displaymodule.com");

  // Start Touch screen tests
  tft.drawString(15,280,"Tap screen to continue!");
  WaitForSquareToBePressed(0,0,240,320);
  tft.clearScreen();
 
  tft.fillRectangle(40, 100, 80, 140, BLUE);
  WaitForSquareToBePressed(40,100,40,40);
  tft.fillRectangle(40, 100, 80, 140, BLACK);
  
  tft.fillRectangle(160, 100, 200, 140, RED);
  WaitForSquareToBePressed(160,100,40,40);
  tft.fillRectangle(160, 100, 200, 140, BLACK);
  
  tft.fillRectangle(40, 220, 80, 260, GREEN);
  WaitForSquareToBePressed(40,220,40,40);
  tft.fillRectangle(40, 220, 80, 260, BLACK);
  
  tft.fillRectangle(160, 220, 200, 260, YELLOW);
  WaitForSquareToBePressed(160,220,40,40);
  tft.fillRectangle(40, 100, 80, 140, BLUE);
  tft.fillRectangle(40, 220, 80, 260, GREEN);
  tft.fillRectangle(160, 100, 200, 140, RED);
  
  tft.drawString(26,160,"All features tested!");
  while(1){}
}


void loop() { }



void WaitForSquareToBePressed(int x, int y, int width, int length) {
  uint8_t squarePressed = 0;
  bool touched;
  uint16_t posX, posY;
    
  while (!squarePressed) {
    if (dmTouch.isTouched()) {
      dmTouch.readTouchData(posX, posY, touched);
      if (posX >= x && posX <= x+width && posY >= y && posY <= y+length) {
        squarePressed = 1;
      }
    }
  }
}


void PrintTestName(char* text) {
  textRow += 20;
  tft.drawString(5, textRow, text);
  Serial.print(text);
}

void PrintTestResult(char* text) {
  tft.drawString(200, textRow, text);
  Serial.print(": ");
  Serial.println(text);
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
