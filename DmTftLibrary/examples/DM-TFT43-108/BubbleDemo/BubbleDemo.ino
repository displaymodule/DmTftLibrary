#include <SPI.h>
#include <DmTftRa8875.h>
#include <utility/BubbleDemo.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4

DmTftRa8875 tft = DmTftRa8875();

BubbleDemo bubbleDemo(&tft, 320, 240);  
// if use RA8875_480x272 use 
// BubbleDemo bubbleDemo(&tft, 480, 272);
// if use RA8875_800x480 use 
// BubbleDemo bubbleDemo(&tft, 800, 480);

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
  
  //RA8875_320x240, RA8875_480x272 or RA8875_800x480
  tft.init(RA8875_320x240);
  tft.backlightOn(true); // Turn on backlight
  tft.backlightAdjust(255);   // default value max
}

void loop()
{
  bubbleDemo.run(750, 20);
}


