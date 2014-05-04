/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#include "DmTftBase.h"

uint32_t DmTftBase::myPow(uint8_t m,uint8_t n) {
  uint32_t result=1;     
  while (n--) {
    result *= m;
  }
  return result;
}

void DmTftBase::setPixel(uint16_t posX, uint16_t posY, uint16_t color) {
  cbi(_pinCS, _bitmaskCS);
  
  setAddress(posX, posY, posX, posY);
  
  sendData(color);
  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::fillScreen(void) {
  cbi(_pinCS, _bitmaskCS);

  setAddress(0,0,_width-1, _height-1);
  uint16_t color = 0x0000;

  for(uint16_t i=0; i<_height; i++) {
    for(uint16_t j=0; j<_width; j++) {
        sendData(color);
    }
  }
    
  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::fillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color) {
  unsigned long  XY=0;
  unsigned long i=0;

  if(XL > XR) {
    XL = XL^XR;
    XR = XL^XR;
    XL = XL^XR;
  }

  if(YU > YD) {
    YU = YU^YD;
    YD = YU^YD;
    YU = YU^YD;
  }

  XL = constrain(XL, 0, _width-1);
  XR = constrain(XR, 0, _width-1);
  YU = constrain(YU, 0, _height-1);
  YD = constrain(YD, 0, _height-1);

  XY = (XR-XL+1);
  XY = XY*(YD-YU+1);

  cbi(_pinCS, _bitmaskCS);
  setAddress(XL,YU,XR,YD);/* start to write to display ra */

  for(i=0; i < XY; i++) {
    sendData(color);
  }
    
  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::fillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color) {
  fillScreen(poX, poX+length, poY, poY+width, color);
}

void  DmTftBase::drawHorizontalLine( uint16_t poX, uint16_t poY,uint16_t length,uint16_t color) {
  cbi(_pinCS, _bitmaskCS);
  
  setAddress(poX,poY,poX + length,poY);
  
  for (int i = 0; i < length; i++) {
    sendData(color);
  }
    
  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::drawVerticalLine(uint16_t poX, uint16_t poY, uint16_t length,uint16_t color) {
  cbi(_pinCS, _bitmaskCS);
  
  setAddress(poX,poY,poX ,poY + length);

  for (int i = 0; i < length; i++) {
    sendData(color);
  }
    
  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::drawLine( uint16_t x0,uint16_t y0,uint16_t x1, uint16_t y1,uint16_t color) {
  int x = x1-x0;
  int y = y1-y0;
  int dx = abs(x), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2;  /* error value e_xy             */
  for (;;) {
    setPixel(x0,y0,color);
    e2 = 2*err;
    if (e2 >= dy) {      /* e_xy+e_x > 0                 */
      if (x0 == x1) {
        break;
      }
      err += dy; x0 += sx;
    }
    if (e2 <= dx) { /* e_xy+e_y < 0   */
      if (y0 == y1) {
        break;
      }
      err += dx; y0 += sy;
    }
  }
}

void DmTftBase::drawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width,uint16_t color) {
  drawHorizontalLine(poX, poY, length, color);
  drawHorizontalLine(poX, poY+width, length, color);
  drawVerticalLine(poX, poY, width,color);
  drawVerticalLine(poX + length, poY, width,color);
}

void DmTftBase::drawCircle(int poX, int poY, int r,uint16_t color) {
  int x = -r, y = 0, err = 2-2*r, e2;
  do {
    setPixel(poX-x, poY+y,color);
    setPixel(poX+x, poY+y,color);
    setPixel(poX+x, poY-y,color);
    setPixel(poX-x, poY-y,color);
    e2 = err;
    if (e2 <= y) {
      err += ++y*2+1;
      if (-x == y && e2 <= x) {
        e2 = 0;
      }
    }
    if (e2 > x) {
      err += ++x * 2 + 1;
    }
  } while (x <= 0);
}

void DmTftBase::fillCircle(int poX, int poY, int r,uint16_t color) {
  int x = -r, y = 0, err = 2-2*r, e2;
  do {
    drawVerticalLine(poX-x, poY-y, 2*y, color);
    drawVerticalLine(poX+x, poY-y, 2*y, color);

    e2 = err;
    if (e2 <= y) {
      err += ++y * 2 + 1;
      if (-x == y && e2 <= x) {
        e2 = 0;
      }
    }
    if (e2 > x) {
      err += ++x*2+1;
    }
  } while (x <= 0);
}

void DmTftBase::drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color) {
  drawLine(poX1, poY1, poX2, poY2,color);
  drawLine(poX1, poY1, poX3, poY3,color);
  drawLine(poX2, poY2, poX3, poY3,color);
}

void DmTftBase::drawPoint(uint16_t x,uint16_t y) {
  cbi(_pinCS, _bitmaskCS);

  setAddress(x,y,x,y);
  sendData(POINT_COLOR);      
    
  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::drawPoint_big(uint16_t x,uint16_t y) {
  fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}

void DmTftBase::fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color) {       
  cbi(_pinCS, _bitmaskCS);

  uint16_t i,j; 
  setAddress(xsta,ysta,xend,yend);

  for(i=ysta;i<=yend;i++)   {                                                           
    for (j = xsta; j <= xend; j++) {
      sendData(color);
    }
  }     

  sbi(_pinCS, _bitmaskCS);
}

void DmTftBase::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 

  delta_x=x2-x1; 
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if (delta_x > 0) {
    incx = 1;
  }
  else if (delta_x == 0) {
    incx = 0;
  }
  else {
    incx=-1;
    delta_x=-delta_x;
  }

  if (delta_y > 0) {
    incy = 1;
  }
  else if (delta_y == 0) {
    incy = 0;
  }
  else {
    incy=-1;
    delta_y=-delta_y;
  }

  if (delta_x > delta_y){
    distance=delta_x;
  }
  else {
    distance=delta_y;
  }

  for (t=0;t<=distance+1;t++ ) {  
    drawPoint(uRow,uCol);
    xerr+=delta_x ; 
    yerr+=delta_y ; 
    if(xerr>distance) { 
      xerr-=distance; 
      uRow+=incx; 
    } 
    if(yerr>distance) { 
      yerr-=distance; 
      uCol+=incy; 
    } 
  }  
}    

