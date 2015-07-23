/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmTftSt7789.h"
#if defined (DM_TOOLCHAIN_ARDUINO)
DmTftSt7789::DmTftSt7789(uint8_t cs, uint8_t dc, uint8_t rst)
#elif defined (DM_TOOLCHAIN_MBED)
DmTftSt7789::DmTftSt7789(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t miso, uint8_t mosi, uint8_t clk)
#endif
: DmTftBase(240, 320){
  _cs = cs;
  _dc = dc;
	_rst = rst;
#if defined (DM_TOOLCHAIN_MBED)
  _miso = miso;
  _mosi = mosi;
  _clk = clk;
#endif
}

DmTftSt7789::~DmTftSt7789() {
#if defined (DM_TOOLCHAIN_MBED)
delete _pinCS;
delete _pinDC;
delete _spi;

_pinCS = NULL;
_pinDC = NULL;
_spi = NULL;
#endif
}

void DmTftSt7789::writeBus(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
  SPCR = _spiSettings;         // SPI Control Register
  SPDR = data;                 // SPI Data Register
  while(!(SPSR & _BV(SPIF)));  // SPI Status Register Wait for transmission to finish
#elif defined (DM_TOOLCHAIN_MBED)
  _spi->write(data);
#endif
}

void DmTftSt7789::sendCommand(uint8_t index) {
  cbi(_pinDC, _bitmaskDC);
  writeBus(index);
}

void DmTftSt7789::send8BitData(uint8_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data);
}

void DmTftSt7789::sendData(uint16_t data) {
  uint8_t dh = data>>8;
  uint8_t dl = data&0xff;

  sbi(_pinDC, _bitmaskDC);
  writeBus(dh);
  writeBus(dl);
}

void DmTftSt7789::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  sendCommand(0x2A); // Set Column
  sendData(x0);
  sendData(x1);

  sendCommand(0x2B);  // Set Page
  sendData(y0);
  sendData(y1);

  sendCommand(0x2c);
}

void DmTftSt7789::init(void) {
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
  SPI.setDataMode(SPI_MODE2);
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

  // ST7789 init
  sbi(_pinRST, _bitmaskRST);
  delay(1);
  cbi(_pinRST, _bitmaskRST);
  delay(10);
  sbi(_pinRST, _bitmaskRST);
  delay(120);

  sendCommand(0xB2);
  send8BitData(0x0C);
  send8BitData(0x0C);
  send8BitData(0X00);
  send8BitData(0X33);
  send8BitData(0X33);

  sendCommand(0xB7);
  send8BitData(0x35);

  sendCommand(0x3A);
  send8BitData(0x05);

  sendCommand(0xBB);   // VCOM Setting
  send8BitData(0x30);  
  sendCommand(0xC3);
  send8BitData(0x1C);	
  sendCommand(0xC4);	
  send8BitData(0X18);

  sendCommand(0xC6);  // Frame rate control in normal mode
  send8BitData(0x0F);
  sendCommand(0xD0);  // Power control
  send8BitData(0xA4);
  send8BitData(0xA2);

  sendCommand(0xE0);   // gamma setting
  send8BitData(0xF0);
  send8BitData(0x00);
  send8BitData(0x0A);
  send8BitData(0x10);
  send8BitData(0x12);
  send8BitData(0x1B);
  send8BitData(0x39);
  send8BitData(0x44);
  send8BitData(0x47);
  send8BitData(0x28);
  send8BitData(0x12);
  send8BitData(0x10);
  send8BitData(0x16);
  send8BitData(0x1B);

  sendCommand(0xE1);
  send8BitData(0xF0);
  send8BitData(0x00);
  send8BitData(0x0A);
  send8BitData(0x10);
  send8BitData(0x11);
  send8BitData(0x1A);
  send8BitData(0x3B);
  send8BitData(0x34);
  send8BitData(0x4E);
  send8BitData(0x3A);
  send8BitData(0x17);
  send8BitData(0x16);
  send8BitData(0x21);
  send8BitData(0x22);

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

