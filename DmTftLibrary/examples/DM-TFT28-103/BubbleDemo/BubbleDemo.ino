#include <SPI.h>
#include <DmTftIli9325.h>
#include <utility/BubbleDemo.h>

DmTftIli9325 tft = DmTftIli9325();
BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());

void setup ()
{
  tft.init();
}

void loop()
{
  bubbleDemo.run(750, 20);
}


