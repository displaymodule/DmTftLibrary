#include <SPI.h>
#include <DmTftHX8353C.h>
#include <utility/BubbleDemo.h>

DmTftHX8353C tft = DmTftHX8353C();
BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());

void setup ()
{
  tft.init();
}

void loop()
{
  bubbleDemo.run(750, 20);
}


