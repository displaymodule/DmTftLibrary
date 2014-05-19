/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmDrawBmpFromProgmem.h"

boolean DmDrawBmpFromProgmem::drawImage(uint8_t* imageBuffer, DmTftBase& tft, uint16_t x, uint16_t y) {
  _imageBuffer = imageBuffer;
  _filePosition = 0;
  
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

boolean DmDrawBmpFromProgmem::draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) { 
  const uint8_t bytesPerPixel = 3;
  uint32_t filePosition = _bitmapOffset; 
  
  uint8_t red, green, blue;
  uint16_t row, column;
  uint16_t bytesPerRow = (bytesPerPixel*_width + 3) & ~3;
  
  tft.select();
  tft.setAddress(x, y, x+_width-1, y+_height-1);
  
  for(row=0; row<_height; row++) {
    _filePosition = _bitmapOffset + (_height - 1 -row ) * bytesPerRow;
       
    for(column=0; column<_width; column++) {          
      blue = pgm_read_byte_near(_imageBuffer+_filePosition++);
      green = pgm_read_byte_near(_imageBuffer+_filePosition++);
      red = pgm_read_byte_near(_imageBuffer+_filePosition++);
      
      tft.sendData(Convert888to565(red, green, blue));
    }
  }
  tft.unSelect();
  return true;
}

boolean DmDrawBmpFromProgmem::draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) {
  _filePosition = _bitmapOffset;
  
  const uint8_t bytesPerPixel = 2;
  uint16_t bytesPerRow = (bytesPerPixel * _width + 3) & ~3; // bytes Per Row including padding to 4 byte row boundary
  uint16_t paddingSize = bytesPerRow - (bytesPerPixel * _width); // paddingSize for each row
  uint16_t height = -_height; // Change for top->bottom
  uint16_t pixel;
  uint16_t row, column;
     
  tft.select();
  tft.setAddress(x, y, x+_width-1, y+height-1);
   
  for(row=0; row<height; row++) {
    for(column=0; column<_width; column++) {
      pixel = pgm_read_byte_near(_imageBuffer+_filePosition++) & 0xFF;
      pixel |= pgm_read_byte_near(_imageBuffer+_filePosition++) << 8;
      tft.sendData(pixel);
    }

    if ( paddingSize > 0 ) { // Check if there is padding in the row   
		_filePosition += paddingSize;
    }
  }
  tft.unSelect();
  
  return true;
}

boolean DmDrawBmpFromProgmem::printHeaderInfo(uint8_t* imageBuffer) { 
  _imageBuffer = imageBuffer;
  _filePosition = 0;
	  
  if (!readBmpHeader()) {
    return false;
  }

  printBmpHeaderInfo();
  
  return true;
}

void DmDrawBmpFromProgmem::setPosition(uint32_t newPosition) {
  _filePosition = newPosition;
}

uint16_t DmDrawBmpFromProgmem::read16() {
  uint16_t word = pgm_read_word_near(_imageBuffer+_filePosition);
  _filePosition += 2;
  return word;
}

uint32_t DmDrawBmpFromProgmem::read32() {
  uint32_t dWord = pgm_read_dword_near(_imageBuffer+_filePosition);
  _filePosition += 4;
  return dWord;
}
int32_t DmDrawBmpFromProgmem::readInt32() {
  int32_t dWord = pgm_read_dword_near(_imageBuffer+_filePosition);
  _filePosition += 4;
  return dWord;
}

