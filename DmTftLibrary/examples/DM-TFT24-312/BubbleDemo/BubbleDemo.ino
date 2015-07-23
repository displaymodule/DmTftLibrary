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
#include <DmTftSt7789.h>
#include <utility/BubbleDemo.h>

#define TFT_CS  17
#define T_CS    4

DmTftSt7789 tft = DmTftSt7789(17, 19, 16);
BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());

void setup ()
{
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  pinMode(T_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(T_CS, HIGH);
 
  tft.init();
}

void loop()
{
  bubbleDemo.run(750, 20);
}


