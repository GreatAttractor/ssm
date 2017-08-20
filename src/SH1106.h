/*
 * Solar Scintillation Monitor firmware
 * Copyright (c) 2017 Filip Szczerek <ga.software@yahoo.com>
 *
 * This project is licensed under the terms of the MIT license
 * (see the LICENSE file for details).
 *
 *
 * File description:
 *
 *     SH1106 controller driver header.
 */

#pragma once

#include <stdint.h>
#include "fonts/fonts.h"


namespace Display
{

/// Represents a graphics (dot matrix) display driven by SH1106
class SH1106
{
public:

    // Each instance allocates a draw buffer of MaxBitmapWidth*((MaxBitmapHeight - 2)/8 + 2) bytes
    static constexpr uint8_t MAX_BITMAP_WIDTH  = 32;
    static constexpr uint8_t MAX_BITMAP_HEIGHT = 32;

    SH1106(uint8_t pinRES, uint8_t pinCS, uint8_t pinDC,
           uint8_t width = 128, ///< Does not include non-visible columns
           uint8_t height = 64);

    void Init(uint8_t contrast = 0x80);

    /// Fills whole display with bit pattern
    void FillPattern(uint8_t pattern);

    /// Fills area with specified bit pattern
    void FillPattern(uint8_t page, uint8_t startCol, uint8_t colCount, uint8_t pattern);

    /// Sets display contrast (value from 0x00-0xFF)
    void SetContrast(uint8_t contrast);

    /// Updates pixels in specified page (group of 8 rows)
    void UpdatePixels(uint8_t page,     ///< Page 0 contains rows 0-7, page 1 contains rows 8-15 etc.
                      uint8_t startCol, ///< Note: some columns at the beginning may not be visible; see GetFirstVisibleCol()
                      uint8_t colCount, ///< Number of columns to update

                      /// Each byte contains 8 subsequent pixels of a column; LSB is the pixel with the smallest row number
                     const void *srcBuf);

    /// Draws a bitmap residing in flash (defined with PROGMEM attribute)
    void DrawBitmap(uint8_t posX, uint8_t posY,
                    uint8_t width,  ///< Must not exceed MAX_BITMAP_WIDTH
                    uint8_t height, ///< Must not exceed MAX_BITMAP_HEIGHT

                    /// Accepts only PROGMEM storage
                    /** Consists of 'height'/8 rows (at least 1 row) of 'width' bytes each; each byte contains
                        8 subsequent pixels of a column; least-significant bit is the pixel with the smallest Y. */
                    const uint8_t *pixels);

    /// Prints a null-terminatd string at given position; tab & newline characters are not handled
    void DrawText(uint8_t posX, uint8_t posY, const char *str,
                  const Fonts::FontDef &font);

    /// Returns the first visible column
    uint8_t GetFirstVisibleCol() const { return 2; }

    uint8_t GetVisibleColCount() const { return Width; }


private:

    uint8_t PinRES, PinCS, PinDC, Width, Height;
    uint8_t DrawBuf[(MAX_BITMAP_HEIGHT - 2) / 8 + 2][MAX_BITMAP_WIDTH];

    void Command(uint8_t cmd);
};


} // namespace Display
