/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include <SPI.h>
#include <SPIFlash.h> // SPIFlash library from lowpowerlab (https://github.com/LowPowerLab/SPIFlash)
#include <SD.h>
#include <DmTftRa8875.h>
#include <DmDrawBmpFromSpiFlash.h>

#define TFT_CS  10
#define SD_CS   8
#define F_CS    6
#define F_SEL   9
DmTftRa8875 tft = DmTftRa8875(TFT_CS, F_SEL);
DmDrawBmpFromSpiFlash drawImage = DmDrawBmpFromSpiFlash();

SPIFlash spiFlash(F_CS, 0xEF40);
uint16_t textRow = 0;
uint8_t bl_value = 255;
char bl_value_array[5];
int16_t x_pos = 0;
int16_t y_pos = 0;
void setup()
{
  // Set CS SPI pin HIGH for all SPI units, so they don't interfere
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  pinMode(F_CS, OUTPUT);
  digitalWrite(F_CS, HIGH);
  
  Serial.begin(9600);
  //'RA8875_480x272' or 'RA8875_800x480'
  tft.init(RA8875_480x272);
  tft.backlightOn(true); // Turn on backlight
  tft.backlightAdjust(255);   // default value max
  
  initFlash(spiFlash);
  tft.drawString(5, 0, "www.displaymodule.com");
  Serial.print(F("Initializing SD-card: "));
  PrintTestName("Initializing SD-card: ");
  if (SD.begin(SD_CS)) {
    Serial.println(F("OK"));
    PrintTestResult("OK");
  }
  else
  {
    Serial.println(F("Fail"));
    PrintTestResult("Fail");
  }
    // write from SD-card to flash memory
    PrintTestName("Erase Flash ");
    eraseFlash(spiFlash);
    PrintTestResult("OK");
    
    PrintTestName("Write bmp to ext. flash");
    writeFileToFlash(0, spiFlash, "logol565.bin");
    PrintTestResult("OK");
    
    delay(1000);
  uint32_t startTime, endTime;
  
  startTime = millis();
  //drawImageContinuous(uint32_t startaddress,uint32_t count,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1)
  tft.drawImageContinuous(0, 76800, 0, 0, 320, 240);
  endTime = millis();
  Serial.print("Draw Image: ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
  
  delay(1500);
  tft.clearScreen(BLACK);
  
  startTime = millis();
  //drawImageBlock(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t pic_width, uint16_t block_width, uint16_t block_height)
  tft.drawImageBlock(3200, 64000, (tft.width()-320)/2, (tft.height()-200)/2, (tft.width()-320)/2 + 320, (tft.height()-200)/2 + 200, 320, 320, 200);
  endTime = millis();
  Serial.print("Draw Block Image : ");
  Serial.print(endTime - startTime, DEC);
  Serial.println(" ms");
  
   // key test
  textRow = 0;
  PrintTestName("Press a key to continue!");  
  tft.eableKeyScan(true);
  uint8_t key_value;
  char buf[8];
  while(1){
    delay(50);
    if(tft.isKeyPress()){
      key_value = tft.getKeyValue();
      switch(key_value)
      {
        case 0:          
          if(bl_value >= 11){
            bl_value -=10;
            tft.backlightAdjust(bl_value);
            String bl_text = String(bl_value);
            bl_text.toCharArray(bl_value_array, sizeof(bl_value_array));
            tft.clearScreen(BLACK);
            tft.drawString((tft.width()-170)/2, tft.height()/2, "Current BL value is: ");
            tft.drawString((tft.width()-170)/2 + 160,tft.height()/2, bl_value_array);
          }
          else{
             tft.clearScreen(BLACK);
             tft.drawString((tft.width()-170)/2,tft.height()/2,"Current BL value is Minimum");
          }
        break;
        
        case 1:
          if(bl_value <= 244){
            bl_value +=10;
            tft.backlightAdjust(bl_value);
            String bl_text = String(bl_value);
            bl_text.toCharArray(bl_value_array, sizeof(bl_value_array));
            tft.clearScreen(BLACK);
            tft.drawString((tft.width()-170)/2, tft.height()/2, "Current BL value is: ");
            tft.drawString((tft.width()-170)/2 + 160,tft.height()/2, bl_value_array);
          }
          else{
             tft.clearScreen(BLACK);
             tft.drawString((tft.width()-170)/2,tft.height()/2,"Current BL value is Maximum");
          }          
        break;
        
        case 2:
          if(x_pos >= 0){
            tft.clearScreen(BLACK);
            tft.drawImageContinuous(0, 76800, x_pos, y_pos, x_pos + 320, y_pos + 240);
            x_pos -= (tft.width()-320)/16;
            y_pos -= (tft.height()-240)/16;
            if(x_pos < 0){
              x_pos = 0;
              y_pos = 0;
            }            
          }             
        break;
        
        case 3:
          if(x_pos <= tft.width()-320){
            tft.clearScreen(BLACK);
            tft.drawImageContinuous(0, 76800, x_pos, y_pos, x_pos + 320, y_pos + 240);
            x_pos += (tft.width()-320)/16;
            y_pos += (tft.height()-240)/16;
            if(x_pos > tft.width()-320){
               x_pos = (tft.width()-320);
              y_pos = (tft.height()-240);
            }            
          }          
        break;
        
        case 4:
          PrintTestName("Key 5 is pressed!");
        break;  
        
        default :
          break;       
      }
    }
  }
  
  
  
}

void loop() { }

void initFlash(SPIFlash spiFlash) {
  Serial.print(F("External Flash: "));
  if (spiFlash.initialize()) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("Fail"));
  }
}

void eraseFlash(SPIFlash spiFlash) {
  Serial.print(F("Erasing Flash: "));
  spiFlash.chipErase();
  while(spiFlash.busy());
  Serial.println(F("OK"));
}

void writeFileToFlash(uint32_t startAddress, SPIFlash spiFlash, char* fileName) {
  uint32_t fileSize;
  uint32_t counter;
  
  Serial.print(F("Write to Flash: "));
  
  File imageFile;
  if ((imageFile = SD.open(fileName)) == NULL) {
    Serial.println(F("Can't open image file"));
    return;
  }
  
  fileSize = imageFile.size();
  counter = startAddress;
  while(counter<=fileSize) {
    spiFlash.writeByte(counter, imageFile.read());
    counter++;
  }
  imageFile.close();
  
  Serial.println(F("OK"));
}

void PrintTestName(char* text) {
  textRow += 20;
  
  if(textRow >= tft.height()-20){
    tft.clearScreen(BLACK);
    textRow = 10;    
  }  
  tft.drawString(5, textRow, text);
  Serial.print(text);
}
void PrintTestResult(char* text) {
  tft.drawString(200, textRow, text);
  Serial.print(": ");
  Serial.println(text);
}
