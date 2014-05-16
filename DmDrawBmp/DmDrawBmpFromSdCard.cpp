/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmDrawBmpFromSdCard.h"

boolean DmDrawBmpFromSdCard::drawImage(char* fileName, DmTftBase& tft, uint16_t x, uint16_t y) {
  if ( !(readHeader(fileName)) ){ // Open and parse the header - __Will not close the file__
    return false;
  }
  
  if (IsValid565Bitmap()) {
    boolean success = draw565Bitmap(tft, x, y);
    _imageFile.close();
    return success;
  }
  if (IsValid888Bitmap()) {
    boolean success = draw888Bitmap(tft, x, y);
    _imageFile.close();
    return success;
  }
  _imageFile.close();
  
  return false; // Not a valid 888 or 565 image
}

boolean DmDrawBmpFromSdCard::draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) {
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
    if(_imageFile.position() != filePosition) {
      tft.unSelect();
      _imageFile.seek(filePosition);
      tft.select();
      buffPos = sizeof(buff);
    }
    
    for(column=0; column<_width; column++) { 
      if (buffPos >= sizeof(buff)) {
        tft.unSelect();
        _imageFile.read(buff, sizeof(buff));
        tft.select();
        buffPos = 0;
      }
          
      blue = buff[buffPos++];
      green = buff[buffPos++];
      red = buff[buffPos++];

      tft.sendData(Convert888to565(red, green, blue));
    }
  }
  tft.unSelect();
  return true;
}

boolean DmDrawBmpFromSdCard::draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y) {
  const uint8_t bytesPerPixel = 2;
  uint8_t buff[30*bytesPerPixel]; // Should be dividable by bytesPerPixel
  uint8_t buffPos = sizeof(buff); 
  uint16_t bytesPerRow = (bytesPerPixel * _width + 3) & ~3; // bytes Per Row including padding to 4 bytes boundary
  uint16_t paddingSize = bytesPerRow - (bytesPerPixel * _width); // paddingSize for each row
  uint16_t height = -_height; // Change if load bottom-top
  uint16_t pixel;
  uint16_t row, column;
  
  _imageFile.seek(_bitmapOffset);
    
  tft.select();
  tft.setAddress(x, y, x+_width-1, y+height-1);
  tft.unSelect();

  for(row=0; row<height; row++) {
    for(column=0; column<_width; column++) {
      if (buffPos >= sizeof(buff)) {
        tft.unSelect();
        _imageFile.read(buff, sizeof(buff));
        tft.select();
        buffPos = 0;
      }
      pixel = buff[buffPos++] & 0xFF;
      pixel |= buff[buffPos++] << 8;
      tft.sendData(pixel);
    }

    if ( paddingSize > 0 ) { // Check if there is padding in the file     
      if ((sizeof(buff) - buffPos) >= paddingSize) { // Most common case, the padding is in the buffer 
        buffPos += paddingSize;
      }
      else { // Padding is not in the buffer, we have to load the buffer from file       
        tft.unSelect();
        _imageFile.read(buff, sizeof(buff));
        tft.select();
        buffPos = paddingSize-(sizeof(buff) - buffPos); // paddingSize (0-3) spaceLeftInBuffer (0-3)  where spaceLeftInBuffer < paddingSize
      }
    }
  }
  tft.unSelect();
  
  return true;
}

boolean DmDrawBmpFromSdCard::printHeaderInfo(char* fileName) { // For debugging
  if (!readHeader(fileName))
  {
    return false;
  }
  _imageFile.close();

  printBmpHeaderInfo();
  
  return true;
}

// Info about bmp format can be found here: http://en.wikipedia.org/wiki/BMP_file_format
boolean DmDrawBmpFromSdCard::readHeader(char* fileName) {
  File imageFile;
  
  if ((imageFile = SD.open(fileName)) == NULL) {
    return false;
  }
  _imageFile = imageFile;
  
  if (!readBmpHeader()) {
    _imageFile.close();
    return false;
  }

  return true;
}

void DmDrawBmpFromSdCard::setPosition(uint32_t newPosition) {
  _imageFile.seek(newPosition);
}

uint16_t DmDrawBmpFromSdCard::read16() {
  uint16_t d;
  uint8_t b;
  b = _imageFile.read();
  d = _imageFile.read();
  d <<= 8;
  d |= b;
  return d;
}
