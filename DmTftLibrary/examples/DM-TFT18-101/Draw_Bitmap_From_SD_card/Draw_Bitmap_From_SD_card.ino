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
#include <DmTftHX8353C.h>
#include <utility\DmDrawBmpFromSdCard.h>

#define SD_CS   10

DmTftHX8353C tft = DmTftHX8353C();
DmDrawBmpFromSdCard drawImage = DmDrawBmpFromSdCard();

void setup()
{ 
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
  
  if (! drawImage.printHeaderInfo("dm101565.bmp")) {
    Serial.println(F("Can open bitmap."));
    return;
  }
  
  if (! drawImage.printHeaderInfo("dm101888.bmp")) {
    Serial.println(F("Can open bitmap."));
    return;
  }
  
  uint32_t startTime, endTime;
  
  startTime = millis();
  drawImage.drawImage("dm101565.bmp", tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
  
  delay(500);
  tft.clearScreen();
  
  startTime = millis();
  drawImage.drawImage("dm101888.bmp", tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
}

void loop() { }
