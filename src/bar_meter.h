#pragma once
#include "SH1106.h"

/** Draws a meter consisting of bars of increasing height.

    Example (7 bars total, 3 filled):


                              #
                          #
                      #       #
                  #       #
            ###       #       #
        ### ###   #       #
    ### ### ###       #       #


    The meter always occupies a multiple of 8 pixels vertically, i.e. for 1-8 bars it takes 8 pixels (1 page),
    for 9-16 bars it takes 16 pixels (2 pages) etc.
 */
template<uint8_t NumBars>
static void DrawMeter(uint8_t value, ///< 0 to 255
                      uint8_t posX, uint8_t posY,
                      Display::SH1106 &display)
{
    constexpr uint8_t numPages = (NumBars + 7) / 8;
    constexpr uint8_t width = 4 * NumBars - 1;
    const uint8_t numFilledBars = (value == 0) ? 0 : (NumBars * (value + 255/NumBars - 1)) / 255;

    static uint8_t drawBuf[width] = { 0 }; // for drawing a single page (a group of 8 rows)

    for (uint8_t page = 0; page < numPages; page++)
    {
        for (uint8_t bar = 0; bar < NumBars; bar++)
        {
            const uint8_t p = numPages - page - 1;
            uint8_t barPatternInPage;
            if (bar/8 < p)
                barPatternInPage = 0;
            else if (bar/8 == p)
                barPatternInPage = 0xFF << (7 - (bar) % 8);
            else
                barPatternInPage = 0xFF;

            if (bar >= numFilledBars)
            {
                drawBuf[bar*4 + 0] = 0;
                drawBuf[bar*4 + 1] = 0;
                if (bar & 1)
                    drawBuf[bar*4 + 2] = barPatternInPage & 0x55;
                else
                    drawBuf[bar*4 + 2] = barPatternInPage & 0xAA;
            }
            else
            {
                drawBuf[bar*4 + 0] = barPatternInPage;
                drawBuf[bar*4 + 1] = barPatternInPage;
                drawBuf[bar*4 + 2] = barPatternInPage;
            }
        }

        display.UpdatePixels(page + posY/8, posX, width, drawBuf);
    }
}
