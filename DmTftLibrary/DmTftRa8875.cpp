/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmTftRa8875.h"
#if defined (DM_TOOLCHAIN_ARDUINO)
DmTftRa8875::DmTftRa8875(uint8_t cs, uint8_t sel)
#elif defined (DM_TOOLCHAIN_MBED)
DmTftRa8875::DmTftRa8875(uint8_t cs, uint8_t sel, uint8_t miso, uint8_t mosi, uint8_t clk)
#endif
: DmTftBase(480, 272)
{
  _cs = cs;
	_sel = sel;
#if defined (DM_TOOLCHAIN_MBED)
  _miso = miso;
  _mosi = mosi;
  _clk = clk;
#endif
}

DmTftRa8875::~DmTftRa8875() {
#if defined (DM_TOOLCHAIN_MBED)
delete _pinCS;
delete _pinSEL;
delete _spi;

_pinCS = NULL;
_pinSEL = NULL;
_spi = NULL;
#endif
}

uint16_t DmTftRa8875::width(void) { return _width; }

uint16_t DmTftRa8875::height(void) { return _height; }

void DmTftRa8875::writeBus(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
  //SPCR = _spiSettings;         // SPI Control Register
  SPDR = data;                 // SPI Data Register
  while(!(SPSR & _BV(SPIF)));  // SPI Status Register Wait for transmission to finish
#elif defined (DM_TOOLCHAIN_MBED)
  _spi->write(data);
#endif
}

uint8_t DmTftRa8875::readBus(void)
{
#if defined (DM_TOOLCHAIN_ARDUINO)

	//SPCR = _spiSettings;		 // SPI Control Register
	SPDR = 0;				 // SPI Data Register
	while(!(SPSR & _BV(SPIF)));  // SPI Status Register Wait for transmission to finish
  return SPDR;
#elif defined (DM_TOOLCHAIN_MBED)	
  return _spi->write(0x00); // dummy byte to read
#endif
}

void DmTftRa8875::sendCommand(uint8_t index) {
  cbi(_pinCS, _bitmaskCS);
  
  writeBus(0x80);
  writeBus(index);
  
  sbi(_pinCS, _bitmaskCS);
}

uint8_t DmTftRa8875::readStatus(void)
{
	cbi(_pinCS, _bitmaskCS);
	
	writeBus(0xC0);
	uint8_t data = readBus();	
	
	sbi(_pinCS, _bitmaskCS);

	return data;
}

void DmTftRa8875::sendData(uint16_t data) {

  uint8_t dh = data>>8;
  uint8_t dl = data&0xff;

  cbi(_pinCS, _bitmaskCS);

  writeBus(0x00);
  writeBus(dh);
  writeBus(dl);
	
  sbi(_pinCS, _bitmaskCS);
  
}

void DmTftRa8875::send8BitData(uint8_t data) {
  cbi(_pinCS, _bitmaskCS);
  
  writeBus(0x00);
  writeBus(data);
  
  sbi(_pinCS, _bitmaskCS);
}
uint8_t DmTftRa8875::readData(void)
{
	cbi(_pinCS, _bitmaskCS);
	
	writeBus(0x40);
	uint8_t data = readBus();
   
	sbi(_pinCS, _bitmaskCS);
	
	return data;
}

void DmTftRa8875::writeReg(uint8_t reg, uint8_t val)
{
	sendCommand(reg);
	send8BitData(val);
}

uint8_t DmTftRa8875::readReg(uint8_t reg)
{
   sendCommand(reg);
   return readData();
}

void DmTftRa8875::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
 /* Set active window X */
  writeReg(0x30, x0);                                        // horizontal start point
  writeReg(0x31, x0 >> 8);
  writeReg(0x34, (uint16_t)(x1) & 0xFF);            // horizontal end point
  writeReg(0x35, (uint16_t)(x1) >> 8);
	
	/* Set active window Y */
  writeReg(0x32, y0);                                        // vertical start point
  writeReg(0x33, y0 >> 8);  
  writeReg(0x36, (uint16_t)(y1) & 0xFF);           // vertical end point
  writeReg(0x37, (uint16_t)(y1) >> 8);

	writeReg(0x46, x0);
  writeReg(0x47, x0 >> 8);
  writeReg(0x48, y0);
  writeReg(0x49, y0 >> 8); 
	sendCommand(0x02);
}

