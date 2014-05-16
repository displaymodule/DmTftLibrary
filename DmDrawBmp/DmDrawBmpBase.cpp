/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmDrawBmpBase.h"

void DmDrawBmpBase::printBmpHeaderInfo() {
  Serial.print(F("Image size:         ")); 
  Serial.println(_fileSize, DEC);
  Serial.print(F("Image offset:       ")); 
  Serial.println(_bitmapOffset, DEC);
  Serial.print(F("Image size:         ")); 
  Serial.print(_width, DEC);
  Serial.print(F(", "));
  Serial.println(_height, DEC);
  Serial.print(F("BitsPerPixel:       ")); 
  Serial.println(_bitsPerPixel, DEC);
  Serial.print(F("Compression:        ")); 
  Serial.println(_compression, DEC);
  Serial.print(F("Is 24-bit bmp:      "));
  Serial.println(IsValid888Bitmap());  
  Serial.print(F("Is 16-bit 565 bmp:  "));
  Serial.println(IsValid565Bitmap());  
  Serial.print(F("Has 565 color mask: "));
  Serial.println(Is565ColorMask());  
}

boolean DmDrawBmpBase::readBmpHeader() {
  if (read16() !=0x4D42){ // read magic byte
    return false;
  }

  _fileSize = read32();
  read32(); // Value depends on application which created the image 
  _bitmapOffset = read32();

  // read DIB header
  _headerSize = read32();
  _width = readInt32();
  _height = readInt32();

  if (read16() != 1) { // number of color planes must be 1
    return false;
  }
  
  _bitsPerPixel = read16();
  _compression = read32();
  
  if (_bitmapOffset == 66 || _bitmapOffset == 70) { // V3 or v2 format
    setPosition(54);
    _redMask = read32();
    _greenMask = read32();
    _blueMask = read32();
  }
  else {
    _redMask = 0x00;
    _greenMask = 0x00;
    _blueMask = 0x00;
  }
  
  if (!IsValid888Bitmap() && !IsValid565Bitmap())
  {
    return false;
  }
  
  return true;
}

// In this context a valid bitmap
// - Stored bottom to top
// - 24-bit file
// - No compression
boolean DmDrawBmpBase::IsValid888Bitmap() {
  if (_height > 0 && _bitsPerPixel == 24 && _compression == 0)
  {
    return true;
  }
  return false;
}

// In this context a valid bitmap
// - Stored top to bottom
// - 16-bit file
// - Compression 3 (BI_BITFIELDS)
// - Have a 565 Colormask
boolean DmDrawBmpBase::IsValid565Bitmap() {
  if (_height < 0 && _bitsPerPixel == 16 && _compression == 3 && Is565ColorMask())
  {
    return true;
  }
  return false;
}

boolean DmDrawBmpBase::Is565ColorMask() {
  if (_redMask == 0xF800 && _greenMask == 0x7E0 && _blueMask == 0x1F)
  {
    return true;
  }
  return false;
}

int32_t DmDrawBmpBase::readInt32() {
  int32_t d;
  uint16_t b;

  b = read16();
  d = read16();
  d <<= 16;
  d |= b;
  return d;
}

uint32_t DmDrawBmpBase::read32() {
  uint32_t d;
  uint16_t b;

  b = read16();
  d = read16();
  d <<= 16;
  d |= b;
  return d;
}

// http://stackoverflow.com/questions/2442576/how-does-one-convert-16-bit-rgb565-to-24-bit-rgb888
uint16_t DmDrawBmpBase::Convert888to565(uint8_t red, uint8_t green, uint8_t blue){
  return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}
