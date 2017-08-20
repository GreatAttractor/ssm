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
 *     4x6 font's glyph data in SH1106-friendly format.
 */

#pragma once

#include <avr/pgmspace.h>
#include "fonts.h"


namespace Fonts
{

const uint8_t Font4x6data[] PROGMEM =
{
        // character code:
        0x20, // ASCII: ' '
        // . . . .
        // . . . .
        // . . . .
        // . . . .
        // . . . .
        // . . . .
        //
        // pixel data:
        0x00, 0x00, 0x00, 0x00,

        // character code:
        0x22, // ASCII: '"'
        // # . # .
        // # . # .
        // . . . .
        // . . . .
        // . . . .
        // . . . .
        //
        // pixel data:
        0x03, 0x00, 0x03, 0x00,

        // character code:
        0x2C, // ASCII: ','
        // . . . .
        // . . . .
        // . . . .
        // . . . .
        // . # . .
        // # . . .
        //
        // pixel data:
        0x20, 0x10, 0x00, 0x00,

        // character code:
        0x2E, // ASCII: '.'
        // . . . .
        // . . . .
        // . . . .
        // . . . .
        // . # . .
        // . . . .
        //
        // pixel data:
        0x00, 0x10, 0x00, 0x00,

        // character code:
        0x30, // ASCII: '0'
        // # # # .
        // # . # .
        // # . # .
        // # . # .
        // # # # .
        // . . . .
        //
        // pixel data:
        0x1F, 0x11, 0x1F, 0x00,

        // character code:
        0x31, // ASCII: '1'
        // . # . .
        // . # . .
        // . # . .
        // . # . .
        // . # . .
        // . . . .
        //
        // pixel data:
        0x00, 0x1F, 0x00, 0x00,

        // character code:
        0x32, // ASCII: '2'
        // # # # .
        // . . # .
        // . # . .
        // # . . .
        // # # # .
        // . . . .

        //
        // pixel data:
        0x19, 0x15, 0x13, 0x00,

        // character code:
        0x33, // ASCII: '3'
        // # # # .
        // . . # .
        // . # # .
        // . . # .
        // # # # .
        // . . . .

        //
        // pixel data:
        0x11, 0x15, 0x1F, 0x00,

        // character code:
        0x34, // ASCII: '4'
        // # . . .
        // # . # .
        // # # # .
        // . . # .
        // . . # .
        // . . . .

        //
        // pixel data:
        0x07, 0x04, 0x1E, 0x00,

        // character code:
        0x35, // ASCII: '5'
        // # # # .
        // # . . .
        // # # # .
        // . . # .
        // # # . .
        // . . . .

        //
        // pixel data:
        0x17, 0x15, 0x0D, 0x00,

        // character code:
        0x36, // ASCII: '6'
        // # . . .
        // # . . .
        // # # # .
        // # . # .
        // # # # .
        // . . . .

        //
        // pixel data:
        0x1F, 0x14, 0x1C, 0x00,

        // character code:
        0x37, // ASCII: '7'
        // # # # .
        // . . # .
        // . # . .
        // # . . .
        // # . . .
        // . . . .

        //
        // pixel data:
        0x19, 0x05, 0x03, 0x00,

        // character code:
        0x38, // ASCII: '8'
        // # # # .
        // # . # .
        // # # # .
        // # . # .
        // # # # .
        // . . . .

        //
        // pixel data:
        0x1F, 0x15, 0x1F, 0x00,

        // character code:
        0x39, // ASCII: '9'
        // # # # .
        // # . # .
        // # # # .
        // . . # .
        // . . # .
        // . . . .

        //
        // pixel data:
        0x07, 0x05, 0x1F, 0x00,

        // character code:
        0x56, // ASCII: 'V'
        // # . # .
        // # . # .
        // # . # .
        // # . # .
        // . # . .
        // . . . .

        //
        // pixel data:
        0x0F, 0x10, 0x0F, 0x00,

};

const FontDef Font4x6 =
{
    4, 6,
    15,
    Font4x6data
};

} // namespace Fonts
