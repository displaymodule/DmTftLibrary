/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmTouchCalibration.h"
//#define log(...) Serial.println(__VA_ARGS__)
#define log(...)

DmTouchCalibration::DmTouchCalibration(DmTftBase *tftBase, DmTouch *touch) {
  _tft = tftBase;
  _touch = touch;
  _textRow = 20;
}

// For best result, create a calibration for each display
CalibrationMatrix DmTouchCalibration::getDefaultCalibrationData(int disp) {
  CalibrationMatrix calibrationMatrix;
  switch (disp) {
    case DmTouch::DM_TFT28_103:
      calibrationMatrix.a = 63787;
      calibrationMatrix.b = -138;
      calibrationMatrix.c = -15921157;
      calibrationMatrix.d = -244;
      calibrationMatrix.e = 89313;
      calibrationMatrix.f = -10726623;
      break;
    case DmTouch::DM_TFT24_104:
      calibrationMatrix.a = -71855;
      calibrationMatrix.b = 2147;
      calibrationMatrix.c = 259719524;
      calibrationMatrix.d = -1339;
      calibrationMatrix.e = -91012;
      calibrationMatrix.f = 354268832;
      break;    	  
    case DmTouch::DM_TFT28_105:
      calibrationMatrix.a = 65521;
      calibrationMatrix.b = -253;
      calibrationMatrix.c = -11813673;
      calibrationMatrix.d = -439;
      calibrationMatrix.e = 89201;
      calibrationMatrix.f = -10450920;
	  break;	
	case DmTouch::DM_TFT35_107:
      calibrationMatrix.a = 85984;
      calibrationMatrix.b = 451;
      calibrationMatrix.c = -16494041;
      calibrationMatrix.d = 2308;
      calibrationMatrix.e = 65173;
      calibrationMatrix.f = -19179080;
      break;	  
	case DmTouch::DM_TFT43_108: 
		if(_tft->width() == 320){        // DM-TFT35-324
      		calibrationMatrix.a = 339489;
      		calibrationMatrix.b = 1602;
      		calibrationMatrix.c = -15546011;
      		calibrationMatrix.d = -241;
      		calibrationMatrix.e = 271933;
      		calibrationMatrix.f = -23184312;	
		}else if(_tft->width() == 480){  // DM-TFT43-302 or DM-TFT43-327
      		calibrationMatrix.a = 541307;
      		calibrationMatrix.b = -4288;
      		calibrationMatrix.c = -36678732;
      		calibrationMatrix.d = 2730;
      		calibrationMatrix.e = 321714;
      		calibrationMatrix.f = -31439472;	
		}else if(_tft->width() == 800){   //DM-TFT50-303 or DM-TFT50-318
      		calibrationMatrix.a = 871837;
      		calibrationMatrix.b = 7273;
      		calibrationMatrix.c = -57891280;
      		calibrationMatrix.d = -5861;
      		calibrationMatrix.e = 536190;
      		calibrationMatrix.f = -39840708;					
		}
		break;
	case DmTouch::DM_TFT18_310:
	  calibrationMatrix.a = 34852;
	  calibrationMatrix.b = -242;
	  calibrationMatrix.c = -5593092;
	  calibrationMatrix.d = 244;
	  calibrationMatrix.e = 46016;
	  calibrationMatrix.f = -10464877;
	  break;
	case DmTouch::DM_TFT24_312:
	  calibrationMatrix.a = 64832;
	  calibrationMatrix.b = -663;
	  calibrationMatrix.c = -8230510;
	  calibrationMatrix.d = 652;
	  calibrationMatrix.e = -86408;
	  calibrationMatrix.f = 338477984;
	  break;	  
	case DmTouch::DM_TFT24_314:
	  calibrationMatrix.a = 64894;
	  calibrationMatrix.b = -58;
	  calibrationMatrix.c = -14175864;
	  calibrationMatrix.d = -412;
	  calibrationMatrix.e = 87172;
	  calibrationMatrix.f = -12410956;
	  break;	
	case DmTouch::DM_TFT24_316:
	  calibrationMatrix.a = -65670;
	  calibrationMatrix.b = 1494;
	  calibrationMatrix.c = 247084992;
	  calibrationMatrix.d = -96;
	  calibrationMatrix.e = -88946;
	  calibrationMatrix.f = 331414016;
	  break;	  
    default:
      break;
  }
  return calibrationMatrix;
}


