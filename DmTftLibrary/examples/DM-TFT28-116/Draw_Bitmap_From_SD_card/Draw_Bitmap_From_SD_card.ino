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
#include <Wire.h>
#include <SPIFlash.h>
#include <SD.h>
#include <DmTftIli9341.h>
#include <DmDrawBmpFromSdCard.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6

DmTftIli9341 tft = DmTftIli9341(10, 9);
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
  tft.init();
  
  Serial.print(F("Initializing SD-card: "));
  if (!SD.begin(SD_CS)) {
    Serial.println(F("Fail"));
    return;
  }
  else
  {
    Serial.println(F("OK"));
  }
  
  if (! drawImage.printHeaderInfo("logop565.bmp")) {
    Serial.println(F("Can open bitmap."));
    return;
  }
  
  if (! drawImage.printHeaderInfo("logop888.bmp")) {
    Serial.println(F("Can open bitmap."));
    return;
  }
  
  uint32_t startTime, endTime;
  
  startTime = millis();
  drawImage.drawImage("logop565.bmp", tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
  
  delay(500);
  tft.clearScreen();
  
  startTime = millis();
  drawImage.drawImage("logop888.bmp", tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
}

void loop() { }
