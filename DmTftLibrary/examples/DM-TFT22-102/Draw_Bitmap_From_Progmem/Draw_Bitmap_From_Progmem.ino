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
#include <DmTftS6D0164.h>
#include <DmDrawBmpFromProgmem.h>

DmTftS6D0164 tft = DmTftS6D0164();
DmDrawBmpFromProgmem drawImage = DmDrawBmpFromProgmem();

extern uint8_t dmlogo[];

void setup()
{
  tft.init();
  drawImage.drawImage(dmlogo, tft, 0, 0);
}

void loop() { }
