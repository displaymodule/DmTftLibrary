/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmTftIli9163c.h"
#if defined (DM_TOOLCHAIN_ARDUINO)
DmTftIli9163c::DmTftIli9163c(uint8_t cs, uint8_t dc, uint8_t rst)
#elif defined (DM_TOOLCHAIN_MBED)
DmTftIli9163c::DmTftIli9163c(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t miso, uint8_t mosi, uint8_t clk)
#endif
: DmTftBase(128, 160){
  _cs = cs;
  _dc = dc;
	_rst = rst;
#if defined (DM_TOOLCHAIN_MBED)
  _miso = miso;
  _mosi = mosi;
  _clk = clk;
#endif
}

DmTftIli9163c::~DmTftIli9163c() {
#if defined (DM_TOOLCHAIN_MBED)
delete _pinCS;
delete _pinDC;
delete _spi;

_pinCS = NULL;
_pinDC = NULL;
_spi = NULL;
#endif
}

void DmTftIli9163c::writeBus(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
  SPCR = _spiSettings;         // SPI Control Register
  SPDR = data;                 // SPI Data Register
  while(!(SPSR & _BV(SPIF)));  // SPI Status Register Wait for transmission to finish
#elif defined (DM_TOOLCHAIN_MBED)
  _spi->write(data);
#endif
}

void DmTftIli9163c::sendCommand(uint8_t index) {
  cbi(_pinDC, _bitmaskDC);
  writeBus(index);
}

void DmTftIli9163c::send8BitData(uint8_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data);
}

void DmTftIli9163c::sendData(uint16_t data) {
  uint8_t dh = data>>8;
  uint8_t dl = data&0xff;

  sbi(_pinDC, _bitmaskDC);
  writeBus(dh);
  writeBus(dl);
}

void DmTftIli9163c::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  sendCommand(0x2A); // Set Column
  sendData(x0);
  sendData(x1);

  sendCommand(0x2B);  // Set Page
  sendData(y0);
  sendData(y1);

  sendCommand(0x2C);
}

void DmTftIli9163c::init(void) {
  setTextColor(BLACK, WHITE);
#if defined (DM_TOOLCHAIN_ARDUINO)
  _pinCS  = portOutputRegister(digitalPinToPort(_cs));
  _bitmaskCS  = digitalPinToBitMask(_cs);
  _pinDC  = portOutputRegister(digitalPinToPort(_dc));
  _bitmaskDC  = digitalPinToBitMask(_dc);
  _pinRST= portOutputRegister(digitalPinToPort(_rst));
  _bitmaskRST= digitalPinToBitMask(_rst);	
  pinMode(_cs,OUTPUT);
  pinMode(_dc,OUTPUT);
	pinMode(_rst,OUTPUT);

  sbi(_pinCS, _bitmaskCS);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  _spiSettings = SPCR;
#elif defined (DM_TOOLCHAIN_MBED)
  _pinCS = new DigitalOut((PinName)_cs);
  _pinDC = new DigitalOut((PinName)_dc);
	_pinRST= new DigitalOut((PinName)_rst)
  sbi(_pinCS, _bitmaskCS);

  _spi = new SPI((PinName)_mosi, (PinName)_miso, (PinName)_clk);
  _spi->format(8,0);
  _spi->frequency(8000000); // Max SPI speed for display is 10 and for 17 for LPC15xx
#endif
  cbi(_pinCS, _bitmaskCS);
  delay(135); // This much delay needed

  // Ili9163 init
  sbi(_pinRST, _bitmaskRST);
  delay(1);
  cbi(_pinRST, _bitmaskRST);
  delay(10);
  sbi(_pinRST, _bitmaskRST);
  delay(120);

  sendCommand(0x26);  // Set default gamma
  send8BitData(0x04);
  sendCommand(0xB1);  // Set Frame Rate
  send8BitData(0X0C);
  send8BitData(0X14);	

  sendCommand(0xC0);
  send8BitData(0x0C);
  send8BitData(0X05);

  sendCommand(0xC1);
  send8BitData(0x02);

  sendCommand(0xC5);   // VCOM Setting
  send8BitData(0x29);  
  send8BitData(0x50);
  sendCommand(0xC7);
  send8BitData(0x40);	
	
  sendCommand(0x36);	//Set Scanning Direction
  send8BitData(0XC8);

  sendCommand(0xB7);  //Set Source Output Direction
  send8BitData(0x00);
  sendCommand(0x3A);  //Set Color Format  
  send8BitData(0x05);
  sendCommand(0xF2);  
  send8BitData(0x01);

  sendCommand(0xE0);   // gamma setting
  send8BitData(0x36);
  send8BitData(0x29);
  send8BitData(0x12);
  send8BitData(0x22);
  send8BitData(0x1C);
  send8BitData(0x15);
  send8BitData(0x42);
  send8BitData(0xB7);
  send8BitData(0x2F);
  send8BitData(0x13);
  send8BitData(0x12);
  send8BitData(0x0A);
  send8BitData(0x11);
  send8BitData(0x0B);
  send8BitData(0x06);

  sendCommand(0xE1);
  send8BitData(0x09);
  send8BitData(0x16);
  send8BitData(0x2D);
  send8BitData(0x0D);
  send8BitData(0x13);
  send8BitData(0x15);
  send8BitData(0x40);
  send8BitData(0x48);
  send8BitData(0x53);
  send8BitData(0x0C);
  send8BitData(0x1D);
  send8BitData(0x25);
  send8BitData(0x2E);
  send8BitData(0x34);
  send8BitData(0x39);

  sendCommand(0x3A);
  send8BitData(0x05); 

  sendCommand(0x11);    //Exit Sleep
  delay(120);
  sendCommand(0x29);    //Display on
  sendCommand(0x2C);
  delay(50);
  sbi(_pinCS, _bitmaskCS);
  clearScreen();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

