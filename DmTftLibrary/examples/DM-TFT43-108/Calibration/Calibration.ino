#include <SPI.h>
#include <DmTftRa8875.h>
#include <DmTouch.h>
#include <utility/DmTouchCalibration.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define T_CS    4
#define T_IRQ   2

DmTftRa8875 tft = DmTftRa8875();
DmTouch dmTouch = DmTouch(DmTouch::DM_TFT43_108, DmTouch::Auto, true);
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
  
  Serial.begin(9600);
  //'RA8875_480x272' or 'RA8875_800x480'
  tft.init(RA8875_480x272);
  dmTouch.init();
  tft.backlightOn(true); // Turn on backlight
  tft.backlightAdjust(255);   // default value max  
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
      tft.clearScreen(BLACK);

      calibration.drawCalibPoint(displayRefPoint[0].x, displayRefPoint[0].y); 
      calibration.drawCalibPoint(displayRefPoint[1].x, displayRefPoint[1].y); 
      calibration.drawCalibPoint(displayRefPoint[2].x, displayRefPoint[2].y); 
      calibration.drawCalibPoint(displayRefPoint[3].x, displayRefPoint[3].y); 
      calibration.drawCalibPoint(displayRefPoint[4].x, displayRefPoint[4].y);
      calibration.drawCalibration(calibrationMatrix);
      calibrated = true;
    } else {
      tft.clearScreen(BLACK);
      tft.drawString(5, 5, "Calibration failed");
      tft.drawString(5, 25, "Please try again.");
      delay(2000);
      tft.clearScreen(BLACK);

      return;
    }
  }
  
  if (dmTouch.isTouched()) {
    dmTouch.readTouchData(x,y,touched);
    calibration.drawCalibPoint(x, y);
    
    /* Another way to read touch data */
    // if(dmTouch.readTouchData2(x,y)) calibration.drawCalibPoint(x, y);    
  }
}
















