#pragma once
#include <Arduino.h>

/**
    Button handler

    Usage:

    1) call  BtnName::Init() from setup()

    2) in loop() add the following:

        bool btnPressed = false;

        noInterrupts();
        if (BtnName::pressed)
        {
            btnPressed = true;
            BtnName::pressed = false;
        }
        interrupts();

        if (btnPressed) { <react to button press> }

*/
#define BUTTON_HANDLER(BtnName, BtnPin, DebounceTimeUs)                       \
                                                                              \
namespace BtnName                                                             \
{                                                                             \
    volatile unsigned long tsLastPressed = 0;                                 \
    volatile bool pressed = false;                                            \
                                                                              \
    void Init()                                                               \
    {                                                                         \
        pinMode(BtnPin, INPUT_PULLUP);                                        \
        attachInterrupt(digitalPinToInterrupt(BtnPin),                        \
                        []()                                                  \
                        {                                                     \
                            const auto timestamp = micros();                  \
                            if (timestamp - tsLastPressed >= DebounceTimeUs)  \
                            {                                                 \
                                pressed = true;                               \
                                tsLastPressed = timestamp;                    \
                            }                                                 \
                        }, FALLING);                                          \
    }                                                                         \
}
