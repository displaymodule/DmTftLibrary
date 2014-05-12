/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
// Tested with Xpt2046

#include "DmTouch.h"

#if defined(__AVR__)
// disp        - which display is used
// cs          - pin number CS (Chip Select, SS)
// irq         - pin number IRQ (set to -1 if you don't want to use)
// hardwareSpi - set to 1 if you want to use hardware Spi, 0 otherwise
//               (note, if you use hardware Spi on arduino, clk, mosi and miso will be disregarded)
// clk         - pin number CLK (SLK)
// mosi        - pin number MOSI
// miso        - pin number MISO
// width       - width of display (default 240)
// height      - height of display (default 320)
DmTouch::DmTouch(Display disp, int8_t cs, int8_t irq, int8_t hardwareSpi, int8_t clk, int8_t mosi, int8_t miso, uint16_t width, uint16_t height) : _width(width), _height(height)
{
  _hardwareSpi = hardwareSpi;
// disp        - which display is used
// cs          - pin number CS (Chip Select, SS)
// irq         - pin number IRQ (set to -1 if you don't want to use)
// clk         - pin number CLK (SLK)
// mosi        - pin number MOSI
// miso        - pin number MISO
#elif defined (TOOLCHAIN_ARM_MICRO)
DmTouch::DmTouch(Display disp, int8_t cs, int8_t irq, int8_t clk, int8_t mosi, int8_t miso)
{
  // Display geometry is fixed for the three displays that are in use now
  _width = 240;
  _height = 320;
#endif

  _cs = cs;
  _irq = irq;
  _clk = clk;
  _mosi = mosi;
  _miso = miso;

  // Calibration data for the different displays
  switch (disp) {
    case DmTouch::DM_TFT28_103:
      _calibLowX = 220;
      _calibLowY = 220;
      _calibModifierX = 15.25;
      _calibModifierY = 11.16;
      _calibInvertedTouch = false;
      break;

    case DmTouch::DM_TFT24_104:
      _calibLowX = 210;
      _calibLowY = 280;
      _calibModifierX = 15;
      _calibModifierY = 11.24;
      _calibInvertedTouch = true;
      break;

	case DmTouch::DM_TFT35_107:
      _calibLowX = 912;
      _calibLowY = 422 ;
      _calibModifierX = 3.25;
      _calibModifierY = 1.175;
      _calibInvertedTouch = false;
	  break;
	  
    case DmTouch::DM_TFT28_105:
    default:
      _calibLowX = 260;
      _calibLowY = 220;
      _calibModifierX = 14.42;
      _calibModifierY = 10.78;
      _calibInvertedTouch = false;
      break;
  }
}

void DmTouch::begin() {
#if defined (__AVR__)
  pinMode(_cs, OUTPUT);
  _pinCS  = portOutputRegister(digitalPinToPort(_cs));
  _bitmaskCS  = digitalPinToBitMask(_cs);

  if (_hardwareSpi) {
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    _spiSettings = SPCR;
  }
  else {
    pinMode(_clk, OUTPUT);
    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
    _pinCLK  = portOutputRegister(digitalPinToPort(_clk));
    _bitmaskCLK  = digitalPinToBitMask(_clk);
    _pinMOSI  = portOutputRegister(digitalPinToPort(_mosi));
    _bitmaskMOSI  = digitalPinToBitMask(_mosi);
    _pinMISO = portInputRegister(digitalPinToPort(_miso));
    _bitmaskMISO  = digitalPinToBitMask(_miso);
  }
#elif defined (TOOLCHAIN_ARM_MICRO)
  _pinCS = new DigitalOut((PinName)_cs);
  sbi(_pinCS, _bitmaskCS);
  _spi = new SPI((PinName)_mosi, (PinName)_miso, (PinName)_clk);
  _spi->format(8,0);
  _spi->frequency(8000000); // Max SPI speed
  cbi(_pinCS, _bitmaskCS);
#endif

  if (_irq != -1) { // We will use Touch IRQ
    enableIrq();
  }
}

void DmTouch::enableIrq(){
#if defined (__AVR__)
  pinMode(_irq, INPUT);
  _pinIrq = portInputRegister(digitalPinToPort(_irq));
  _bitmaskIrq  = digitalPinToBitMask(_irq);
#elif defined (TOOLCHAIN_ARM_MICRO)
  _pinIrq = new DigitalIn((PinName)_irq);
  _pinIrq->mode(PullUp);
#endif

  cbi(_pinCS, _bitmaskCS);
  spiWrite(0x80); // Enable PENIRQ
  sbi(_pinCS, _bitmaskCS);
}

void DmTouch::spiWrite(uint8_t data) {
#if defined (__AVR__)
  if (_hardwareSpi) {
    SPCR = _spiSettings;   // SPI Control Register
    SPDR = data;        // SPI Data Register
    while(!(SPSR & _BV(SPIF)));  // SPI Status Register Wait for transmission to finish
  }
  else {
    uint8_t count=0;
    uint8_t temp;
    unsigned nop;
    temp=data; // really needed?
    cbi(_pinCLK, _bitmaskCLK);
    for(count=0;count<8;count++) {
      if(temp&0x80) {
        sbi(_pinMOSI, _bitmaskMOSI);
      }
      else {
        cbi(_pinMOSI, _bitmaskMOSI);
      }

      temp=temp<<1;

      pulse_low(_pinCLK, _bitmaskCLK);
    }
  }
#elif defined (TOOLCHAIN_ARM_MICRO)
  _spi->write(data);
#endif
}

uint8_t DmTouch::spiRead() {// Only used for Hardware SPI
#if defined (__AVR__)
  uint8_t data;
  SPCR = _spiSettings;
  spiWrite(0x00);
  data = SPDR;

  return data;
#elif defined (TOOLCHAIN_ARM_MICRO)
  return _spi->write(0x00); // dummy byte to read
#endif
}

uint16_t DmTouch::readData12(uint8_t command) {
  uint8_t temp = 0;
  uint16_t value = 0;

  spiWrite(command); // Send command
//--------------
#if defined (__AVR__)
  if (_hardwareSpi) {
    // We use 7-bits from the first byte and 5-bit from the second byte
    temp = spiRead();
    value = temp<<8;
    temp = spiRead();
    value |= temp;
    value >>=3;
    value &= 0xFFF;
  }
  else {
    pulse_high(_pinCLK, _bitmaskCLK);
    unsigned nop;
    uint8_t count=0;
    for(count=0;count<12;count++) {
      value<<=1;
      pulse_high(_pinCLK, _bitmaskCLK);
      if ( gbi(_pinMISO, _bitmaskMISO) ) {
        value++;
      }
    }
  }
#elif defined (TOOLCHAIN_ARM_MICRO)
  // We use 7-bits from the first byte and 5-bit from the second byte
  temp = spiRead();
  value = temp<<8;
  temp = spiRead();
  value |= temp;
  value >>=3;
  value &= 0xFFF;
#endif
  return value;
}

void DmTouch::readTouchData(uint16_t& posX, uint16_t& posY, bool& touching) {
#if defined (TOOLCHAIN_ARM_MICRO)
  if (!isTouched()) {
    touching = false;
    return;
  }
  touching = true;
#endif
  unsigned int TP_X, TP_Y;
  cbi(_pinCS, _bitmaskCS);

  TP_X = readData12(0xD0);
  TP_Y = readData12(0x90);

  sbi(_pinCS, _bitmaskCS);

 Serial.print("Raw X: ");
 Serial.println(TP_X);
 Serial.print("Raw Y: ");
 Serial.println(TP_Y);

  // Convert raw data to screen positions
  if (_calibInvertedTouch) {
    posX=_width-((TP_X-_calibLowX)/_calibModifierX);
    posY=_height-((TP_Y-_calibLowY)/_calibModifierY);
  } else {
    posX=((TP_X-_calibLowX)/_calibModifierX);
    posY=((TP_Y-_calibLowY)/_calibModifierY);
  }

#if defined (__AVR__)
  if (posX >= 0 && posX <= _width && posY >= 0 && posY <= _height) {
    touching = true;
  } else {
    touching = false;
  }
#endif
}

uint8_t DmTouch::isTouched() {
#if defined (__AVR__)
  if (_irq == -1) {
    uint16_t posX, posY;
    bool touched;
    readTouchData(posX, posY, touched);
    return touched;
  }

  if ( !gbi(_pinIrq, _bitmaskIrq) ) {
    Serial.println("IsTouched");
    return true;
  }

  return false;
#elif defined (TOOLCHAIN_ARM_MICRO)
  return (*_pinIrq == 0);
#endif
}
