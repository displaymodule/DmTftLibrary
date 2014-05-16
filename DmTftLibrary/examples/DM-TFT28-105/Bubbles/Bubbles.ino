/******************************************************************************
 * Includes
 *****************************************************************************/

#include "mbed.h"

#include "BubbleDemo.h"

#include "DmTftHX8353C.h"
#include "DmTftS6D0164.h"
#include "DmTftIli9325.h"
#include "DmTftIli9341.h"
#include "DmTftSsd2119.h"

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

#define RESET_FLAG  \
  do { \
    if (abortTest) { \
      abortTest = false; \
      wait(0.04); \
    } \
  } while(false)


/******************************************************************************
 * Local variables
 *****************************************************************************/

static InterruptIn buttonInterrupt(P2_10);
static DigitalOut led(LED1);

//DmTftHX8353C tft;  /* DM_TFT18_101 */
//DmTftS6D0164 tft;  /* DM_TFT22_102 */
//DmTftIli9325 tft;  /* DM_TFT28_103 and DM_TFT24_104 */
DmTftIli9341 tft;  /* DM_TFT28_105 */
//DmTftSsd2119 tft;   /* DM_TFT35_107 */

/******************************************************************************
 * Global variables
 *****************************************************************************/

/******************************************************************************
 * Main
 *****************************************************************************/

int main (void)
{
    tft.init();

    BubbleDemo bubbleDemo(&tft, tft.width(), tft.height());
    while (1) {    
        bubbleDemo.run(750, 20);
    }
}
