/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
// Copy flashlog.bmp and sdlogo.bmp to the SD-card

#include <SPI.h>
#include <SPIFlash.h> // SPIFlash library from lowpowerlab (https://github.com/LowPowerLab/SPIFlash)
#include <SD.h>
#include <DmTftSsd2119.h>
#include <DmTftIli9341.h>
#include <DmTouch.h>
#include <DmDrawBmpFromSpiFlash.h>
#include <DmDrawBmpFromSdCard.h>
#include <utility/DmTouchCalibration.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4
#define T_IRQ   3

DmTftSsd2119 tft = DmTftSsd2119();
DmDrawBmpFromSpiFlash spiFlashImage = DmDrawBmpFromSpiFlash();
DmDrawBmpFromSdCard sdCardImage = DmDrawBmpFromSdCard();
DmTouch dmTouch = DmTouch(DmTouch::DM_TFT35_107);
DmTouchCalibration calibration = DmTouchCalibration(&tft, &dmTouch);

SPIFlash spiFlash(F_CS, 0xEF40);
uint16_t textRow = 20;

void setup() {
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);  
  pinMode(F_CS, OUTPUT);
  pinMode(T_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(F_CS, HIGH);
  digitalWrite(T_CS, HIGH);
  
  Serial.begin(9600);
  Serial.print(F("Free RAM is: "));
  Serial.println(freeRam());
  
  Serial.println(F("Init TFT drivers"));
  tft.init();
  Serial.println(F("Init Touch drivers"));
  dmTouch.init();
  dmTouch.setCalibrationMatrix(calibration.getDefaultCalibrationData((int)DmTouch::DM_TFT35_107));
  
  tft.drawString(35, 10, "www.displaymodule.com");

  PrintTestName("Check bmp on SD-card");
  if (!SD.begin(SD_CS)) {
    PrintTestResult("Fail");
    Serial.println("Failed to init SD-card");
    while(1);
  }
  if (!sdCardImage.printHeaderInfo("flashlog.bmp") || !sdCardImage.printHeaderInfo("sdlogo.bmp")) {
    PrintTestResult("Fail");
    Serial.println("Can't find images on SD-card or the images are corrupt");
    while(1);
  }
  PrintTestResult("OK");
  
  PrintTestName("Check Ext. Flash ");
  if (!spiFlash.initialize()) {
    PrintTestResult("Fail");
    while(1);
  }
  PrintTestResult("OK");
  
  PrintTestName("Erase Flash ");
  eraseFlash(spiFlash);
  PrintTestResult("OK");
  
  PrintTestName("Write bmp to ext. flash");
  if (!writeFileToFlash(0, spiFlash, "flashlog.bmp")) {
    PrintTestResult("Fail");
    while(1);
  }
  PrintTestResult("OK");
  
  PrintTestName("Draw bmp ext. Flash");
  spiFlashImage.drawImage(0, spiFlash, tft, 0, 200);
  PrintTestResult("OK");
 
  PrintTestName("Draw bmp SD-card");
  sdCardImage.drawImage("sdlogo.bmp", tft, 181, 200);
  PrintTestResult("OK");
  
  tft.drawString(15,280,"Tap screen to continue!");
  WaitForSquareToBePressed(0,0,240,320);
  tft.clearScreen();
  
  spiFlashImage.drawImage(0, spiFlash, tft, 0, 0);
  tft.drawString(36,36,"www.displaymodule.com");
  tft.drawString(24,56,"Press the colored squares");
  
  pressSquare(80, 80, 120, 120, BLUE);
  pressSquare(160, 80, 200, 120, RED);
  pressSquare(80, 150, 120, 190, GREEN);
  pressSquare(160, 150, 200, 190, YELLOW);
   
  tft.drawString(26,200,"All features tested!");
  while(1){}
}


void loop() { }


void eraseFlash(SPIFlash spiFlash) {
  spiFlash.chipErase();
  while(spiFlash.busy());
}

boolean writeFileToFlash(uint32_t startAddress, SPIFlash spiFlash, char* fileName) {
  uint32_t fileSize;
  uint32_t counter;
   
  File imageFile;
  if ((imageFile = SD.open(fileName)) == NULL) {
    Serial.println(F("Can't open image file"));
    return false;
  }
  
  fileSize = imageFile.size();
  counter = startAddress;
  while(counter<=fileSize) {
    spiFlash.writeByte(counter, imageFile.read());
    counter++;
  }
  imageFile.close();
  
  return true;
}

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