bool DmTouchCalibration::getTouchReferencePoints(Point displayRefPoint[], Point touchRefPoint[], uint16_t tftWidth, uint16_t tftHeight) {

	_touch->setPrecison(60);
  displayRefPoint[0].x = (tftWidth / 2);
  displayRefPoint[0].y = (tftHeight / 2);
  displayRefPoint[1].x = 1 * (tftWidth / 5);
  displayRefPoint[1].y = 1 * (tftHeight / 5);
  displayRefPoint[2].x = 4 * (tftWidth / 5);
  displayRefPoint[2].y = 1 * (tftHeight / 5);
  displayRefPoint[3].x = 4 * (tftWidth / 5);
  displayRefPoint[3].y = 4 * (tftHeight / 5);
  displayRefPoint[4].x = 1 * (tftWidth / 5);
  displayRefPoint[4].y = 4 * (tftHeight / 5);  

  for(int n;n<5;n++) {
    drawCalibPoint(displayRefPoint[n].x, displayRefPoint[n].y);
    if (!getRawTouch(touchRefPoint[n].x, touchRefPoint[n].y))
      	return false;
    releaseTouch(displayRefPoint[n].x, displayRefPoint[n].y);
  }
  
  _touch->setPrecison(1);
  return true;
}

bool DmTouchCalibration::getRawTouch(uint16_t& x, uint16_t& y) {
  bool touched = false;
  bool hasAllSamples = false;
  while(!touched) {
    if (_touch->isTouched()) {
      hasAllSamples = _touch->getMiddleXY(x, y);			
      touched = true; 
    }
  }
  return hasAllSamples;
}

void DmTouchCalibration::drawCalibPoint(uint16_t x, uint16_t y) {
  const uint8_t lineSize = 5;
  _tft->drawHorizontalLine(x-lineSize, y, lineSize*2, RED);
  _tft->drawVerticalLine(x, y-lineSize, lineSize*2, RED);
}

void DmTouchCalibration::releaseTouch(uint16_t x, uint16_t y) {
  _tft->drawCircle(x, y, 10, YELLOW);
  delay(100);
  _tft->fillCircle(x, y, 10, GREEN);
  delay(100);
  _tft->fillCircle(x, y, 10, BLACK);
  delay(300);
	_touch->waitForTouchRelease();
}

