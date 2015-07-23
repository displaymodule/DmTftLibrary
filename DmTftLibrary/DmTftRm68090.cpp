/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
#include "DmTftRm68090.h"

DmTftRm68090::DmTftRm68090(uint8_t wr, uint8_t cs, uint8_t dc, uint8_t rst) : DmTftBase(240, 320) {
  _wr = wr;
  _cs = cs;
  _dc = dc;
  _rst = rst;
}

DmTftRm68090::~DmTftRm68090() {
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

void DmTftRm68090::writeBus(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
  PORTD = data;
#elif defined (DM_TOOLCHAIN_MBED)
  *_virtualPortD = data;
#endif
  pulse_low(_pinWR, _bitmaskWR);
}

void DmTftRm68090::sendCommand(uint8_t index) {
  cbi(_pinDC, _bitmaskDC);
  writeBus(0x00);
  writeBus(index);
}

void DmTftRm68090::send8BitData(uint8_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data);
}

void DmTftRm68090::sendData(uint16_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data>>8);
  writeBus(data);
}

void DmTftRm68090::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	sendCommand(0x50); 
	sendData(x0);
	sendCommand(0x51); 
	sendData(x1);

	sendCommand(0x52); 
	sendData(y0);
	sendCommand(0x53); 
	sendData(y1);	
	
	sendCommand(0x20);	
	sendData(x0);
	sendCommand(0x21);	
	sendData(y0);
	
	sendCommand(0x22);
}

void DmTftRm68090::init(void) {
  setTextColor(BLACK, WHITE);
#if defined (DM_TOOLCHAIN_ARDUINO)
/**************************************
      DM-TFT24-316       Arduino UNO      NUM

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


  sendCommand(0x01); 
  sendData(0x0100); // Set SS and SM bit
  sendCommand(0x02);
  sendData(0x0700); // Set line inversion
  sendCommand(0x03);
  sendData(0x1030); // Set Write direction
  sendCommand(0x04);
  sendData(0x0000); // Set Scaling function off
  sendCommand(0x08); 
  sendData(0x0207); // Set BP and FP
  sendCommand(0x09); 
  sendData(0x0000); // Set non-display area
  sendCommand(0x0A); 
  sendData(0x0000); // Frame marker control
  sendCommand(0x0C); 
  sendData(0x0000); // Set interface control
  sendCommand(0x0D); 
  sendData(0x0000); // Frame marker Position
  sendCommand(0x0F); 
  sendData(0x0000); // Set RGB interface 
  //--------------- Power On Sequence----------------//
  sendCommand(0x10); 
  sendData(0x0000); // Set SAP);BT[3:0]);AP);SLP);STB
  sendCommand(0x11); 
  sendData(0x0007); // Set DC1[2:0]);DC0[2:0]);VC
  sendCommand(0x12); 
  sendData(0x0000); // Set VREG1OUT voltage
  sendCommand(0x13); 
  sendData(0x0000); // Set VCOM AMP voltage
  sendCommand(0x07); 
  sendData(0x0001); // Set VCOM AMP voltage
  sendCommand(0x07); 
  sendData(0x0020); // Set VCOM AMP voltage
  delay(200);
  sendCommand(0x10); 
  sendData(0x1290); // Set SAP);BT[3:0]);AP);SLP);STB
  sendCommand(0x11); 
  sendData(0x0221); // Set DC1[2:0]);DC0[2:0]);VC[2:0]
  delay(50);
  sendCommand(0x12); 
  sendData(0x0081); // Set VREG1OUT voltaged
  delay(50);
  sendCommand(0x13); 
  sendData(0x1500); // Set VCOM AMP voltage
  sendCommand(0x29); 
  sendData(0x000c); // Set VCOMH voltage
  sendCommand(0x2B); 
  sendData(0x000D); // Set Frame rate.
  delay(50);
  sendCommand(0x20); 
  sendData(0x0000); // Set GRAM Horizontal Address
  sendCommand(0x21); 
  sendData(0x0000); // Set GRAM Vertical Address
  //****************************************************
  sendCommand(0x30); 
  sendData(0x0303);
  sendCommand(0x31); 
  sendData(0x0006);
  sendCommand(0x32); 
  sendData(0x0001);
  sendCommand(0x35); 
  sendData(0x0204);
  sendCommand(0x36); 
  sendData(0x0004);
  sendCommand(0x37); 
  sendData(0x0407);
  sendCommand(0x38); 
  sendData(0x0000);
  sendCommand(0x39); 
  sendData(0x0404);
  sendCommand(0x3C); 
  sendData(0x0402);
  sendCommand(0x3D); 
  sendData(0x0004);
  //---------------  RAM Address Control ----------------//
  sendCommand(0x50); 
  sendData(0x0000); // Set GRAM Horizontal Start Address
  sendCommand(0x51); 
  sendData(0x00EF); // Set GRAM Horizontal End Address
  sendCommand(0x52); 
  sendData(0x0000); // Set GRAM Vertical Start Address
  sendCommand(0x53); 
  sendData(0x013F); // Set GRAM Vertical End Address
  //---------------  Panel Image Control -----------------//
  sendCommand(0x60); 
  sendData(0x2700); // Set Gate Scan line
  sendCommand(0x61); 
  sendData(0x0001); // Set NDL); VLE); REV
  sendCommand(0x6A); 
  sendData(0x0000); // Set Scrolling line
  //---------------  Panel Interface Control---------------//
  sendCommand(0x90); 
  sendData(0x0010);
  sendCommand(0x92); 
  sendData(0x0000);
  //--------------- Display On-------------------------------//
  sendCommand(0x07); 
  sendData(0x0133); // Display on
  sendCommand(0x22); 
  delay(50);
  sbi(_pinCS, _bitmaskCS);
  delay(100);
  clearScreen();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
