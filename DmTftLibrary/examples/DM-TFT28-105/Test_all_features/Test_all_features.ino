/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

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
#include <DmTftIli9341.h>
#include <DmTouch.h>
#include <DmDrawBmpFromSpiFlash.h>
#include <DmDrawBmpFromSdCard.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4
#define T_IRQ   3

DmTftIli9341 tft = DmTftIli9341(10, 9);
DmDrawBmpFromSpiFlash spiFlashImage = DmDrawBmpFromSpiFlash();
DmDrawBmpFromSdCard sdCardImage = DmDrawBmpFromSdCard();
DmTouch dmTouch = DmTouch(DmTouch::DM_TFT28_105, T_CS, T_IRQ);

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

  //Debug info
  Serial.print(F("Free RAM is: "));
  Serial.println(freeRam());
  
  Serial.println(F("Init TFT drivers"));
  tft.init();
  Serial.println(F("Init Touch drivers"));
  dmTouch.init();
  
  
  tft.drawString(35, 10, "www.displaymodule.com");
  

  PrintTestName("Check bmp on SD-card");
  if (!SD.begin(SD_CS) || !sdCardImage.printHeaderInfo("flashlog.bmp") || !sdCardImage.printHeaderInfo("sdlogo.bmp")) {
    PrintTestResult("Fail");
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
  sdCardImage.drawImage("sdlogo.bmp", tft, 0, 230);
  PrintTestResult("OK");
  
  // Start Touch screen tests
  tft.drawString(15,280,"Tap screen to continue!");
  WaitForSquareToBePressed(0,0,240,320);
  tft.clearScreen();
  
  spiFlashImage.drawImage(0, spiFlash, tft, 0, 0);
  tft.drawString(36,36,"www.displaymodule.com");
  tft.drawString(24,56,"Press the colored squares");
  
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
  
  tft.drawString(26,290,"All features tested!");
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
