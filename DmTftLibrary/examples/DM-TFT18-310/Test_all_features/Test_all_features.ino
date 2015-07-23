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
#include <SPIFlash.h> // SPIFlash library from lowpowerlab (https://github.com/LowPowerLab/SPIFlash)
#include <SD.h>
#include <DmTftIli9163c.h>
#include <DmTouch.h>
#include <utility/DmTouchCalibration.h>

DmTftIli9163c tft = DmTftIli9163c();

DmTouch dmTouch = DmTouch(DmTouch::DM_TFT18_310, DmTouch::Auto, false);
DmTouchCalibration calibration = DmTouchCalibration(&tft, &dmTouch);

uint16_t textRow = 20;

void setup() {
  tft.init();
  dmTouch.init();
  dmTouch.setCalibrationMatrix(calibration.getDefaultCalibrationData((int)DmTouch::DM_TFT18_310));

  tft.drawString(2, 10, "www.displaymodule.com");

  tft.drawString(2,120,"Tap screen to continue!");
  WaitForSquareToBePressed(0,0,128,160);
  tft.clearScreen();
  
  tft.drawString(2,50,"Press the colored squares");
  
  pressSquare(10, 20, 40, 50, BLUE);
  pressSquare(88, 20, 118, 50, RED);
  pressSquare(10, 110, 40, 140, GREEN);
  pressSquare(88, 110, 118, 140, YELLOW);
   
  tft.drawString(2,100,"All features tested!");
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

