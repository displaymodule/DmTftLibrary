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
#include <DmTftRa8875.h>
#include <DmDrawBmpFromSpiFlash.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define F_SEL   9
DmTftRa8875 tft = DmTftRa8875(TFT_CS, F_SEL);
DmDrawBmpFromSpiFlash drawImage = DmDrawBmpFromSpiFlash();

SPIFlash spiFlash(F_CS, 0xEF40);
uint16_t textRow = 0;

void setup()
{
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  pinMode(F_CS, OUTPUT);
  digitalWrite(F_CS, HIGH);
  
  Serial.begin(9600);
  //RA8875_320x240, RA8875_480x272 or RA8875_800x480
  tft.init(RA8875_320x240);
  tft.backlightOn(true); // Turn on backlight
  tft.backlightAdjust(255);   // default value max
  
  initFlash(spiFlash);
  tft.drawString(5, 0, "www.displaymodule.com");
  Serial.print(F("Initializing SD-card: "));
  PrintTestName("Initializing SD-card: ");
  if (SD.begin(SD_CS)) {
    PrintTestResult("OK");
    Serial.println(F("OK"));
    // SD-card found, write from SD-card to flash memory
    PrintTestName("Erase Flash... ");
    eraseFlash(spiFlash);
    PrintTestResult("OK");
    
    PrintTestName("Write bmp to ext. flash");
    writeFileToFlash(0, spiFlash, "logol565.bmp");
    PrintTestResult("OK");
  }
  else
  {
    Serial.println(F("Fail"));
  }
  
  if (! drawImage.printHeaderInfo(0, spiFlash)) {
    Serial.println(F("Can't open bitmap."));
    return;
  }
  
  uint32_t startTime, endTime;
  
  startTime = millis();
  drawImage.drawImage(0, spiFlash, tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
  
  delay(500);
  tft.clearScreen(BLACK);

  

  startTime = millis();
  drawImage.drawImage(0, spiFlash, tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
}

void loop() { }

void initFlash(SPIFlash spiFlash) {
  Serial.print(F("External Flash: "));
  if (spiFlash.initialize()) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("Fail"));
  }
}

void eraseFlash(SPIFlash spiFlash) {
  Serial.print(F("Erasing Flash: "));
  spiFlash.chipErase();
  while(spiFlash.busy());
  Serial.println(F("OK"));
}

void writeFileToFlash(uint32_t startAddress, SPIFlash spiFlash, char* fileName) {
  uint32_t fileSize;
  uint32_t counter;
  
  Serial.print(F("Write to Flash: "));
  
  File imageFile;
  if ((imageFile = SD.open(fileName)) == NULL) {
    Serial.println(F("Can't open image file"));
    return;
  }
  
  fileSize = imageFile.size();
  counter = startAddress;
  while(counter<=fileSize) {
    spiFlash.writeByte(counter, imageFile.read());
    counter++;
  }
  imageFile.close();
  
  Serial.println(F("OK"));
}
void PrintTestName(char* text) {
  textRow += 20;
  
  if(textRow >= tft.height()-20){
    tft.clearScreen(BLACK);
    textRow = 10;    
  }  
  tft.drawString(5, textRow, text);
  Serial.print(text);
}
void PrintTestResult(char* text) {
  tft.drawString(200, textRow, text);
  Serial.print(": ");
  Serial.println(text);
}
