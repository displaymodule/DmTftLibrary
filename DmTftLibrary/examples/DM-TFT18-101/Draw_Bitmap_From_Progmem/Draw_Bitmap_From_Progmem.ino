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
#include <DmDrawBmpFromProgmem.h>

DmTftHX8353C tft = DmTftHX8353C();
DmDrawBmpFromProgmem drawImage = DmDrawBmpFromProgmem();

extern uint8_t dmlogo[];

void setup()
{
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
