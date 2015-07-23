/**********************************************************************************************
 Copyright (c) 2015 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
#include "DmTftIli9163_mcu.h"

DmTftIli9163_mcu::DmTftIli9163_mcu(uint8_t wr, uint8_t cs, uint8_t dc, uint8_t rst) : DmTftBase(128, 160) {
  _wr = wr;
  _cs = cs;
  _dc = dc;
  _rst = rst;
}

DmTftIli9163_mcu::~DmTftIli9163_mcu() {
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

void DmTftIli9163_mcu::writeBus(uint8_t data) {
#if defined (DM_TOOLCHAIN_ARDUINO)
  PORTD = data;
#elif defined (DM_TOOLCHAIN_MBED)
  *_virtualPortD = data;
#endif
  pulse_low(_pinWR, _bitmaskWR);
}

void DmTftIli9163_mcu::sendCommand(uint8_t index) {
  cbi(_pinDC, _bitmaskDC);
  writeBus(index);
}

void DmTftIli9163_mcu::send8BitData(uint8_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data);
}

void DmTftIli9163_mcu::sendData(uint16_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data>>8);
  writeBus(data);
}

void DmTftIli9163_mcu::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	sendCommand(0x2A); // Set Column
	sendData(x0);
	sendData(x1);
	
	sendCommand(0x2B);	// Set Page
	sendData(y0);
	sendData(y1);
	
	sendCommand(0x2C);
}

void DmTftIli9163_mcu::init(void) {
  setTextColor(BLACK, WHITE);
#if defined (DM_TOOLCHAIN_ARDUINO)
/**************************************
      DM-TFT18-308       Arduino UNO      NUM

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

 //	************* Start Initial Sequence **********

   sendCommand(0x26); //Set Default Gamma
   send8BitData(0x04);

   sendCommand(0xb1);//Set Frame Rate
   send8BitData(0x0b);
   send8BitData(0x14);

   sendCommand(0xc0);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
   send8BitData(0x08);
   send8BitData(0x00);

   sendCommand(0xc1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
   send8BitData(0x05);

   sendCommand(0xc5); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
   send8BitData(0x41);
   send8BitData(0x30);

   sendCommand(0xc7);
   send8BitData(0xc1);

   sendCommand(0x36); //Set Scanning Direction
   send8BitData(0xc8);

   sendCommand(0xec);//Set pumping clock frequency 
   send8BitData(0x1b);

   sendCommand(0x3a); //Set Color Format  
   send8BitData(0x55);

   sendCommand(0xf2); //E0h & E1h Enable/Disable
   send8BitData(0x01);

   sendCommand(0xe0); // P GAM
   send8BitData(0x28); //P1
   send8BitData(0x24); //P2
   send8BitData(0x22); //P3
   send8BitData(0x31); //P4
   send8BitData(0x2b); //P5
   send8BitData(0x0e); //P6
   send8BitData(0x53); //P7
   send8BitData(0xa5); //P8
   send8BitData(0x42); //P9
   send8BitData(0x16); //P10
   send8BitData(0x18); //P11
   send8BitData(0x12); //P12
   send8BitData(0x1a); //P13
   send8BitData(0x14); //P14
   send8BitData(0x03); //P15

   sendCommand(0xe1); // N GAM
   send8BitData(0x17); //P1
   send8BitData(0x1b); //P2
   send8BitData(0x1d); //P3
   send8BitData(0x0e); //P4
   send8BitData(0x14); //P5
   send8BitData(0x11); //P6
   send8BitData(0x2c); //P7
   send8BitData(0xa5); //P8
   send8BitData(0x3d); //P9
   send8BitData(0x09); //P10
   send8BitData(0x27); //P11
   send8BitData(0x2d); //P12
   send8BitData(0x25);  //P13
   send8BitData(0x2b);  //P14
   send8BitData(0x3c); //P15 

   sendCommand(0x2A); //Column Address Set
   send8BitData(0x00);
   send8BitData(0x00);
   send8BitData(0x00);
   send8BitData(0x7f); //SET 127

   sendCommand(0x2B);  //Page Address set
   send8BitData(0x00);
   send8BitData(0x00);
   send8BitData(0x00);
   send8BitData(0x9f);//SET159
   
   sendCommand(0x11); // Exit Sleep
   delay(20);
   sendCommand(0x29); // Display On
   delay(50);
   sendCommand(0x2C); 
   sbi(_pinCS, _bitmaskCS);
   delay(100);
   clearScreen();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
