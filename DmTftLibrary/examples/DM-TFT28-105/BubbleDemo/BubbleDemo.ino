#include <SPI.h>
#include <DmTftIli9341.h>
#include <utility/BubbleDemo.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4

DmTftIli9341 tft = DmTftIli9341(10, 9);
BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());

void setup ()
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
  
  tft.init();
}

void loop()
{
  bubbleDemo.run(750, 20);
}


