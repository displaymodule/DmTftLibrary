/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_DRAW_BMP_FROM_SD_CARD_h
#define DM_DRAW_BMP_FROM_SD_CARD_h

#include <Arduino.h>
#include <SD.h>
#include <DmTftBase.h>
#include "DmDrawBmpBase.h"

class DmDrawBmpFromSdCard : public DmDrawBmpBase
{
public:
  boolean printHeaderInfo(char* fileName);
  boolean drawImage(char* fileName, DmTftBase& tftBase, uint16_t x, uint16_t y); 
protected:
  virtual boolean draw565Bitmap(DmTftBase& tft, uint16_t x, uint16_t y);
  virtual boolean draw888Bitmap(DmTftBase& tft, uint16_t x, uint16_t y);
  virtual void setPosition(uint32_t newPosition);
  virtual boolean readHeader(char* fileName);
  virtual uint16_t read16();
private:
  File _imageFile;
};
#endif
