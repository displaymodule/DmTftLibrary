#include <SPI.h>
#include <DmTftS6D0164.h>
#include <utility/BubbleDemo.h>

DmTftS6D0164 tft = DmTftS6D0164();
BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());

void setup ()
{
  tft.init();
}

void loop()
{
  bubbleDemo.run(750, 20);
}


