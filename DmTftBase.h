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
#define RED 0xf800
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

extern uint8_t font[];
class DmTftBase {
public:
  DmTftBase(const uint16_t width, const uint16_t height) : _width(width),_height(height){};
  virtual ~DmTftBase() { };
  virtual void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) = 0;
  virtual void sendCommand(uint8_t index) = 0;
  virtual void sendData(uint16_t data) = 0;

  void setPixel(uint16_t posX, uint16_t posY,uint16_t color);
  void fillScreen(void);
  void fillScreen(uint16_t XL,uint16_t XR,uint16_t YU,uint16_t YD,uint16_t color);
  void fillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);
  void drawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color);
  void drawVerticalLine(uint16_t poX, uint16_t poY,uint16_t length,uint16_t color);
  void drawHorizontalLine(uint16_t poX, uint16_t poY,uint16_t length,uint16_t color);
  void drawRectangle(uint16_t poX, uint16_t poY, uint16_t length,uint16_t width,uint16_t color);
  void drawCircle(int poX, int poY, int r,uint16_t color);
  void fillCircle(int poX, int poY, int r,uint16_t color);
  void drawTraingle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color);
  void drawPoint(uint16_t x,uint16_t y);
  void drawPoint_big(uint16_t x,uint16_t y);
  void fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
  void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void drawCircle(uint16_t x0,uint16_t y0,uint8_t r);
  void showChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
  void showNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len);
  void show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len);
  void showString(uint16_t x,uint16_t y, const char *p);

#if defined (__AVR__)
  regtype *_pinCS;
  regsize _bitmaskCS;
#elif defined (TOOLCHAIN_ARM_MICRO)
  DigitalOut* _pinCS;
  uint8_t _bitmaskCS;
#endif

  uint16_t BACK_COLOR, POINT_COLOR;
  
private:
  const uint16_t _width;
  const uint16_t _height;
  uint32_t myPow(uint8_t m,uint8_t n);
};
#endif
 
