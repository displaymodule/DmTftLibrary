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
#include <Wire.h>
#include <DmTftIli9341.h>
#include <DmTpFt6x06.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_IRQ   2

DmTftIli9341 tft = DmTftIli9341();
DmTpFt6x06 dmTouch = DmTpFt6x06(DmTpFt6x06::DM_TFT28_116 );

void setup() {
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(F_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(F_CS, HIGH);
  
  Serial.begin(115200);
  
  tft.init();
  dmTouch.init();
  tft.drawString(35, 10, "www.displaymodule.com");
  tft.drawString(40, 30, "please touch screen");
}

void loop() {
  uint16_t x, y = 0;
  bool touched = false;
  if (dmTouch.isTouched()) {
    dmTouch.readTouchData(x,y,touched);
    
    //Re-maps a number from one range to another
    x = map(x, 0, 240, 240, 0);
    y = map(y, 0, 320, 320, 0);    
    Serial.print("x = ");Serial.println(x,DEC); 
    Serial.print("y = ");Serial.println(y,DEC); 
    Serial.print("\r\n");    
    
    tft.drawPoint(x, y, 1);
  }
}
















