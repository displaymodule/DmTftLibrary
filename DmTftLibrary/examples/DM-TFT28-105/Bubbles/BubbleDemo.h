
#ifndef BUBBLEDEMO_H
#define BUBBLEDEMO_H

#include "DmTftBase.h"

class BubbleDemo {
public:

    enum Constants {
        NumBalls = 2//17
    };

    /** Set the address of the frame buffer to use.
     *
     *  It is the content of the frame buffer that is shown on the
     *  display. All the drawing on the frame buffer can be done
     *  'offline' and whenever it should be shown this function
     *  can be called with the address of the offline frame buffer.
     *
     *  @param pFrameBuf  Pointer to the frame buffer, which must be
     *                    3 times as big as the frame size (for tripple
     *                    buffering).
     *         dispWidth  The width of the display (in pixels).
     *         dispHeight The height of the display (in pixels).
     *         loops      Number of loops in the demo code.
     *         delayMs    Delay in milliseconds between schreen updates.
     *
     *  @returns
     *       none
     */
    BubbleDemo(DmTftBase* display, uint16_t dispWidth, uint16_t dispHeight);
    
    void run(uint32_t loops, uint32_t delayMs);

private:
    int32_t windowX;
    int32_t windowY;
    
    DmTftBase* tft;
    
    uint8_t i;
    uint8_t j;
    
    float x[NumBalls];
    float y[NumBalls];
    uint8_t r[NumBalls];
    
    float oldX[NumBalls];
    float oldY[NumBalls];
    
    float velX[NumBalls];
    float velY[NumBalls];
    
    uint8_t red[NumBalls];
    uint8_t green[NumBalls];
    uint8_t blue[NumBalls];
    
    
    void initialize();
    void collision();
    void borders();
    void draw();
};

#endif /* BUBBLEDEMO_H */


