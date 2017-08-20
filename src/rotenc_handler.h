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
 *     Interrupt-based rotary encoder handler.
 */

#pragma once
#include <Arduino.h>

/**
    Rotary encoder handler

    Usage:

    1) call  RotEncName::Init() from setup()

    2) in loop() add the following:

        bool encChanged = false;
        bool encRotClockwise;

        noInterrupts();
        if (RotEncName::flags.changed)
        {
            encChanged = true;
            encRotClockwise = RotEncName::flags.clockwise;

            RotEncName::flags.changed = false;
        }
        interrupts();

        if (encChanged) { <react to encoder change> }

*/
#define ROT_ENC_HANDLER(RotEncName, Channel1Pin, Channel2Pin, DebounceTimeUs)             \
                                                                                          \
namespace RotEncName                                                                      \
{                                                                                         \
    volatile unsigned long tsLastChanged = 0;                                             \
                                                                                          \
    struct                                                                                \
    {                                                                                     \
        volatile uint8_t changed   : 1;                                                   \
        volatile uint8_t clockwise : 1;                                                   \
    } flags = { 0 };                                                                      \
                                                                                          \
    void Init()                                                                           \
    {                                                                                     \
        /* Due to enabling internal pull-up resistors, encoder will use inverted logic */ \
        pinMode(Channel1Pin, INPUT_PULLUP);                                               \
        pinMode(Channel2Pin, INPUT_PULLUP);                                               \
                                                                                          \
        attachInterrupt(digitalPinToInterrupt(Channel1Pin),                               \
                        []()                                                              \
                        {                                                                 \
                            if (digitalRead(Channel2Pin) == LOW)                          \
                            {                                                             \
                                const auto timestamp = micros();                          \
                                if (timestamp - tsLastChanged >= DebounceTimeUs)          \
                                {                                                         \
                                    flags.changed = true;                                 \
                                    flags.clockwise = true;                               \
                                    tsLastChanged = timestamp;                            \
                                }                                                         \
                            }                                                             \
                        },                                                                \
                        FALLING);                                                         \
                                                                                          \
        attachInterrupt(digitalPinToInterrupt(Channel2Pin),                               \
                        []()                                                              \
                        {                                                                 \
                            if (digitalRead(Channel1Pin) == LOW)                          \
                            {                                                             \
                                const auto timestamp = micros();                          \
                                if (timestamp - tsLastChanged >= DebounceTimeUs)          \
                                {                                                         \
                                    flags.changed = true;                                 \
                                    flags.clockwise = false;                              \
                                    tsLastChanged = timestamp;                            \
                                }                                                         \
                            }                                                             \
                        },                                                                \
                        FALLING);                                                         \
    }                                                                                     \
}
