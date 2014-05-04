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
#include <DmTftHX8353C.h>

int bmpWidth, bmpHeight;
uint8_t bmpImageoffset;
extern uint8_t dmlogo[];

DmTftHX8353C tft = DmTftHX8353C(2,3,4,5,6);

void setup()
{
  Serial.begin(9600);
  tft.init();
  
  if (! bmpReadHeader()) { 
    Serial.println("bad bmp");
    return;
  }

  drawBmpFromFlash(0, 0);
  drawBmpFromFlash(0, 130);
   
  tft.showString(0,32,"www.");
  tft.showString(12,48,"displaymodule");
  tft.showString(88,64,".com");

  tft.drawRectangle(20,85,20,20,GREEN); 
  tft.drawCircle(60,95,10,BLUE);
  tft.drawTraingle(90,85, 80,105,100,105, RED);
}

void loop()
{
}

// Info about bmp format can be found here: http://en.wikipedia.org/wiki/BMP_file_format
void drawBmpFromFlash(int x, int y) 
{
  uint16_t pos = bmpImageoffset;

  uint32_t time = millis();
  uint16_t p;  // pixel 
  uint8_t g, b;
  int i, j; // line, column

  for(i=bmpHeight; i>0; i--) {
    for(j=0; j<bmpWidth; j++) { 
      // convert pixel from 888 to 565
      b = pgm_read_byte_near(dmlogo+pos++);     // blue
      g = pgm_read_byte_near(dmlogo+pos++);     // green
      p = pgm_read_byte_near(dmlogo+pos++);      // red

      p >>= 3;
      p <<= 6;

      g >>= 2;
      p |= g;
      p <<= 5;

      b >>= 3;
      p |= b;

      // write out the 16 bits of color
      tft.setPixel(j, i+y, p);
    }
  }
  Serial.print("Time to display image on screen: ");
  Serial.print(millis() - time, DEC);
  Serial.println(" ms");
}


boolean bmpReadHeader() {
  uint32_t fileSize;
  uint32_t headerSize;
  uint16_t bmpDepth;
  uint16_t pos = 0;
  
  if (pgm_read_word_near(dmlogo) !=0x4D42){ // read magic byte
    return false;
  }
  pos += 2;

  // read file size
  fileSize = pgm_read_dword_near(dmlogo+pos);
  pos += 4;

  pos += 4; // Skip creator bytes
  
  bmpImageoffset = pgm_read_dword_near(dmlogo+pos);
  pos += 4;

  // read DIB header
  headerSize = pgm_read_dword_near(dmlogo+pos);
  pos +=4;
  bmpWidth = pgm_read_dword_near(dmlogo+pos);
  pos += 4;
  bmpHeight = pgm_read_dword_near(dmlogo+pos);
  pos += 4;

  Serial.print("Image size:     "); 
  Serial.println(fileSize, DEC);
  Serial.print("Image offset:   "); 
  Serial.println(bmpImageoffset, DEC);
  Serial.print("Header size:    "); 
  Serial.println(headerSize, DEC);
  Serial.print("Image size:     "); 
  Serial.print(bmpWidth, DEC);
  Serial.print(", ");
  Serial.println(bmpHeight, DEC);

  if (pgm_read_word_near(dmlogo+pos) != 1){
    // number of color planes must be 1
    return false;
  }
  pos += 2;
  
  bmpDepth = pgm_read_word_near(dmlogo+pos);
  pos +=2;
  Serial.print("Bitdepth:       "); 
  Serial.println(bmpDepth, DEC);

  if (pgm_read_dword_near(dmlogo+pos) != 0) {
    // compression not supported!
    return false;
  }
  pos += 2;

  return true;
}