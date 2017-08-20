#include <Arduino.h>
#include "graph.h"


/** Changes the specified bit in graphBar[] using the following order of bits:

        graphBar[0]_bit7..graphBar[0]_bit0, graphBar[1]_bit7..graphBar[1]_bit0, ..., graphBar[4]_bit7..graphBar[4]_bit0

    I.e. bit=0 means 7-th bit in graphBar[0], bit=1 means 6th bit in graphBar[0], ..., bit=39 means 0th bit in graphBar[4].
*/
void ChangeBit(Graph::GraphBar &graphBar,
               uint8_t bit, ///< Value from [0; 39]
               bool setBit)
{
    const uint8_t mask = 1 << (7 - (bit % 8));
    uint8_t &byteToChange = graphBar[bit/8];

    if (setBit)
        byteToChange |= mask;
    else
        byteToChange &= ~mask;
}

void FillGraphBar(Graph::GraphBar &graphBar, uint8_t value)
{
    constexpr uint16_t ONES16 = 0xFF00;

    graphBar[0] = ONES16 >> value;
    if (value >= 8) graphBar[0] = 0xFF;

    graphBar[1] = ONES16 >> (value - 8);
    if (value >= 16) graphBar[1] = 0xFF;

    graphBar[2] = ONES16 >> (value - 16);
    if (value >= 24) graphBar[2] = 0xFF;

    graphBar[3] = ONES16 >> (value - 24);
    if (value >= 32) graphBar[3] = 0xFF;

    graphBar[4] = ONES16 >> (value - 32);
    if (value >= 40) graphBar[4] = 0xFF;
}

/// Overlays two dashed horizontal lines at 1/3 and 2/3 of 'graphBar'
void OverlayDashedGridLines(Graph::GraphBar &graphBar, uint8_t pos)
{
    if (pos & 1)
    {
        graphBar[1] |= 0x04;
        graphBar[3] |= 0x20;
    }
    else
    {
        graphBar[1] &= ~0x04;
        graphBar[3] &= ~0x20;
    }
}

void Graph::DrawBlankStrip(uint8_t pos)
{
    constexpr uint8_t STRIP_WIDTH = 3;
    static const uint8_t zero[STRIP_WIDTH] = { 0 };

    if (pos > lastX)
        return;
    else
    {
        const uint8_t stripWidth = min(STRIP_WIDTH, lastX - pos + 1);
        for (int i = 0; i < 5; i++)
            display.UpdatePixels(3 + i, pos, stripWidth, zero);
    }
}

/// Overlays 'value' over an existing bit pattern in 'graphBar' by using a black pixel surrounded by single white pixels
void OverlaySecondaryValue(Graph::GraphBar &graphBar, int8_t value)
{
    constexpr int8_t WHITE_RADIUS = 1;
    constexpr int8_t BLACK_RADIUS = 1;

    // Set pixel corresponding with 'value' to black
    ChangeBit(graphBar, (uint8_t)value, false);

    // Set pixels within BLACK_RADIUS around 'value' to black
    for (uint8_t i = 1; i <= BLACK_RADIUS; i++)
    {
        if (value + i < 40)
            ChangeBit(graphBar, value + i, false);

        if (value - i >= 0)
            ChangeBit(graphBar, value - i, false);
    }

    // Set pixels within WHITE_RADIUS around the black pixels above to white
    for (uint8_t i = 1; i <= WHITE_RADIUS; i++)
    {
        if (value + BLACK_RADIUS + i < 40)
            ChangeBit(graphBar, value + BLACK_RADIUS + i, true);
        if (value - BLACK_RADIUS - i >= 0)
            ChangeBit(graphBar, value - BLACK_RADIUS - i, true);
    }
}

void Graph::DrawValue(uint8_t value1, uint8_t value2)
{
    uint8_t graphBar[5];

    // Draw current value
    FillGraphBar(graphBar, value1);
    OverlayDashedGridLines(graphBar, drawPos);
    DrawBar(drawPos, graphBar);

    if (totalValuesDrawn > 2*numStoredValues)
    {
        // Draw the running average (secondary value)

        FillGraphBar(graphBar, storedValues[nextStoreIdx]);
        OverlaySecondaryValue(graphBar, value2);
        const uint8_t secDrawPos = (numStoredValues <= drawPos - posX) ? drawPos - numStoredValues
                                                                       : lastX - (numStoredValues - (drawPos - posX));
        OverlayDashedGridLines(graphBar, secDrawPos);
        DrawBar(secDrawPos, graphBar);

        // Erase a few oldest bars to indicate the latest value location
        DrawBlankStrip(drawPos + 1);
    }

    drawPos++;
    if (drawPos > lastX)
        drawPos = posX;

    if (numStoredValues > 0)
    {
        storedValues[nextStoreIdx++] = value1;
        if (nextStoreIdx == numStoredValues)
            nextStoreIdx = 0;
    }

    totalValuesDrawn++;
}

void Graph::DrawBar(uint8_t pos, const GraphBar &graphBar)
{
    for (int i = 0; i < 5; i++)
        display.UpdatePixels(3 + i, pos, 1, graphBar + 5 - i - 1);
}
