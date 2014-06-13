#include <SPI.h>
#include <DmTftSsd2119.h>
#include <DmTftIli9341.h>
#include <DmTftIli9325.h>
#include <DmTouch.h>
#include <utility/DmTouchCalibration.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4
#define T_IRQ   2

DmTftSsd2119 tft = DmTftSsd2119();
DmTouch dmTouch = DmTouch(DmTouch::DM_TFT35_107, DmTouch::Auto, false);
DmTouchCalibration calibration = DmTouchCalibration(&tft, &dmTouch);

bool calibrated = false;

void setup() {
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  pinMode(T_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(F_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(T_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(F_CS, HIGH);
  
  tft.init();
  dmTouch.init();
}

void loop() {
  uint16_t x, y = 0;
  bool touched = false;
  
  if (!calibrated) {
    tft.drawString(5, 5, "Press and hold on cross");
    tft.drawString(5, 25, "until it turns green. ");
      
    Point displayRefPoint[5];
    Point touchRefPoint[5];
    if (calibration.getTouchReferencePoints(displayRefPoint, touchRefPoint, tft.width(), tft.height())) {
      CalibrationMatrix calibrationMatrix = calibration.calculateCalibrationMatrix(displayRefPoint, touchRefPoint);
      dmTouch.setCalibrationMatrix(calibrationMatrix);
      tft.clearScreen();
      calibration.drawCalibPoint(displayRefPoint[0].x, displayRefPoint[0].y); 
      calibration.drawCalibPoint(displayRefPoint[1].x, displayRefPoint[1].y); 
      calibration.drawCalibPoint(displayRefPoint[2].x, displayRefPoint[2].y); 
      calibration.drawCalibPoint(displayRefPoint[3].x, displayRefPoint[3].y); 
      calibration.drawCalibPoint(displayRefPoint[4].x, displayRefPoint[4].y);
      calibration.drawCalibration(calibrationMatrix);
      calibrated = true;
    } else {
      tft.clearScreen();
      tft.drawString(5, 5, "Calibration failed");
      tft.drawString(5, 25, "Please try again.");
      delay(2000);
      tft.clearScreen();
      return;
    }
  }
  
  if (dmTouch.isTouched()) {
    dmTouch.readTouchData(x,y,touched);
    calibration.drawCalibPoint(x, y);
  }
}
















