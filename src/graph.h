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
 *     Seeing graph header.
 */

#pragma once

#include "SH1106.h"


class Graph
{
public:

    /// Max number of the stored most recent values
    static constexpr uint8_t MAX_STORED_VALUES = 128;

    Graph(Display::SH1106 &_display,
          uint8_t _numStoredValues,
          uint8_t _posX, ///< Position of the first graph bar
          uint8_t _lastX ///< Position of the last graph bar
    ):
        display(_display),
        numStoredValues(_numStoredValues),
        posX(_posX),
        lastX(_lastX),
        nextStoreIdx(0),
        drawPos(_posX),
        totalValuesDrawn(0)
    { }

    void DrawValue(uint8_t value1, ///< Primary value (in pixels)
                   uint8_t value2  ///< Secondary value (in pixels), drawn at position earlier by 'numStoredValues'
    );

    using GraphBar = uint8_t[5]; /// Element [0] stores the bottom-most pixels (with highest Y)

private:

    void DrawBar(uint8_t pos, const GraphBar &graphBar);
    void DrawBlankStrip(uint8_t pos);


    Display::SH1106 &display;
    uint8_t numStoredValues;
    uint8_t posX;
    uint8_t lastX;
    uint8_t nextStoreIdx;
    uint8_t storedValues[MAX_STORED_VALUES];
    uint8_t drawPos; ///< Current draw position
    uint32_t totalValuesDrawn;
};
