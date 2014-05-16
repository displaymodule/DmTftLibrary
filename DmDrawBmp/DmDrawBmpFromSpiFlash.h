/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_DRAW_BMP_FROM_SPI_FLASH_h
#define DM_DRAW_BMP_FROM_SPI_FLASH_h

#include <Arduino.h>
#include <SPI.h>
#include <SPIFlash.h> // SPIFlash library from lowpowerlab
#include <DmTftBase.h>
#include "DmDrawBmpBase.h"

class DmDrawBmpFromSpiFlash : public DmDrawBmpBase
{
public:
  boolean printHeaderInfo(uint32_t startAddress, SPIFlash &spiFlash);
  boolean drawImage(uint32_t startAddress, SPIFlash &spiFlash, DmTftBase& tftBase, uint16_t x, uint16_t y);
protected:
  virtual boolean draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y);
  virtual boolean draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y);
  virtual void setPosition(uint32_t newPosition);
  virtual uint16_t read16();
private:
  SPIFlash* _spiFlash;
  uint32_t _filePosition; // Used when parsing header
};
#endif
