/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
#ifndef DMTFTBASE_h
#define DMTFTBASE_h

#if defined (__AVR__)
  #include <Arduino.h>
  #include <avr/pgmspace.h>
  #define cbi(reg, _bitmask) *reg &= ~_bitmask
  #define sbi(reg, _bitmask) *reg |= _bitmask
  #define pulse_high(reg, _bitmask) sbi(reg, _bitmask); cbi(reg, _bitmask);
  #define pulse_low(reg, _bitmask) cbi(reg, _bitmask); sbi(reg, _bitmask);

  #define D2   2
  #define D3   3
  #define D4   4
  #define D5   5
  #define D6   6
  #define D9   9
  #define D10 10
  #define A2  16
  #define A3  17
  #define A4  18
  #define A5  19

#elif defined (TOOLCHAIN_ARM_MICRO)
  #include "mbed.h"
  #define sbi(reg, _bitmask) *reg = 1;
  #define cbi(reg, _bitmask) *reg = 0;
  #define pulse_high(reg, _bitmask) *reg = 1; *reg = 0;
  #define pulse_low(reg, _bitmask) *reg = 0; *reg = 1;
  #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
  #define delay(ms) wait_ms(ms);
#endif

//Basic Colors
#define RED     0xf800
#define GREEN   0x07e0
#define BLUE    0x001f
#define BLACK   0x0000
#define YELLOW  0xffe0
#define WHITE   0xffff

//Other Colors
#define CYAN        0x07ff
#define BRIGHT_RED  0xf810
#define GRAY1       0x8410
#define GRAY2       0x4208

#ifndef uint8_t
  #define uint8_t unsigned char
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

class DmTftBase {
public:
  DmTftBase(const uint16_t width, const uint16_t height) : _width(width), _height(height){};
  virtual ~DmTftBase() { };

  uint16_t width() { return _width; }
  uint16_t height() { return _height; }

  void setTextColor(uint16_t background, uint16_t foreground) { _bgColor = background; _fgColor = foreground; }

  void setPixel(uint16_t x, uint16_t y, uint16_t color);

  void clearScreen(uint16_t color = BLACK);

  void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
  void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);

  void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  void fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

  void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

  void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

  void drawPoint(uint16_t x, uint16_t y, uint16_t radius=0);

  void drawChar(uint16_t x, uint16_t y, char ch, bool transparent);
  void drawNumber(uint16_t x, uint16_t y, int num, int digitsToShow, bool leadingZeros=false);
  void drawString(uint16_t x, uint16_t y, const char *p);

  void drawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* data);

protected:

  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) = 0;
  virtual void sendCommand(uint8_t index) = 0;
  virtual void sendData(uint16_t data) = 0;

#if defined (__AVR__)
  regtype *_pinCS;
  regsize _bitmaskCS;
#elif defined (TOOLCHAIN_ARM_MICRO)
  DigitalOut* _pinCS;
  uint8_t _bitmaskCS;
#endif

private:
  const uint16_t _width;
  const uint16_t _height;

  uint16_t _bgColor;
  uint16_t _fgColor;
};
#endif


