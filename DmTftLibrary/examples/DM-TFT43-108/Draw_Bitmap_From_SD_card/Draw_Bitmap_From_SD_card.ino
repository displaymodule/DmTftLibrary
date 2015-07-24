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
#include <SPIFlash.h>
#include <SD.h>
#include <DmTftRa8875.h>
#include <DmDrawBmpFromSdCard.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6


DmTftRa8875 tft = DmTftRa8875(10, 9);
DmDrawBmpFromSdCard drawImage = DmDrawBmpFromSdCard();

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
  
  Serial.print(F("Initializing SD-card: "));
  if (!SD.begin(SD_CS)) {
    Serial.println(F("Fail"));
    return;
  }
  else
  {
    Serial.println(F("OK"));
  }
  
  if (! drawImage.printHeaderInfo("logol565.bmp")) {
    Serial.println(F("Can open bitmap."));
    return;
  }
  
  if (! drawImage.printHeaderInfo("logol888.bmp")) {
    Serial.println(F("Can open bitmap."));
    return;
  }
  
  uint32_t startTime, endTime;
  
  startTime = millis();
  drawImage.drawImage("logol565.bmp", tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
  
  delay(500);
  tft.clearScreen(BLACK);
  
  startTime = millis();
  drawImage.drawImage("logol888.bmp", tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
}

void loop() { }
