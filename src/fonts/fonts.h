#pragma once

#include <stddef.h>
#include <stdint.h>


namespace Fonts
{

struct FontDef
{
    uint8_t width;  ///< Glyph width in pixels
    uint8_t height; ///< Glyph height in pixels; actual number of pixels used is ((height+7)/8)*8
    uint16_t numGlyphs;

    /// Points to glyph pixel data (PROGMEM)
    /** Number of elements depends on the previous fields. There are 'numGlyphs' groups of bytes,
        each containing:

            - 1-byte code (ASCII or extended, i.e. >0x7F)
            - (height+7)/8 groups of 'width' bytes defining pixels

        The groups are sorted by the glyph code.
     */
    const uint8_t *glyphs;

    /// Returns distance between items in 'glyphs'
    ptrdiff_t BytesPerGlyph() const { return 1 + width * ((height + 7)/8); }

    /// Returns pointer to glyph's bitmap contents
    const uint8_t *FindGlyph(uint8_t charCode) const
    {
        uint8_t low = 0;
        uint8_t high = numGlyphs - 1;
        uint8_t mid = 0;

        do
        {
            mid = (low + high)/2;
            uint8_t midCC = pgm_read_byte(glyphs + mid * BytesPerGlyph());
            if (midCC < charCode)
                low = mid + 1;
            else if (midCC > charCode)
            {
                if (mid == 0)
                    break; // 'charCode' not found
                else
                    high = mid - 1;
            }
            else
                break;

        } while (low <= high);

        if (low > high)
            mid = 0; // 'charCode' not found

        return glyphs + 1 + mid * BytesPerGlyph();
    }
};

extern const FontDef Font8x14;
extern const FontDef Font12x22;

} // namespace Fonts
