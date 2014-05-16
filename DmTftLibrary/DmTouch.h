/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TOUCH_h
#define DM_TOUCH_h

#include "dm_platform.h"

class DmTouch
{
public:
  enum Display {
    DM_TFT28_103,
    DM_TFT24_104,
    DM_TFT28_105,
	DM_TFT35_107
  };

#if defined (DM_TOOLCHAIN_ARDUINO)
  DmTouch(Display disp, uint8_t cs, int8_t tIrq = -1, int8_t hardwareSpi = 1, uint8_t clk = -1, uint8_t mosi = -1, uint8_t miso = -1, uint16_t width = 240, uint16_t height = 320);
#elif defined (DM_TOOLCHAIN_MBED)
  DmTouch(Display disp, bool hardwareSPI=true);
#endif
  void init();
  void readTouchData(uint16_t& posX, uint16_t& posY, bool& touching);
  uint8_t isTouched();
private:
  void spiWrite(uint8_t data);
  uint8_t spiRead();
  uint16_t readData12(uint8_t command);
  void enableIrq();

  uint16_t _width, _height;
  uint8_t _cs, _clk, _mosi, _miso;
  uint8_t _irq;

  int _calibLowX, _calibLowY;
  float _calibModifierX, _calibModifierY;
  bool _calibInvertedTouch;
  bool _hardwareSpi;

#if defined (DM_TOOLCHAIN_ARDUINO)
  regtype *_pinDC, *_pinCS, *_pinCLK, *_pinMOSI, *_pinMISO, *_pinIrq;
  regsize _bitmaskDC, _bitmaskCS, _bitmaskCLK, _bitmaskMOSI, _bitmaskMISO, _bitmaskIrq;
  uint8_t _spiSettings;
#elif defined (DM_TOOLCHAIN_MBED)
  DigitalOut *_pinDC, *_pinCS, *_pinCLK, *_pinMOSI, *_pinMISO, *_led;
  DigitalIn *_pinIrq;
  SPI *_spi;
#endif

};
#endif



