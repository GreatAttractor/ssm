/*
 * Solar Scintillation Monitor firmware
 * Copyright (c) 2017 Filip Szczerek <ga.software@yahoo.com>
 *
 * This project is licensed under the terms of the MIT license
 * (see the LICENSE file for details).
 *
 *
 * Based on:
 *
 *   E. J. Seykora
 *   An Inexpensive Solar Scintillation Seeing Monitor Circuit
 *   with Arduino Interface
 *
 *   https://www.ecu.edu/cs-cas/physics/upload/
 *     An-Inexpensive-Solar-Scintillation-Seeing-Monitor-Circuit-with-Arduino-Interface-final2.pdf
 *
 *
 * In the following code, rather than always taking 10 000 samples of the averaged and varying input,
 * the "Timer Three" library is used to perform sampling over a specified duration (CALCULATION_PERIOD).
 *
 * Building tested with PlatformIO framework (platformio.org).
 */

#include <stdarg.h>
#include <stdio.h>

#include <LiquidCrystal.h>
#include <TimerThree.h>


// Literal operators to convert durations into microseconds
constexpr unsigned long operator "" _s(unsigned long long val) { return val * 1000UL * 1000UL; }
constexpr unsigned long operator "" _ms(unsigned long long val) { return val * 1000UL; }
constexpr unsigned long operator "" _us(unsigned long long val) { return val; }

constexpr unsigned long CALCULATION_PERIOD = 1_s;

constexpr uint8_t AVERAGE_INTENSITY_INPUT = A0;
constexpr uint8_t VARYING_INTENSITY_INPUT = A2;

constexpr int SUN_DIAMETER_ARCSEC = 1900;
constexpr float VARYING_INTENSITY_GAIN = 425.5f; // From the original schematic, where it equals 20M/47k due to resistors' values
constexpr float SEEING_DC_OFFSET = -0.05f; // TODO: explain

constexpr float AVG_INTENSITY_MIN = 0.5; ///< Minimal input voltage allowing seeing calculation
constexpr float AVG_INTENSITY_MAX = 1.2; ///< Maximal input voltage allowing seeing calculation

constexpr float SEEING_INVALID_VAL = 9.99; ///< Returned when the input voltage is too low or too high

/// Max length of string that can be created by FormatStr
constexpr int MAX_PRINTF_LENGTH = 64;


class InterruptDisabler
{
public:

    InterruptDisabler()  { noInterrupts(); }
    ~InterruptDisabler() { interrupts(); }
};

/// Ensures atomic execution of the surrounding scope
#define ATOMIC  InterruptDisabler interruptDisabler;

/// Normalizes analog input (10-bit, 0..1023) to +/- 2.5 V
/** Channels 1 and 2 of the op-amp use voltage divider's (10k:10k) output as their reference, i.e. 1/2 * 5 V = 2.5 V */
inline float Normalize(int analogInput) { return 2.5f/511 * (analogInput - 512); }

// Serial output follows the Airylab's protocol; compatible with popular software, including FireCapture SSM plugin by Joachim Stehle
namespace CommProtocol
{
    // Standard identifiers
    const char *Input  = "A0";
    const char *Seeing = "A1";

    // Other data can also be sent; add new identifiers below as needed
    const char *Samples = "D0";

} // namespace CommProtocol

namespace LCD
{

// Set the values to reflect your LCD<->Arduino wiring
namespace Pins
{
constexpr uint8_t RS = 12;
constexpr uint8_t EN = 11;
constexpr uint8_t D4 = 5;
constexpr uint8_t D5 = 7;
constexpr uint8_t D6 = 4;
constexpr uint8_t D7 = 9;
} // namespace LCD::Pins

namespace Chars
{
    constexpr uint8_t omega = 1;
    constexpr uint8_t phi   = 2;

namespace Bitmaps
{
    uint8_t omega[8] =
    {
        B00000,
        B00000,
        B01010,
        B10001,
        B10101,
        B10101,
        B01010
    };

    uint8_t phi[8] =
    {
        B00100,
        B00100,
        B01110,
        B10101,
        B10101,
        B01110,
        B00100
    };
} // namespace LCD::Chars::Bitmaps

} // namespace LCD::Chars

} // namespace LCD

LiquidCrystal lcd(LCD::Pins::RS, LCD::Pins::EN,
                  LCD::Pins::D4,
                  LCD::Pins::D5,
                  LCD::Pins::D6,
                  LCD::Pins::D7);

volatile bool TimerTriggered = false;

bool ledOn = false;

