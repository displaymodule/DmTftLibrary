/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TFT_SSD2119_h
#define DM_TFT_SSD2119_h

#include "DmTftBase.h"

class DmTftSsd2119 : public DmTftBase
{
public:
#if defined (__AVR__)
  DmTftSsd2119(uint8_t cs, uint8_t dc);
#elif defined (TOOLCHAIN_ARM_MICRO)
  DmTftSsd2119(uint8_t cs=D10, uint8_t dc=D9, uint8_t miso=D12, uint8_t mosi=D11, uint8_t clk=D13);
#endif
  virtual ~DmTftSsd2119();
  void init(void);
  virtual void setPixel(uint16_t x, uint16_t y, uint16_t color);

  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  virtual void sendData(uint16_t data);
  
private:
  void send8BitData(uint8_t data);
  void writeBus(uint8_t data);

  virtual void sendCommand(uint8_t index);

  uint8_t _cs, _dc;
  static const uint16_t _width;
  static const uint16_t _height;
#if defined (DM_TOOLCHAIN_ARDUINO)
  regtype *_pinDC;
  regsize _bitmaskDC;
  uint8_t _spiSettings;
#elif defined (DM_TOOLCHAIN_MBED)
  uint8_t _miso, _mosi, _clk;
  DigitalOut *_pinDC;
  SPI *_spi;
#endif
};


#endif


