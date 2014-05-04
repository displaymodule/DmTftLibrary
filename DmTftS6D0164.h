/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TFT_S6D0164_h
#define DM_TFT_S6D0164_h

#include "DmTftBase.h"

class DmTftS6D0164 : public DmTftBase
{
public:
  DmTftS6D0164(uint8_t wr, uint8_t cs, uint8_t dc, uint8_t rst);
  virtual ~DmTftS6D0164();
  void init(void);
private:
  void send8BitData(uint8_t data);
  void writeBus(uint8_t data);
  
  virtual void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
  virtual void sendCommand(uint8_t index);
  virtual void sendData(uint16_t data);

  uint8_t _wr, _cs, _dc, _rst;
  static const uint16_t _width;
  static const uint16_t _height;
#if defined (__AVR__)
  regtype *_pinDC, *_pinRST, *_pinWR;
  regsize _bitmaskDC, _bitmaskRST, _bitmaskWR;
#elif defined (TOOLCHAIN_ARM_MICRO)
  DigitalOut* _pinDC, *_pinRST, *_pinWR;
  BusOut *_virtualPortD;
#endif
};
#endif

