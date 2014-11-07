/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TFT_RA8875_h
#define DM_TFT_RA8875_h

#include "DmTftBase.h"

enum RA8875Size{ 
	RA8875_480x272, 
	RA8875_800x480 
};

class DmTftRa8875 : public DmTftBase
{
public:	
#if defined (DM_TOOLCHAIN_ARDUINO)
  DmTftRa8875(uint8_t cs=D10, uint8_t sel=D9);
#elif defined (DM_TOOLCHAIN_MBED)
  DmTftRa8875(uint8_t cs=D10, uint8_t sel=D9, uint8_t miso=D12, uint8_t mosi=D11, uint8_t clk=D13);
#endif
  virtual ~DmTftRa8875();
  void init(enum RA8875Size size);
	void w25CtrlByRa8875(void);
	void w25CtrlByMCU(void);
	void clearScreen(uint16_t color);
	void dispAscii(uint16_t x, uint16_t y, const char *p);
	void setFontColor(uint16_t background,uint16_t foreground);
	void setFontZoom(uint8_t Hsize, uint8_t Vsize);
	void eableKeyScan(bool on);
	bool isKeyPress(void);
	uint8_t getKeyValue(void);
	void backlightOn(bool on);
	void backlightAdjust(uint8_t value);	
	void drawImageContinuous(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void drawImageBlock(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t pic_width, uint16_t block_width, uint16_t block_height);
 	uint16_t width(void);
	uint16_t height(void);
	
private:
  void writeBus(uint8_t data);
  uint8_t readBus(void);  
  virtual void sendCommand(uint8_t index);
	uint8_t readStatus(void);
  virtual void sendData(uint16_t data);
	void send8BitData(uint8_t data);
  uint8_t readData(void);
  virtual void writeReg(uint8_t reg, uint8_t val);
  uint8_t readReg(uint8_t reg);
  
  void rectHelper  (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled);
  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	uint16_t _width;
	uint16_t _height;

  enum RA8875Size _size;
	
  uint8_t _cs, _sel;
#if defined (DM_TOOLCHAIN_ARDUINO)
  regtype *_pinSEL;
  regsize _bitmaskSEL;
  uint8_t _spiSettings;
#elif defined (DM_TOOLCHAIN_MBED)
  uint8_t _miso, _mosi, _clk;
  DigitalOut *_pinSEL;
  SPI *_spi;
#endif
};

#endif

