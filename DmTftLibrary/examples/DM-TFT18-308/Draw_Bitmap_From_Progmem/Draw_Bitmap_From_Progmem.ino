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
#include <SPIFlash.h>
#include <SD.h>
#include <DmTftIli9163_mcu.h>
#include <DmDrawBmpFromProgmem.h>

DmTftIli9163_mcu tft = DmTftIli9163_mcu();
DmDrawBmpFromProgmem drawImage = DmDrawBmpFromProgmem();

extern uint8_t dmlogo[];

void setup()
{
  tft.init();
  drawImage.drawImage(dmlogo, tft, 0, 0);
}

void loop() { }
