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

class DmTftIli9341 : public DmTftBase
{
public:
#if defined (DM_TOOLCHAIN_ARDUINO)
  DmTftIli9341(uint8_t cs=D10, uint8_t dc=D9);
#elif defined (DM_TOOLCHAIN_MBED)
  DmTftIli9341(uint8_t cs=D10, uint8_t dc=D9, uint8_t miso=D12, uint8_t mosi=D11, uint8_t clk=D13);
#endif
  virtual ~DmTftIli9341();
  void init(void);
private:
  void send8BitData(uint8_t data);
  void writeBus(uint8_t data);

  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  virtual void sendCommand(uint8_t index);
  virtual void sendData(uint16_t data);

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


