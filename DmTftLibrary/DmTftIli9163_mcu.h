/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TFT_ILI9163_MCU_h
#define DM_TFT_ILI9163_MCU_h

#include "DmTftBase.h"

class DmTftIli9163_mcu: public DmTftBase
{
public:
  DmTftIli9163_mcu(uint8_t wr=A4, uint8_t cs=A3, uint8_t dc=A5, uint8_t rst=A2);
  virtual ~DmTftIli9163_mcu();
  void init(void);
private:
  void send8BitData(uint8_t data);
  void writeBus(uint8_t data);

  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  virtual void sendCommand(uint8_t index);
  virtual void sendData(uint16_t data);

  uint8_t _wr, _cs, _dc, _rst;
  static const uint16_t _width;
  static const uint16_t _height;
#if defined (DM_TOOLCHAIN_ARDUINO)
  regtype *_pinDC, *_pinRST, *_pinWR;
  regsize _bitmaskDC, _bitmaskRST, _bitmaskWR;
#elif defined (DM_TOOLCHAIN_MBED)
  DigitalOut* _pinDC, *_pinRST, *_pinWR;
  BusOut *_virtualPortD;
#endif
};
#endif