void DmTftBase::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  drawLine(x1,y1,x2,y1);
  drawLine(x1,y1,x1,y2);
  drawLine(x1,y2,x2,y2);
  drawLine(x2,y1,x2,y2);
}

void DmTftBase::drawCircle(uint16_t x0,uint16_t y0,uint8_t r) {
  int a,b;
  int di;
  a=0;b=r;    
  di=3-(r<<1);

  while(a<=b) {
    drawPoint(x0-b,y0-a);             //3           
    drawPoint(x0+b,y0-a);             //0           
    drawPoint(x0-a,y0+b);             //1       
    drawPoint(x0-b,y0-a);             //7           
    drawPoint(x0-a,y0-b);             //2             
    drawPoint(x0+b,y0+a);             //4               
    drawPoint(x0+a,y0-b);             //5
    drawPoint(x0+a,y0+b);             //6 
    drawPoint(x0-b,y0+a);             
    a++;
    //use Bresenham Algorithm
    if (di < 0) {
      di += 4 * a + 6;
    }
    else {
      di+=10+4*(a-b);   
      b--;
    } 
    drawPoint(x0+a,y0+b);
  }
} 

void DmTftBase::showChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode) {
  cbi(_pinCS, _bitmaskCS);

  uint8_t temp;
  uint8_t pos,t;
  uint16_t x0=x;
  uint16_t colortemp=POINT_COLOR;      
  
  if (x > _width - 16 || y > _height - 16) {
    return;
  }     
    
  num=num-' ';
  setAddress(x,y,x+8-1,y+16-1);      
  if (!mode) { //Non-overlap_ping mode
    for(pos=0;pos<16;pos++) {
#if defined (__AVR__)
      temp=pgm_read_byte(&font[(uint16_t)num*16+pos]); //use 16* 8 font
#elif defined (TOOLCHAIN_ARM_MICRO)
      temp=font[(uint16_t)num*16+pos];     
#endif
      for(t=0;t<8;t++) {                 
        if (temp & 0x01) {
          POINT_COLOR = colortemp;
        }   
        else {
          POINT_COLOR = BACK_COLOR;
        }
        sendData(POINT_COLOR);  
        temp>>=1; 
        x++;
      }
      x=x0;
      y++;
    }   
  }
  else { //Overlap_ping mode
    for(pos=0;pos<16;pos++) {
#if defined (__AVR__)
      temp=pgm_read_byte(&font[(uint16_t)num*16+pos]);      //use 16* 8 font
#elif defined (TOOLCHAIN_ARM_MICRO)
      temp=font[(uint16_t)num*16+pos];     
#endif
      for(t=0;t<8;t++) {                 
        if (temp & 0x01) {
          drawPoint(x + t, y + pos);
        }
        temp>>=1; 
      }
    }
  }

  POINT_COLOR=colortemp;    
  sbi(_pinCS, _bitmaskCS);
}   
             
void DmTftBase::showNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len) {           
  uint8_t t,temp;
  uint8_t enshow=0;
  num=(uint16_t)num;
  for(t=0;t<len;t++) {
    temp=(num/myPow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1)) {
      if(temp==0) {
        showChar(x+8*t,y,' ',0);
        continue;
      }
      else {
        enshow = 1;
      }
    }
    showChar(x+8*t,y,temp+48,0); 
  }
} 
 
void DmTftBase::show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len) {          
  uint8_t t,temp;                          
  for(t=0;t<len;t++) {
    temp=(num/myPow(10,len-t-1))%10;
    showChar(x+8*t,y,temp+'0',0); 
  }
}

void DmTftBase::showString(uint16_t x, uint16_t y, const char *p) {
  while(*p!='\0')
  { 
    if(x>_width-16) {
      x=0;
      y+=16;
    }
    if(y>_height-16) {
      y=x=0;
    }
    showChar(x,y,*p,0);
    x+=8;
    p++;
  }
}
