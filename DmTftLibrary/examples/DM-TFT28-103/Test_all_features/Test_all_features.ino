/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
// Copy sdlogo.bmp to the SD-card
// This does not test T_IRQ, instead it test the SD-card

#include <SPI.h>
#include <SPIFlash.h> // SPIFlash library from lowpowerlab (https://github.com/LowPowerLab/SPIFlash)
#include <SD.h>
#include <DmTftIli9325.h>
#include <DmTouch.h>
#include <DmDrawBmpFromSdCard.h>
#include <utility/DmTouchCalibration.h>

#define SD_CS   10

DmTftIli9325 tft = DmTftIli9325();
DmDrawBmpFromSdCard sdCardImage = DmDrawBmpFromSdCard();
DmTouch dmTouch = DmTouch(DmTouch::DM_TFT28_103, DmTouch::Auto, false);
DmTouchCalibration calibration = DmTouchCalibration(&tft, &dmTouch);

uint16_t textRow = 20;

void setup() {
  tft.init();
  dmTouch.init();
  dmTouch.setCalibrationMatrix(calibration.getDefaultCalibrationData((int)DmTouch::DM_TFT28_103));

  tft.drawString(35, 10, "www.displaymodule.com");

  PrintTestName("Check bmp on SD-card");
  if (!SD.begin(SD_CS)) {
    PrintTestResult("Fail");
    while(1);
  }
  if (!sdCardImage.printHeaderInfo("sdlogo.bmp")) {
    PrintTestResult("Fail");
    while(1);
  }
  PrintTestResult("OK");
   
  PrintTestName("Draw bmp SD-card");
  sdCardImage.drawImage("sdlogo.bmp", tft, 0, 200);
  PrintTestResult("OK");
  
  tft.drawString(15,280,"Tap screen to continue!");
  WaitForSquareToBePressed(0,0,240,320);
  tft.clearScreen();
  
  tft.drawString(36,36,"www.displaymodule.com");
  tft.drawString(24,56,"Press the colored squares");
  
  pressSquare(40, 100, 80, 140, BLUE);
  pressSquare(160, 100, 200, 140, RED);
  pressSquare(40, 220, 80, 260, GREEN);
  pressSquare(160, 220, 200, 260, YELLOW);
   
  tft.drawString(26,200,"All features tested!");
  while(1){}
}

void loop() { }

void pressSquare(int x1, int y1, int x2, int y2, uint16_t color) {
  tft.fillRectangle(x1, y1, x2, y2, color);
  WaitForSquareToBePressed(x1 ,y1 ,x2 ,y2);
  tft.fillRectangle(x1, y1, x2, y2, BLACK);
}

void WaitForSquareToBePressed(int x1, int y1, int x2, int y2) {
  int width = x2 - x1;
  int length = y2 - y1;
  uint8_t squarePressed = 0;
  bool touched;
  uint16_t posX, posY;
    
  while (!squarePressed) {
    if (dmTouch.isTouched()) {
      dmTouch.readTouchData(posX, posY, touched);
      if (posX >= x1 && posX <= x1+width && posY >= y1 && posY <= y1+length) {
        squarePressed = 1;
      }
    }
  }
}

void PrintTestName(char* text) {
  textRow += 20;
  tft.drawString(5, textRow, text);
}

void PrintTestResult(char* text) {
  tft.drawString(200, textRow, text);
}