/// Outputs 2 arguments (integer and fractional part) for FormatStr()
/** To print n decimal places, specify fractWeight = 10^n, e.g.:

      FormatStr("%01d.%02d", FLOAT_ARGS(12.234, 100));

    prints "12.23".
*/
#define FLOAT_ARGS(value, fractWeight)  (int)(value), (int)((value) * (fractWeight)) % (fractWeight)

// Returns a formatted string created with snprintf()
const char* FormatStr(const char* format, ...)
{
    static char buf[MAX_PRINTF_LENGTH + 1];

    va_list args;

    va_start(args, format);
    vsnprintf(buf, MAX_PRINTF_LENGTH + 1, format, args);
    va_end(args);

    return buf;
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    lcd.begin(16, 2);

    lcd.createChar(LCD::Chars::omega, LCD::Chars::Bitmaps::omega);
    lcd.createChar(LCD::Chars::phi, LCD::Chars::Bitmaps::phi);

    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("SSM init");
    delay(1000);
    lcd.clear();

    Timer3.initialize(CALCULATION_PERIOD);
    Timer3.attachInterrupt([]() { TimerTriggered = true; });

    Serial.begin(115200);
}

struct
{
    float intensity, varying;
    unsigned long numSamples;
} sum;

void loop()
{
    bool wasTimerTriggered = false;

    { ATOMIC

        wasTimerTriggered = TimerTriggered;
        TimerTriggered = false;
    }

    if (wasTimerTriggered)
    {
        digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
        ledOn = !ledOn;
        wasTimerTriggered = false;

        float avgIntensity = sum.intensity / sum.numSamples;
        const float rmsVaryingIntensity = sqrt(sum.varying / sum.numSamples);

        float seeingArcSec = SEEING_INVALID_VAL;
        if (avgIntensity >= AVG_INTENSITY_MIN && avgIntensity <= AVG_INTENSITY_MAX)
        {
            /*
             * In "Solar Scintillation and the Monitoring of Solar Seeing" Dr. E. Seykora gives the following formula
             * for the value of seeing in arc seconds:
             *
             *     ω = θ (ΔI / <I>)
             *
             * where θ is the angular diameter of the solar disk in arc seconds, ΔI the RMS of (light sensor's)
             * irradiance changes (+/- relative to the average value), and <I> is the average value of irradiance.
             *
             * The factor 4.46 in Dr. Seykora's original circuit description comes from assuming solar disk diameter
             * of 1900 arcsec; additionally, ΔI needs to be divided by its gain (applied by the op-amp designated
             * "U2" in the original diagram) of 20M/47k ≅ 425.5x. This gives 1900/425.5 ≅ 4.46.
             *
             * This factor is represented below by SUN_DIAMETER_ARCSEC/VARYING_INTENSITY_GAIN.
             */
            seeingArcSec = (SUN_DIAMETER_ARCSEC/VARYING_INTENSITY_GAIN * rmsVaryingIntensity + SEEING_DC_OFFSET) / avgIntensity;
            if (seeingArcSec > SEEING_INVALID_VAL)
                seeingArcSec = SEEING_INVALID_VAL;
        }

        if (avgIntensity < 0)
            avgIntensity = 0;

        lcd.setCursor(0, 0);
        if (seeingArcSec == SEEING_INVALID_VAL)
            lcd.print(FormatStr("%c:----- I:%01d.%02dV", LCD::Chars::omega, FLOAT_ARGS(avgIntensity, 100)));
        else
            lcd.print(FormatStr("%c:%01d.%02d\" I:%01d.%02dV", LCD::Chars::omega, FLOAT_ARGS(seeingArcSec, 100), FLOAT_ARGS(avgIntensity, 100)));

        lcd.setCursor(0, 1);
        lcd.print(FormatStr("samples: %5d", sum.numSamples));

        Serial.print(FormatStr("%s: %d.%02d\n"
                               "%s: %d.%02d\n"
                               "%s: %d\n",
                               CommProtocol::Input, FLOAT_ARGS(avgIntensity, 100),
                               CommProtocol::Seeing, FLOAT_ARGS(seeingArcSec, 100),
                               CommProtocol::Samples, sum.numSamples));

        sum.intensity = 0;
        sum.varying = 0;
        sum.numSamples = 0;
    }

    const float input = Normalize(analogRead(VARYING_INTENSITY_INPUT));
    sum.varying   += sq(input);
    sum.intensity += Normalize(analogRead(AVERAGE_INTENSITY_INPUT));

    sum.numSamples++;
}
