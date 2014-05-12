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

#include <SPI.h>

#if defined (__AVR__)
  #include <Arduino.h>
  #include <avr/pgmspace.h>
  #define gbi(reg, _bitmask) ((*reg) & (_bitmask))
  #define cbi(reg, _bitmask) *reg &= ~_bitmask
  #define sbi(reg, _bitmask) *reg |= _bitmask
  #define pulse_high(reg, _bitmask) sbi(reg, _bitmask); cbi(reg, _bitmask);
  #define pulse_low(reg, _bitmask) cbi(reg, _bitmask); sbi(reg, _bitmask);
#elif defined (TOOLCHAIN_ARM_MICRO)
  #include "mbed.h"
  #define sbi(reg, _bitmask) *reg = 1;
  #define cbi(reg, _bitmask) *reg = 0;
  #define pulse_high(reg, _bitmask) *reg = 1; *reg = 0;
  #define pulse_low(reg, _bitmask) *reg = 0; *reg = 1;
  #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
  #define delay(ms) wait_ms(ms);
#endif

#ifndef uint8_t
  #define uint8_t unsigned char
#endif
#ifndef int8_t
  #define int8_t signed char
#endif
#ifndef uint16_t
  #define uint16_t unsigned short
#endif
#ifndef uint32_t
  #define uint32_t unsigned long
#endif
#ifndef regtype
  #define regtype volatile unsigned char
#endif
#ifndef regsize
  #define regsize unsigned char
#endif

class DmTouch
{
public:
  enum Display {
    DM_TFT28_103,
    DM_TFT24_104,
    DM_TFT28_105,
	DM_TFT35_107,
  };

#if defined (__AVR__)
  DmTouch(Display disp, int8_t cs, int8_t tIrq = -1, int8_t hardwareSpi = 1, int8_t clk = -1, int8_t mosi = -1, int8_t miso = -1, uint16_t width = 240, uint16_t height = 320);
#elif defined (TOOLCHAIN_ARM_MICRO)
  DmTouch(Display disp, int8_t cs = D4, int8_t irq = D2, int8_t clk = D13, int8_t mosi = D11, int8_t miso = D12);
#endif
  void begin();
  void readTouchData(uint16_t& posX, uint16_t& posY, bool& touching);
  uint8_t isTouched();
private:
  void spiWrite(uint8_t data);
  uint8_t spiRead();
  uint16_t readData12(uint8_t command);
  void enableIrq();

  uint16_t _width, _height;
  int8_t _cs, _clk, _mosi, _miso;
  int8_t _irq;

  int _calibLowX, _calibLowY;
  float _calibModifierX, _calibModifierY;
  bool _calibInvertedTouch;

#if defined (__AVR__)
  regtype *_pinDC, *_pinCS, *_pinCLK, *_pinMOSI, *_pinMISO, *_pinIrq;
  regsize _bitmaskDC, _bitmaskCS, _bitmaskCLK, _bitmaskMOSI, _bitmaskMISO, _bitmaskIrq;
  uint8_t _spiSettings;
  int8_t _hardwareSpi;
#elif defined (TOOLCHAIN_ARM_MICRO)
  DigitalOut *_pinCS, *_led;
  DigitalIn *_pinIrq;
  SPI *_spi;

#endif

};
#endif