CalibrationMatrix DmTouchCalibration::calculateCalibrationMatrix(Point displayRefPoint[], Point touchRefPoint[]) {
  double A = 0.0, B = 0.0, C = 0.0, D = 0.0, E = 0.0, F = 0.0;
  double d = 0.0, dx1 = 0.0, dx2 = 0.0, dx3 = 0.0, dy1 = 0.0, dy2 = 0.0, dy3 = 0.0;
  uint32_t X2_1 = 0, X2_2 = 0, X2_3 = 0, X2_4 = 0, X2_5 = 0;
  uint32_t Y2_1 = 0, Y2_2 = 0, Y2_3 = 0, Y2_4 = 0, Y2_5 = 0;
  uint32_t XxY_1 = 0, XxY_2 = 0, XxY_3 = 0, XxY_4 = 0, XxY_5 = 0;
  uint32_t XxXd_1 = 0, XxXd_2 = 0, XxXd_3 = 0, XxXd_4 = 0, XxXd_5 = 0;
  uint32_t YxXd_1 = 0, YxXd_2 = 0, YxXd_3 = 0, YxXd_4 = 0, YxXd_5 = 0;
  uint32_t XxYd_1 = 0, XxYd_2 = 0, XxYd_3 = 0, XxYd_4 = 0, XxYd_5 = 0;
  uint32_t YxYd_1 = 0, YxYd_2 = 0, YxYd_3 = 0, YxYd_4 = 0, YxYd_5 = 0;
  uint32_t alfa = 0, beta = 0, chi = 0, Kx = 0, Ky = 0, Lx = 0, Ly = 0;
  uint16_t epsilon = 0, fi = 0, Mx = 0, My = 0;
  
  X2_1 = ((uint32_t)touchRefPoint[0].x * touchRefPoint[0].x);
  X2_2 = ((uint32_t)touchRefPoint[1].x * touchRefPoint[1].x);
  X2_3 = ((uint32_t)touchRefPoint[2].x * touchRefPoint[2].x);
  X2_4 = ((uint32_t)touchRefPoint[3].x * touchRefPoint[3].x);
  X2_5 = ((uint32_t)touchRefPoint[4].x * touchRefPoint[4].x);

  Y2_1 = ((uint32_t)touchRefPoint[0].y * touchRefPoint[0].y);
  Y2_2 = ((uint32_t)touchRefPoint[1].y * touchRefPoint[1].y);
  Y2_3 = ((uint32_t)touchRefPoint[2].y * touchRefPoint[2].y);
  Y2_4 = ((uint32_t)touchRefPoint[3].y * touchRefPoint[3].y);
  Y2_5 = ((uint32_t)touchRefPoint[4].y * touchRefPoint[4].y);
   
  XxY_1 = ((uint32_t)touchRefPoint[0].x * touchRefPoint[0].y);
  XxY_2 = ((uint32_t)touchRefPoint[1].x * touchRefPoint[1].y);
  XxY_3 = ((uint32_t)touchRefPoint[2].x * touchRefPoint[2].y);
  XxY_4 = ((uint32_t)touchRefPoint[3].x * touchRefPoint[3].y);
  XxY_5 = ((uint32_t)touchRefPoint[4].x * touchRefPoint[4].y);

  XxXd_1 = ((uint32_t)touchRefPoint[0].x * displayRefPoint[0].x);
  XxXd_2 = ((uint32_t)touchRefPoint[1].x * displayRefPoint[1].x);
  XxXd_3 = ((uint32_t)touchRefPoint[2].x * displayRefPoint[2].x);
  XxXd_4 = ((uint32_t)touchRefPoint[3].x * displayRefPoint[3].x);
  XxXd_5 = ((uint32_t)touchRefPoint[4].x * displayRefPoint[4].x);

  YxXd_1 = ((uint32_t)touchRefPoint[0].y * displayRefPoint[0].x);
  YxXd_2 = ((uint32_t)touchRefPoint[1].y * displayRefPoint[1].x);
  YxXd_3 = ((uint32_t)touchRefPoint[2].y * displayRefPoint[2].x);
  YxXd_4 = ((uint32_t)touchRefPoint[3].y * displayRefPoint[3].x);
  YxXd_5 = ((uint32_t)touchRefPoint[4].y * displayRefPoint[4].x);

  XxYd_1 = ((uint32_t)touchRefPoint[0].x * displayRefPoint[0].y);
  XxYd_2 = ((uint32_t)touchRefPoint[1].x * displayRefPoint[1].y);
  XxYd_3 = ((uint32_t)touchRefPoint[2].x * displayRefPoint[2].y);
  XxYd_4 = ((uint32_t)touchRefPoint[3].x * displayRefPoint[3].y);
  XxYd_5 = ((uint32_t)touchRefPoint[4].x * displayRefPoint[4].y);

  YxYd_1 = ((uint32_t)touchRefPoint[0].y * displayRefPoint[0].y);
  YxYd_2 = ((uint32_t)touchRefPoint[1].y * displayRefPoint[1].y);
  YxYd_3 = ((uint32_t)touchRefPoint[2].y * displayRefPoint[2].y);
  YxYd_4 = ((uint32_t)touchRefPoint[3].y * displayRefPoint[3].y);
  YxYd_5 = ((uint32_t)touchRefPoint[4].y * displayRefPoint[4].y);

  alfa = X2_1 + X2_2 + X2_3 + X2_4 + X2_5;
  beta = Y2_1 + Y2_2 + Y2_3 + Y2_4 + Y2_5;
  chi = XxY_1 + XxY_2 + XxY_3 + XxY_4 + XxY_5;
  epsilon = touchRefPoint[0].x + touchRefPoint[1].x + touchRefPoint[2].x + touchRefPoint[3].x + touchRefPoint[4].x;
  fi = touchRefPoint[0].y + touchRefPoint[1].y + touchRefPoint[2].y + touchRefPoint[3].y + touchRefPoint[4].y;
  Kx = XxXd_1 + XxXd_2 + XxXd_3 + XxXd_4 + XxXd_5;
  Ky = XxYd_1 + XxYd_2 + XxYd_3 + XxYd_4 + XxYd_5;
  Lx = YxXd_1 + YxXd_2 + YxXd_3 + YxXd_4 + YxXd_5;
  Ly = YxYd_1 + YxYd_2 + YxYd_3 + YxYd_4 + YxYd_5;
  Mx = displayRefPoint[0].x + displayRefPoint[1].x + displayRefPoint[2].x + displayRefPoint[3].x + displayRefPoint[4].x;
  My = displayRefPoint[0].y + displayRefPoint[1].y + displayRefPoint[2].y + displayRefPoint[3].y + displayRefPoint[4].y;
  d = 5 * ( ((double)alfa * beta) - ((double)chi * chi) ) + 2 * ((double)chi * epsilon * fi) - ((double)alfa * fi * fi ) - ( (double)beta * epsilon * epsilon );
  dx1 = 5 * ( ((double)Kx * beta) - ((double)Lx * chi) ) + ((double)fi * ( ((double)Lx * epsilon) - ((double)Kx * fi) )) + ((double)Mx * ( ((double)chi * fi) - ((double)beta * epsilon) ));
  dx2 = 5 * ( ((double)Lx * alfa) - ((double)Kx * chi) ) + ((double)epsilon * ( ((double)Kx * fi) - ((double)Lx * epsilon) )) + ((double)Mx * ( ((double)chi * epsilon) - ((double)alfa * fi) ));
  dx3 = ((double)Kx * ( ((double)chi * fi) - ((double)beta * epsilon) )) + ((double)Lx * ( ((double)chi * epsilon) - ((double)alfa * fi) )) + ((double)Mx * ( ((double)alfa * beta) - ((double)chi * chi) ));
  dy1 = 5 * ( ((double)Ky * beta) - ((double)Ly * chi) ) + ((double)fi * ( ((double)Ly * epsilon) - ((double)Ky * fi) )) + ((double)My * ( ((double)chi * fi) - ((double)beta * epsilon) ));
  dy2 = 5 * ( ((double)Ly * alfa) - ((double)Ky * chi) ) + ((double)epsilon * ( ((double)Ky * fi) - ((double)Ly * epsilon) )) + ((double)My * ( ((double)chi * epsilon) - ((double)alfa * fi) ));
  dy3 = ((double)Ky * ( ((double)chi * fi) - ((double)beta * epsilon) )) + ((double)Ly * ( ((double)chi * epsilon) - ((double)alfa * fi) )) + ((double)My * ( ((double)alfa * beta) - ((double)chi * chi) ));

  A = dx1 / d;
  B = dx2 / d;
  C = dx3 / d;
  D = dy1 / d;
  E = dy2 / d;
  F = dy3 / d;

  CalibrationMatrix calibrationData;
  calibrationData.a = (int32_t)(A * _touch->rescaleFactor());
  calibrationData.b = (int32_t)(B * _touch->rescaleFactor());
  calibrationData.c = (int32_t)(C * _touch->rescaleFactor());
  calibrationData.d = (int32_t)(D * _touch->rescaleFactor());
  calibrationData.e = (int32_t)(E * _touch->rescaleFactor());
  calibrationData.f = (int32_t)(F * _touch->rescaleFactor());

  return calibrationData;
}