void DmTftRa8875::init(enum RA8875Size size) {
	
  setTextColor(BLACK, WHITE);
#if defined (DM_TOOLCHAIN_ARDUINO)
  _pinCS  = portOutputRegister(digitalPinToPort(_cs));
  _bitmaskCS  = digitalPinToBitMask(_cs);
  _pinSEL  = portOutputRegister(digitalPinToPort(_sel));
  _bitmaskSEL  = digitalPinToBitMask(_sel);	
  pinMode(_cs,OUTPUT);
  pinMode(_sel,OUTPUT);
	digitalWrite(_sel, LOW);  // w25 control by MCU

  sbi(_pinCS, _bitmaskCS);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  _spiSettings = SPCR;
#elif defined (DM_TOOLCHAIN_MBED)
  _pinCS = new DigitalOut((PinName)_cs);
	_pinSEL = new DigitalOut((PinName)_sel);
  sbi(_pinCS, _bitmaskCS);
	cbi(

  _spi = new SPI((PinName)_mosi, (PinName)_miso, (PinName)_clk);
  _spi->format(8,0);
  _spi->frequency(8000000); // Max SPI speed for display is 10 and for 17 for LPC15xx
#endif

  cbi(_pinCS, _bitmaskCS);
  delay(135); // This much delay needed??

/* Timing values */
	uint8_t pixclk;
	uint8_t hsync_start;
	uint8_t hsync_pw;
	uint8_t hsync_finetune;
	uint8_t hsync_nondisp;
	uint8_t vsync_pw; 
	uint16_t vsync_nondisp;
	uint16_t vsync_start;

	if(size == RA8875_480x272){
		_width = 480;
		_height = 272;
    setWidth(_width);
		setHeight(_height);
		// PLL init 
		// 20MHz*(10+1)/((RA8875_PLLC1_PLLDIV1 +1) * 2^RA8875_PLLC2_DIV4))
		writeReg(0x88, 0x00 + 10);
		delay(1);
		writeReg(0x89, 0x02);
		delay(1);
	
		pixclk					= 0x80 | 0x02;
		hsync_nondisp 	= 10;
		hsync_start 		= 8;
		hsync_pw				= 48;
		hsync_finetune	= 0;
		vsync_nondisp 	= 3;
		vsync_start 		= 8;
		vsync_pw				= 10;
	}

	if(size == RA8875_800x480){		
		_width = 800;
		_height = 480;
    setWidth(_width);
		setHeight(_height);
		// PLL init 
		// 20MHz*(10+1)/((RA8875_PLLC1_PLLDIV1 +1) * 2^RA8875_PLLC2_DIV4))
		writeReg(0x88, 0x00 + 10);
		delay(1);
		writeReg(0x89, 0x02);
		delay(1);
	
		pixclk					= 0x80 | 0x01;
		hsync_nondisp 	= 26;
		hsync_start 		= 32;
		hsync_pw				= 96;
		hsync_finetune	= 0;
		vsync_nondisp 	= 32;
		vsync_start 		= 23;
		vsync_pw				= 2;
	}

	writeReg(0x10, 0x0C | 0x00);

  writeReg(0x04, pixclk);
  delay(1);
  
  /* Horizontal settings registers */
  writeReg(0x14, (_width / 8) - 1);                          // H width: (HDWR + 1) * 8 = 480
  writeReg(0x15, 0x00 + hsync_finetune);
  writeReg(0x16, (hsync_nondisp - hsync_finetune - 2)/8);    // H non-display: HNDR * 8 + HNDFTR + 2 = 10
  writeReg(0x17, hsync_start/8 - 1);                         // Hsync start: (HSTR + 1)*8 
  writeReg(0x18, 0x00 + (hsync_pw/8 - 1));        // HSync pulse width = (HPWR+1) * 8
  
  /* Vertical settings registers */
  writeReg(0x19, (uint16_t)(_height - 1) & 0xFF);
  writeReg(0x1A, (uint16_t)(_height - 1) >> 8);
  writeReg(0x1B, vsync_nondisp-1);                          // V non-display period = VNDR + 1
  writeReg(0x1C, vsync_nondisp >> 8);
  writeReg(0x1D, vsync_start-1);                            // Vsync start position = VSTR + 1
  writeReg(0x1E, vsync_start >> 8);
  writeReg(0x1F, 0x00 + vsync_pw - 1);            // Vsync pulse width = VPWR + 1
  
  /* Set active window X */
  writeReg(0x30, 0);                                        // horizontal start point
  writeReg(0x31, 0);
  writeReg(0x34, (uint16_t)(_width - 1) & 0xFF);            // horizontal end point
  writeReg(0x35, (uint16_t)(_width - 1) >> 8);
  
  /* Set active window Y */
  writeReg(0x32, 0);                                        // vertical start point
  writeReg(0x33, 0);  
  writeReg(0x36, (uint16_t)(_height - 1) & 0xFF);           // vertical end point
  writeReg(0x37, (uint16_t)(_height - 1) >> 8);
  
  /* ToDo: Setup touch panel? */
  
  /* Clear the entire window */
  writeReg(0x8E, 0x80 | 0x00);
  delay(500); 

	// display on
	writeReg(0x01, 0x00 | 0x80);
  // GPIOX on
	writeReg(0xC7, 1);

  sbi(_pinCS, _bitmaskCS);
	
  clearScreen(BLACK);

}

void DmTftRa8875::eableKeyScan(bool on)
{
	if(on){
		writeReg(0xC0, (1 << 7) | (0 << 4 ) | 1 );		 // enable key scan
	}
	else{
		writeReg(0xC0, (0 << 7));
	}		
}

uint8_t DmTftRa8875::getKeyValue(void)
{
	uint8_t data = 0xFF;
	sendCommand(0xC2);
	data = readData();

  // Clear key interrupt status
	writeReg(0xF1,readReg(0xF1) | 0x10);
	delay(1);
	return data;
	
}
 
bool DmTftRa8875::isKeyPress(void)
{
	uint8_t temp;
	temp = readReg(0xF1);
	
	if(temp & 0x10){
		return true;
	}
	else{
		return false; 
	}
}

void DmTftRa8875::backlightOn(bool on)
{
	if(on){
		writeReg(0x8A, (1 << 7) | (10 << 0 ));		 // enable PWM1 
	}
	else{
		writeReg(0x8A, (0 << 7) | (10 << 0 ));
	}	
}

void DmTftRa8875::backlightAdjust(uint8_t value)
{
	writeReg(0x8B, value);
}

void DmTftRa8875::w25CtrlByMCU(void)
{
#if defined (DM_TOOLCHAIN_ARDUINO)
	digitalWrite(_sel, LOW);
#elif defined (DM_TOOLCHAIN_MBED)	

#endif
}

void DmTftRa8875::w25CtrlByRa8875(void)
{
#if defined (DM_TOOLCHAIN_ARDUINO)
	digitalWrite(_sel, HIGH);
#elif defined (DM_TOOLCHAIN_MBED)	

#endif

}

void DmTftRa8875::drawImageContinuous(uint32_t startaddress,uint32_t count,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1)
{
	w25CtrlByRa8875();

	setAddress(x0, y0, x1-1, y1-1);
	
	writeReg(0xE0, 0x00);

	writeReg(0x05, (0 << 7) | (0 << 6) | (1 << 5) | (0 << 3) | (1 << 2) | (3 << 1));
	writeReg(0x06, (0 << 0)); // set serial flash frequency
	
	writeReg(0xB0,startaddress & 0xFF);  //  DMA Source Start Address
	writeReg(0xB1,startaddress >> 8);
	writeReg(0xB2,startaddress >> 16);

	writeReg(0xB4, count & 0xFF);  // DMA Transfer Number
	writeReg(0xB6, (count & 0xFF00) >> 8 );
	writeReg(0xB8, (count & 0xFF0000) >> 16);

	
	writeReg(0xBF, 0x00);  // Continuous mode 
	writeReg(0xBF, readReg(0xBF) | 0x01); //start DMA
	
  /* Wait for the DMA Transfer to finish */
  while (readReg(0xBF) & 0x01);  

	w25CtrlByMCU();
	
}

void DmTftRa8875::drawImageBlock(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t pic_width, uint16_t block_width, uint16_t block_height)
{
	w25CtrlByRa8875();
	
	setAddress(x0, y0, x1-1, y1-1);
	
	writeReg(0xE0, 0x00);

	writeReg(0x05, (0 << 7) | (0 << 6) | (1 << 5) | (0 << 3) | (1 << 2) | (3 << 1));
	writeReg(0x06, (0 << 0)); // set serial flash frequency
	
	writeReg(0xB0,startaddress & 0xFF);  //  DMA Source Start Address
	writeReg(0xB1,startaddress >> 8);
	writeReg(0xB2,startaddress >> 16);

	writeReg(0xB4, block_width & 0xFF);  // DMA block width
	writeReg(0xB5, block_width >> 8 );
	writeReg(0xB6, block_height & 0xFF);  // DMA block height
	writeReg(0xB7, block_height >> 8 );

	writeReg(0xB8, pic_width & 0xFF);  // DMA soruce picture width
	writeReg(0xB9, pic_width >> 8 );
	
	writeReg(0xBF, 0x02);  // block mode 
	writeReg(0xBF, readReg(0xBF) | 0x01); //start DMA
	
  /* Wait for the DMA Transfer to finish */
  while (readReg(0xBF) & 0x01);  

	w25CtrlByMCU();

}

void DmTftRa8875::clearScreen(uint16_t color)
{
	setAddress(0, 0, _width-1, _height-1);
  rectHelper(0, 0, _width-1, _height-1, color, true);
}


void DmTftRa8875::setFontColor(uint16_t background,uint16_t foreground)
{
  /* Set Background Color */
  sendCommand(0x60);
  send8BitData((background & 0xf800) >> 11);
  sendCommand(0x61);
  send8BitData((background & 0x07e0) >> 5);
  sendCommand(0x62);
  send8BitData((background & 0x001f));

	  /* Set Fore Color */
  sendCommand(0x63);
  send8BitData((foreground & 0xf800) >> 11);
  sendCommand(0x64);
  send8BitData((foreground & 0x07e0) >> 5);
  sendCommand(0x65);
  send8BitData((foreground & 0x001f));

}

void DmTftRa8875::setFontZoom(uint8_t Hsize,uint8_t Vsize)
{
   writeReg(0x22, ((Hsize & 0x03) <<2 | (Vsize & 0x03)));
}

void DmTftRa8875::dispAscii(uint16_t x, uint16_t y, const char *p)
{

	writeReg(0x40, (1 << 7)); // enter text mode

	writeReg(0x2A, x);
  writeReg(0x2B, x >> 8);
  writeReg(0x2C, y);
  writeReg(0x2D, y >> 8); 
	
	
	writeReg(0x2F, 0x00);
	writeReg(0x21, (0 << 7) | (0 << 5) | (0 << 1) | (0 << 0));
	sendCommand(0x02);

	while (*p != '\0')
	{
		send8BitData(*p);
		while ((readStatus() & 0x80) == 0x80);	// wait finish
		p++;
	}
	
	writeReg(0x40, (0 << 7));	// enter graphics mode
}

/**************************************************************************/
/*!
      Helper function for higher level rectangle drawing code
*/
/**************************************************************************/
void DmTftRa8875::rectHelper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled)
{
  /* Set X */
  sendCommand(0x91);
  send8BitData(x);
  sendCommand(0x92);
  send8BitData(x >> 8);
  
  /* Set Y */
  sendCommand(0x93);
  send8BitData(y); 
  sendCommand(0x94);	   
  send8BitData(y >> 8);
  
  /* Set X1 */
  sendCommand(0x95);
  send8BitData(w);
  sendCommand(0x96);
  send8BitData((w) >> 8);
  
  /* Set Y1 */
  sendCommand(0x97);
  send8BitData(h); 
  sendCommand(0x98);
  send8BitData((h) >> 8);

  /* Set Color */
  sendCommand(0x63);
  send8BitData((color & 0xf800) >> 11);
  sendCommand(0x64);
  send8BitData((color & 0x07e0) >> 5);
  sendCommand(0x65);
  send8BitData((color & 0x001f));

  /* Draw! */
  sendCommand(0x90);
  if (filled)
  {
    send8BitData(0xB0);
  }
  else
  {
    send8BitData(0x90);
  }
  
  /* Wait for the command to finish */
  while (readReg(0x90) & 0x80);

}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

