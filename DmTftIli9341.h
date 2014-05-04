/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TFT_ILI9341_h
#define DM_TFT_ILI9341_h

#include "DmTftBase.h"
#if defined (__AVR__)
  #include <SPI.h>
#elif defined (TOOLCHAIN_ARM_MICRO)
#endif
class DmTftIli9341 : public DmTftBase
{
public:
#if defined (__AVR__)
  DmTftIli9341(uint8_t cs, uint8_t dc);
#elif defined (TOOLCHAIN_ARM_MICRO)
  DmTftIli9341(uint8_t cs, uint8_t dc, uint8_t miso, uint8_t mosi, uint8_t clk);
#endif
  virtual ~DmTftIli9341();
  void init(void);
private:
  void send8BitData(uint8_t data);
  void writeBus(uint8_t data);
  
  virtual void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
  virtual void sendCommand(uint8_t index);
  virtual void sendData(uint16_t data);

  uint8_t _cs, _dc;
  static const uint16_t _width;
  static const uint16_t _height;
#if defined (__AVR__)
  regtype *_pinDC;
  regsize _bitmaskDC;
  uint8_t _spiSettings;
#elif defined (TOOLCHAIN_ARM_MICRO)
  uint8_t _miso, _mosi, _clk;
  DigitalOut *_pinDC;
  SPI *_spi;
#endif
};


#endif