void DmTouchCalibration::drawCalibration(CalibrationMatrix calibrationMatrix) {
  printHeadline("CalibrationData: ");
  printHeadline("a: ");
  printNumber(calibrationMatrix.a);
  printHeadline("b: ");
  printNumber(calibrationMatrix.b);
  printHeadline("c: ");
  printNumber(calibrationMatrix.c);
  printHeadline("d: ");
  printNumber(calibrationMatrix.d);
  printHeadline("e: ");
  printNumber(calibrationMatrix.e);
  printHeadline("f: ");
  printNumber(calibrationMatrix.f);
}

void DmTouchCalibration::printHeadline(char* text) {
  _textRow += 20;
  _tft->drawString(5, _textRow, text);
}

void DmTouchCalibration::printNumber(int32_t number) {
  String text = String(number);
  char numberAsCharArray[80];
  text.toCharArray(numberAsCharArray, sizeof(numberAsCharArray));
  _tft->drawString(20, _textRow, numberAsCharArray);
}

void DmTouchCalibration::printCalibration(CalibrationMatrix calibrationMatrix) {
  Serial.println(F("-------- Calibration data ------"));
  Serial.println(F("CalibrationMatrix calibrationMatrix;"));
  Serial.print(F("calibrationMatrix.a = "));
  Serial.print(calibrationMatrix.a);
  Serial.println(F(";"));
  Serial.print(F("calibrationMatrix.b = "));
  Serial.print(calibrationMatrix.b);
  Serial.println(F(";"));
  Serial.print(F("calibrationMatrix.c = "));
  Serial.print(calibrationMatrix.c);
  Serial.println(F(";"));
  Serial.print(F("calibrationMatrix.d = "));
  Serial.print(calibrationMatrix.d);
  Serial.println(F(";"));
  Serial.print(F("calibrationMatrix.e = "));
  Serial.print(calibrationMatrix.e);
  Serial.println(F(";"));
  Serial.print(F("calibrationMatrix.f = "));
  Serial.print(calibrationMatrix.f);
  Serial.println(F(";"));
  Serial.println("-------------------------------");
}
