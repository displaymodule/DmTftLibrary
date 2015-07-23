/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
#include "DmTftIli9341v.h"

DmTftIli9341v::DmTftIli9341v(uint8_t wr, uint8_t cs, uint8_t dc, uint8_t rst) : DmTftBase(240, 320) {
  _wr = wr;
  _cs = cs;
  _dc = dc;
  _rst = rst;
}

DmTftIli9341v::~DmTftIli9341v() {
#if defined (DM_TOOLCHAIN_MBED)
  delete _pinRST;
  delete _pinCS;
  delete _pinWR;
  delete _pinDC;
  delete _virtualPortD;
  _pinRST = NULL;
  _pinCS = NULL;
  _pinWR = NULL;
  _pinDC = NULL;
  _virtualPortD = NULL;
#endif
}

void DmTftIli9341v::writeBus(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
  PORTD = data;
#elif defined (DM_TOOLCHAIN_MBED)
  *_virtualPortD = data;
#endif
  pulse_low(_pinWR, _bitmaskWR);
}

void DmTftIli9341v::sendCommand(uint8_t index) {
  cbi(_pinDC, _bitmaskDC);
  writeBus(index);
}

void DmTftIli9341v::send8BitData(uint8_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data);
}

void DmTftIli9341v::sendData(uint16_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data>>8);
  writeBus(data);
}

void DmTftIli9341v::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	sendCommand(0x2A); // Set Column
	sendData(x0);
	sendData(x1);
	
	sendCommand(0x2B);	// Set Page
	sendData(y0);
	sendData(y1);
	
	sendCommand(0x2c);
}

void DmTftIli9341v::init(void) {
  setTextColor(BLACK, WHITE);
#if defined (DM_TOOLCHAIN_ARDUINO)
/**************************************
      DM-TFT24-314       Arduino UNO      NUM

      RST                       A2                     16
      CS                         A3                     17
      WR                        A4                     18
      RS                         A5                     19

      DB8                       0                       0
      DB9                       1                       1
      DB10                     2                       2
      DB11                     3                       3
      DB12                     4                       4
      DB13                     5                       5
      DB14                     6                       6
      DB15                     7                       7

***************************************/
  DDRD = DDRD | B11111111;  // SET PORT D AS OUTPUT

  _pinRST = portOutputRegister(digitalPinToPort(_rst));
  _bitmaskRST = digitalPinToBitMask(_rst);
  _pinCS = portOutputRegister(digitalPinToPort(_cs));
  _bitmaskCS = digitalPinToBitMask(_cs);
  _pinWR = portOutputRegister(digitalPinToPort(_wr));
  _bitmaskWR = digitalPinToBitMask(_wr);
  _pinDC = portOutputRegister(digitalPinToPort(_dc));
  _bitmaskDC = digitalPinToBitMask(_dc);

  pinMode(_rst, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_dc,OUTPUT);
#elif defined (DM_TOOLCHAIN_MBED)
  _pinRST = new DigitalOut((PinName)_rst);
  _pinCS = new DigitalOut((PinName)_cs);
  _pinWR = new DigitalOut((PinName)_wr);
  _pinDC = new DigitalOut((PinName)_dc);
  #ifdef LPC15XX_H
    _virtualPortD = new BusOut(D0, D1, D2, D3, D4, P0_11, D6, D7);
  #else
    _virtualPortD = new BusOut(D0, D1, D2, D3, D4, D5, D6, D7);
  #endif
#endif

  sbi(_pinRST, _bitmaskRST);
  delay(5);
  cbi(_pinRST, _bitmaskRST);
  delay(15);
  sbi(_pinRST, _bitmaskRST);
  sbi(_pinCS, _bitmaskCS);
  sbi(_pinWR, _bitmaskWR);
  delay(120);
  cbi(_pinCS, _bitmaskCS);

  sendCommand(0xCF);
  send8BitData(0x00);
  send8BitData(0xC1);
  send8BitData(0X30);

  sendCommand(0xED);
  send8BitData(0x64);
  send8BitData(0x03);
  send8BitData(0X12);
  send8BitData(0X81);

  sendCommand(0xE8);
  send8BitData(0x85);
  send8BitData(0x00);
  send8BitData(0x78);

  sendCommand(0xCB);
  send8BitData(0x39);
  send8BitData(0x2C);
  send8BitData(0x00);
  send8BitData(0x34);
  send8BitData(0x02);

  sendCommand(0xF7);
  send8BitData(0x20);

  sendCommand(0xEA);
  send8BitData(0x00);
  send8BitData(0x00);

  sendCommand(0xC0);    //Power control
  send8BitData(0x21);   //VRH[5:0]

  sendCommand(0xC1);    //Power control
  send8BitData(0x11);   //SAP[2:0];BT[3:0]

  sendCommand(0xC5);    //VCM control
  send8BitData(0x31);
  send8BitData(0x3F);

  sendCommand(0xC7);    //VCM control2
  send8BitData(0x93);   //0xB0

  sendCommand(0x36);    // Memory Access Control
  send8BitData(0x08);

  sendCommand(0x3A);
  send8BitData(0x55);

  sendCommand(0xB1);
  send8BitData(0x00);
  send8BitData(0x17);

  sendCommand(0xB6);    // Display Function Control
  send8BitData(0x0A);
  send8BitData(0xA2);

  sendCommand(0xF2);    // 3Gamma Function Disable
  send8BitData(0x00);

  sendCommand(0x26);    //Gamma curve selected
  send8BitData(0x01);

  sendCommand(0xE0);    //Set Gamma
  send8BitData(0x0F);
  send8BitData(0x1F);
  send8BitData(0x1D);
  send8BitData(0x09);
  send8BitData(0x0B);
  send8BitData(0x04);
  send8BitData(0x4E);
  send8BitData(0X92);
  send8BitData(0x40);
  send8BitData(0x0A);
  send8BitData(0x15);
  send8BitData(0x07);
  send8BitData(0x14);
  send8BitData(0x06);
  send8BitData(0x0F);

  sendCommand(0XE1);    //Set Gamma
  send8BitData(0x00);
  send8BitData(0x1C);
  send8BitData(0x1F);
  send8BitData(0x03);
  send8BitData(0x0F);
  send8BitData(0x05);
  send8BitData(0x37);
  send8BitData(0x24);
  send8BitData(0x4C);
  send8BitData(0x04);
  send8BitData(0x0E);
  send8BitData(0x0C);
  send8BitData(0x30);
  send8BitData(0x34);
  send8BitData(0x0F);

  sendCommand(0x11);    //Exit Sleep
  delay(120);
  sendCommand(0x29);    //Display on
  delay(50);
  sbi(_pinCS, _bitmaskCS);
  delay(500);
  clearScreen();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
