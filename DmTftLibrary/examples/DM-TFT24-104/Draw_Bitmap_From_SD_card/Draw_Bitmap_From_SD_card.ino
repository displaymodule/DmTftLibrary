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
#include <DmTftIli9325.h>
#include <utility\DmDrawBmpFromSdCard.h>

#define SD_CS   10

DmTftIli9325 tft = DmTftIli9325();
DmDrawBmpFromSdCard drawImage = DmDrawBmpFromSdCard();

void setup()
{ 
  tft.init();
  
  if (!SD.begin(SD_CS)) {
    return;
  }
 
  drawImage.drawImage("dm102565.bmp", tft, 0, 0);
  
  delay(500);
  tft.clearScreen();
  
  drawImage.drawImage("dm102888.bmp", tft, 0, 0);
}

void loop() { }
