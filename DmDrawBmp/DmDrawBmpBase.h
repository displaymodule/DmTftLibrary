/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_DRAW_BMP_BASE_h
#define DM_DRAW_BMP_BASE_h

#include <Arduino.h>
#include <DmTftBase.h>

class DmDrawBmpBase {
protected:
  virtual boolean draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) = 0;
  virtual boolean draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) = 0;
  virtual void setPosition(uint32_t newPosition) = 0;
  virtual uint16_t read16() = 0;
  
  void printBmpHeaderInfo();
  boolean readBmpHeader();
  boolean IsValid888Bitmap();
  boolean IsValid565Bitmap();
  boolean Is565ColorMask();
  uint16_t Convert888to565(uint8_t red, uint8_t green, uint8_t blue);

  uint32_t read32();
  int32_t readInt32();
    
  uint32_t _fileSize;
  uint32_t _bitmapOffset;

  uint32_t _headerSize;
  int32_t _width, _height;
  uint16_t _bitsPerPixel;
  uint32_t _compression;
  uint32_t _redMask;
  uint32_t _greenMask;
  uint32_t _blueMask;
};
#endif
