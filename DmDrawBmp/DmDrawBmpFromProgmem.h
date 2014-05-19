/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_DRAW_BMP_FROM_PROGMEM_h
#define DM_DRAW_BMP_FROM_PROGMEM_h

#include <Arduino.h>
//#include <SPI.h>
//#include <SPIFlash.h> // SPIFlash library from lowpowerlab
#include <avr/pgmspace.h>
#include <DmTftBase.h>
#include "DmDrawBmpBase.h"

class DmDrawBmpFromProgmem : public DmDrawBmpBase
{
public:
  boolean printHeaderInfo(uint8_t* imageBuffer);
  boolean drawImage(uint8_t* imageBuffer, DmTftBase& tft, uint16_t x, uint16_t y);
protected:
  virtual boolean draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y);
  virtual boolean draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y);
  virtual void setPosition(uint32_t newPosition);
  virtual uint16_t read16();
  virtual uint32_t read32();
  virtual int32_t readInt32();
private:
  uint8_t* _imageBuffer;
  uint32_t _filePosition; // Used when parsing header
};
#endif
