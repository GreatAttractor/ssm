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
 *     SH1106 controller driver implementation.
 */

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include "SH1106.h"


enum OpCode: uint8_t
{
//    TurnOff              = 0xAE,
//    SetColLowAddr        = 0x02,
//    SetColHighAddr       = 0x10,
//    SetStartLineAddr     = 0x40,
    ContrastControl      = 0x81,
//    SetSegColMapping     = 0xA0,
//    SetScanDirection     = 0xC0,
//    SetNormalDisplay     = 0xA6,
//    SetMultiplexRatio    = 0xA8,
//    SetDutyCycle         = 0x3F,

    SetPage              = 0xB0, ///< Add page number (0..7)
    SetLowColAddr        = 0x00, ///< Add low part of column address (0..0xF)
    SetHighColAddr       = 0x10, ///< Add high part of column address (0..0xF)
            //
};

namespace Display
{

void SH1106::Command(uint8_t cmd)
{
    digitalWrite(PinDC, LOW);
    SPI.transfer(cmd);
}

SH1106::SH1106(uint8_t pinRES, uint8_t pinCS, uint8_t pinDC, uint8_t width, uint8_t height)
: PinRES(pinRES), PinCS(pinCS), PinDC(pinDC), Width(width), Height(height)
{ }

void SH1106::Init(uint8_t contrast)
{
    pinMode(PinRES, OUTPUT);
    pinMode(PinDC, OUTPUT);
    pinMode(PinCS, OUTPUT);
    SPI.begin();

    SPI.setClockDivider(SPI_CLOCK_DIV128);

    digitalWrite(PinCS, LOW);
    digitalWrite(PinRES, HIGH);
    delay(10);
    digitalWrite(PinRES, LOW);
    delay(10);
    digitalWrite(PinRES, HIGH);

    Command(0xAE);//--turn off oled panel
    Command(0x02);//---set low column address
    Command(0x10);//---set high column address
    Command(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    Command(0x81);//--set contrast control register
    Command(0xA0);//--Set SEG/Column Mapping
    Command(0xC0);//Set COM/Row Scan Direction
    Command(0xA6);//--set normal display
    Command(0xA8);//--set multiplex ratio(1 to 64)
    Command(0x3F);//--1/64 duty
    Command(0xD3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    Command(0x00);//-not offset
    Command(0xd5);//--set display clock divide ratio/oscillator frequency
    Command(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    Command(0xD9);//--set pre-charge period
    Command(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    Command(0xDA);//--set com pins hardware configuration
    Command(0x12);
    Command(0xDB);//--set vcomh
    Command(0x40);//Set VCOM Deselect Level
    Command(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    Command(0x02);//
    Command(0xA4);// Disable Entire Display On (0xa4/0xa5)
    Command(0xA6);// Disable Inverse Display On (0xa6/a7)
    Command(0xAF);//--turn on oled panel

    SetContrast(contrast);
}

void SH1106::FillPattern(uint8_t pattern)
{
    uint8_t page;

    for (page = 0; page < 8; page++) {
        /* set page address */
        Command(0xB0 + page);
        /* set low column address */
        Command(0x00 + GetFirstVisibleCol());
        /* set high column address */
        Command(0x10);

        /* write data */
        digitalWrite(PinDC, HIGH);
        for (uint8_t x = 0; x < Width; x++)
            SPI.transfer(pattern);
    }
}

void SH1106::FillPattern(uint8_t page, uint8_t startCol, uint8_t colCount, uint8_t pattern)
{
    Command(OpCode::SetPage + page);
    Command(OpCode::SetLowColAddr + (startCol & 0x0F));
    Command(OpCode::SetHighColAddr + (startCol >> 4));

    digitalWrite(PinDC, HIGH);
    for (int x = 0; x < colCount; x++)
        SPI.transfer(pattern);
}

void SH1106::SetContrast(uint8_t contrast)
{
    Command(OpCode::ContrastControl);
    Command(contrast);
}

void SH1106::UpdatePixels(uint8_t page, uint8_t startCol, uint8_t colCount, const void *srcBuf)
{
    Command(OpCode::SetPage + page);
    Command(OpCode::SetLowColAddr  + (startCol & 0x0F));
    Command(OpCode::SetHighColAddr + (startCol >> 4));

    digitalWrite(PinDC, HIGH);
    for (uint8_t i = 0; i < colCount; i++)
        SPI.transfer(((const uint8_t *)srcBuf)[i]);
}

void SH1106::DrawBitmap(uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, const uint8_t *pixels)
{
    if (width == 0 || height == 0 || width > MAX_BITMAP_WIDTH || height > MAX_BITMAP_HEIGHT)
        return;

    const uint8_t pageOffs = posY & 0x7;
    const uint8_t numBmpPages = (height + 7) >> 3;

    memset(DrawBuf, 0, sizeof(DrawBuf));

    if (pageOffs == 0)
    {
        for (uint8_t page = 0; page < numBmpPages; page++)
            // 'pixels' are in PROGMEM, so we use 'memcpy_P' instead of 'memcpy'
            memcpy_P(DrawBuf[page], pixels + page*width, width);

        for (uint8_t page = posY >> 3; page < (posY >> 3) + numBmpPages; page++)
            UpdatePixels(page, posX, width, DrawBuf[page - (posY >> 3)]);
    }
    else
    {
        for (uint8_t page = 0; page < numBmpPages; page++)
            for (uint8_t col = 0; col < width; col++)
            {
                uint8_t pix = pgm_read_word(pixels + col + page*width);

                DrawBuf[page  ][col] |= pix << pageOffs;
                DrawBuf[page+1][col] |= pix >> (8-pageOffs);
            }

        for (uint8_t page = posY >> 3; page < (posY >> 3) + numBmpPages + 1; page++)
            UpdatePixels(page, posX, width, DrawBuf[page - (posY >> 3)]);
    }
}

void SH1106::DrawText(uint8_t posX, uint8_t posY, const char *str,
                      const Fonts::FontDef &font)
{
    const char *currChar = str;

    while (*currChar != '\0')
    {
        DrawBitmap(posX, posY, font.width, font.height, font.FindGlyph((uint8_t)*currChar));
        posX += font.width;
        currChar++;
    }
}

} // namespace Display

