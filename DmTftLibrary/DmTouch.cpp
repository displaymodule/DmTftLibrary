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

#if defined(DM_TOOLCHAIN_ARDUINO)
// disp        - which display is used
// cs          - pin number CS (Chip Select, SS)
// tIrq        - pin number IRQ (set to -1 if you don't want to use)
// hardwareSpi - set to 1 if you want to use hardware Spi, 0 otherwise
//               (note, if you use hardware Spi on arduino, clk, mosi and miso will be disregarded)
// clk         - pin number CLK (SLK)
// mosi        - pin number MOSI
// miso        - pin number MISO
// width       - width of display (default 240)
// height      - height of display (default 320)
DmTouch::DmTouch(Display disp, uint8_t cs, int8_t tIrq, int8_t hardwareSpi, uint8_t clk, uint8_t mosi, uint8_t miso, uint16_t width, uint16_t height) : _width(width), _height(height)
{
  _hardwareSpi = hardwareSpi;
  _cs = cs;
  _irq = tIrq;
  _clk = clk;
  _mosi = mosi;
  _miso = miso;

#elif defined (DM_TOOLCHAIN_MBED)
// disp        - which display is used
DmTouch::DmTouch(Display disp, bool hardwareSPI)
{
  _hardwareSpi = hardwareSPI;
  switch (disp) {
    // Display with 40-pin connector on top of adapter board
    case DmTouch::DM_TFT28_103:
    case DmTouch::DM_TFT24_104:
      _cs = D8;
      _irq = D10;
      _clk = A1;
      _mosi = A0;
      _miso = D9;
      _width = 240;
      _height = 320;
      break;
    
    case DmTouch::DM_TFT28_105:
      _cs = D4;
      _irq = D2;
      _clk = D13;
      _mosi = D11;
      _miso = D12;
      _width = 240;
      _height = 320;
      break;

    case DmTouch::DM_TFT35_107:
    default:
      _cs = D4;
      _irq = D2;
      _clk = D13;
      _mosi = D11;
      _miso = D12;
      _width = 320;
      _height = 240;
      break;
  }
#endif

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

void DmTouch::init() {
#if defined (DM_TOOLCHAIN_ARDUINO)
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
#elif defined (DM_TOOLCHAIN_MBED)
  _pinCS = new DigitalOut((PinName)_cs);
  if (_hardwareSpi) {
    sbi(_pinCS, _bitmaskCS);
    _spi = new SPI((PinName)_mosi, (PinName)_miso, (PinName)_clk);
    _spi->format(8,0);
    _spi->frequency(2000000); // Max SPI speed
    //cbi(_pinCS, _bitmaskCS);
  } else {
    _pinCLK = new DigitalOut((PinName)_clk);
    _pinMISO = new DigitalOut((PinName)_miso);
    _pinMOSI = new DigitalOut((PinName)_mosi);
  }
#endif

  if (_irq != -1) { // We will use Touch IRQ
    enableIrq();
  }
}

void DmTouch::enableIrq() {
#if defined (DM_TOOLCHAIN_ARDUINO)
  pinMode(_irq, INPUT);
  _pinIrq = portInputRegister(digitalPinToPort(_irq));
  _bitmaskIrq  = digitalPinToBitMask(_irq);
#elif defined (DM_TOOLCHAIN_MBED)
  _pinIrq = new DigitalIn((PinName)_irq);
  _pinIrq->mode(PullUp);
#endif

  cbi(_pinCS, _bitmaskCS);
  spiWrite(0x80); // Enable PENIRQ
  sbi(_pinCS, _bitmaskCS);
}

void DmTouch::spiWrite(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
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
#elif defined (DM_TOOLCHAIN_MBED)
  if (_hardwareSpi) {
    _spi->write(data);
  }
  else {
    uint8_t count=0;
    uint8_t temp = data;
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
#endif
}

uint8_t DmTouch::spiRead() {// Only used for Hardware SPI
#if defined (DM_TOOLCHAIN_ARDUINO)
  uint8_t data;
  SPCR = _spiSettings;
  spiWrite(0x00);
  data = SPDR;

  return data;
#elif defined (DM_TOOLCHAIN_MBED)
  if (_hardwareSpi) {
    return _spi->write(0x00); // dummy byte to read
  } else {
    uint8_t count=0;
    uint8_t temp=0;
    cbi(_pinCLK, _bitmaskCLK);
    cbi(_pinMOSI, _bitmaskMOSI);
    for(count=0;count<8;count++) {

      pulse_low(_pinCLK, _bitmaskCLK);
      temp = temp<<1;
      temp |= _pinMISO->read();
    }
    return temp;
  }
#endif
}

uint16_t DmTouch::readData12(uint8_t command) {
  uint8_t temp = 0;
  uint16_t value = 0;

  spiWrite(command); // Send command
//--------------
#if defined (DM_TOOLCHAIN_ARDUINO)
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
#elif defined (DM_TOOLCHAIN_MBED)
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
#if defined (DM_TOOLCHAIN_MBED)
  if (!isTouched()) {
    touching = false;
    return;
  }
  //touching = true;
#endif
  unsigned int TP_X, TP_Y;
  cbi(_pinCS, _bitmaskCS);

  TP_X = readData12(0xD0);
  TP_Y = readData12(0x90);

  sbi(_pinCS, _bitmaskCS);

//  Serial.print("Raw X: ");
//  Serial.println(TP_X);
//  Serial.print("Raw Y: ");
//  Serial.println(TP_Y);

  // Convert raw data to screen positions
  if (_calibInvertedTouch) {
    posX=_width-((TP_X-_calibLowX)/_calibModifierX);
    posY=_height-((TP_Y-_calibLowY)/_calibModifierY);
  } else {
    posX=((TP_X-_calibLowX)/_calibModifierX);
    posY=((TP_Y-_calibLowY)/_calibModifierY);
  }

//#if defined (DM_TOOLCHAIN_ARDUINO)
  if (posX >= 0 && posX <= _width && posY >= 0 && posY <= _height) {
    touching = true;
  } else {
    touching = false;
  }
//#endif
}

uint8_t DmTouch::isTouched() {
#if defined (DM_TOOLCHAIN_ARDUINO)
  if (_irq == -1) {
    uint16_t posX, posY;
    bool touched;
    readTouchData(posX, posY, touched);
    return touched;
  }

  if ( !gbi(_pinIrq, _bitmaskIrq) ) {
    return true;
  }

  return false;
#elif defined (DM_TOOLCHAIN_MBED)
  return (*_pinIrq == 0);
#endif
}


