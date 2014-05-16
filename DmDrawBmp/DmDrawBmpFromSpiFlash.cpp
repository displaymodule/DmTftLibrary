/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmDrawBmpFromSpiFlash.h"

boolean DmDrawBmpFromSpiFlash::drawImage(uint32_t startAddress, SPIFlash &spiFlash, DmTftBase& tft, uint16_t x, uint16_t y) {
  _spiFlash  = &spiFlash;
  _filePosition = startAddress;
  
  if (!readBmpHeader()) {
    return false;
  }
  
  if (IsValid565Bitmap()) {
    return draw565Bitmap(tft, x, y);
  }
  if (IsValid888Bitmap()) {
    return draw888Bitmap(tft, x, y);
  }
  
  return false; // Not valid 888 or 565 image
}

boolean DmDrawBmpFromSpiFlash::draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) { 
  const uint8_t bytesPerPixel = 3;
  uint32_t filePosition = _bitmapOffset; 
  uint8_t red, green, blue;
  uint16_t row, column;
  uint16_t bytesPerRow = (bytesPerPixel*_width + 3) & ~3;
  uint8_t buff[20*bytesPerPixel];
  uint8_t buffPos = sizeof(buff);
  
  tft.select();
  tft.setAddress(x, y, x+_width-1, y+_height-1);
  tft.unSelect();
  
  for(row=0; row<_height; row++) {
    filePosition = _bitmapOffset + (_height - 1 -row ) * bytesPerRow;
    buffPos = sizeof(buff);
       
    for(column=0; column<_width; column++) { 
      if (buffPos >= sizeof(buff)) {
        tft.unSelect();
        _spiFlash->readBytes(filePosition, buff, sizeof(buff));
        tft.select();
        buffPos = 0;
      }
          
      blue = buff[buffPos++];
      green = buff[buffPos++];
      red = buff[buffPos++];
      filePosition += 3;
      
      tft.sendData(Convert888to565(red, green, blue));
    }
  }
  tft.unSelect();
  return true;
}

boolean DmDrawBmpFromSpiFlash::draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) {
  uint32_t filePosition = _bitmapOffset;
  const uint8_t bytesPerPixel = 2;
  uint8_t buff[30*bytesPerPixel];
  uint8_t buffPos = sizeof(buff); 
  uint16_t bytesPerRow = (bytesPerPixel * _width + 3) & ~3; // bytes Per Row including padding to 4 byte row boundary
  uint16_t paddingSize = bytesPerRow - (bytesPerPixel * _width); // paddingSize for each row
  uint16_t height = -_height; // Change for top->bottom
  uint16_t pixel;
  uint16_t row, column;
     
  tft.select();
  tft.setAddress(x, y, x+_width-1, y+height-1);
  tft.unSelect();

  for(row=0; row<height; row++) {
    for(column=0; column<_width; column++) {
      if (buffPos >= sizeof(buff)) {
        tft.unSelect();
        _spiFlash->readBytes(filePosition, buff, sizeof(buff));
        tft.select();
        filePosition += sizeof(buff);
        buffPos = 0;
      }
      pixel = buff[buffPos++] & 0xFF;
      pixel |= buff[buffPos++] << 8;
      tft.sendData(pixel);
    }

    if ( paddingSize > 0 ) { // Check if there is padding in the row     
      if ((sizeof(buff) - buffPos) >= paddingSize) { // Most common case, the padding is in the buffer 
        buffPos += paddingSize;
      }
      else { // Padding is not in the buffer, we have to load the buffer from file       
        tft.unSelect();
        _spiFlash->readBytes(filePosition, buff, sizeof(buff));
        tft.select();
        filePosition += sizeof(buff);
        buffPos = paddingSize-(sizeof(buff) - buffPos); // paddingSize (0-3) spaceLeftInBuffer (0-3)  where spaceLeftInBuffer < paddingSize
      }
    }
  }
  tft.unSelect();
  
  return true;
}

boolean DmDrawBmpFromSpiFlash::printHeaderInfo(uint32_t startAddress, SPIFlash &spiFlash) {
  _spiFlash  = &spiFlash;
  _filePosition = startAddress;
  
  if (!readBmpHeader()) {
    return false;
  }

  printBmpHeaderInfo();
  
  return true;
}

void DmDrawBmpFromSpiFlash::setPosition(uint32_t newPosition) {
  _filePosition = newPosition;
}

uint16_t DmDrawBmpFromSpiFlash::read16() {
  uint16_t d;
  uint8_t b;
  b = _spiFlash->readByte(_filePosition++);
  d = _spiFlash->readByte(_filePosition++);
  d <<= 8;
  d |= b;
  return d;
}
