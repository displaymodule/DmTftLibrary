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
#include <DmTftSsd2119.h>
#include <DmDrawBmpFromProgmem.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4

DmTftSsd2119 tft = DmTftSsd2119(10, 9);
DmDrawBmpFromProgmem drawImage = DmDrawBmpFromProgmem();

extern uint8_t dmlogo[];

void setup()
{
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  pinMode(T_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(F_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(T_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(F_CS, HIGH);
  
  Serial.begin(9600);
  tft.init();
   
  if (! drawImage.printHeaderInfo(dmlogo)) {
    Serial.println(F("Can not open bitmap."));
    return;
  }
  
  uint32_t startTime, endTime;
  
  startTime = millis();
  drawImage.drawImage(dmlogo, tft, 0, 0);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
}

void loop() { }
