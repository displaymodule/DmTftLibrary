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
#include <DmTftIli9341v.h>
#include <utility/BubbleDemo.h>

DmTftIli9341v tft = DmTftIli9341v();
BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());

void setup ()
{
  tft.init();
}

void loop()
{
  bubbleDemo.run(750, 20);
}


