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
 *
 *
 * File description:
 *
 *     Main program file.
 */

// If set to 1, pre-recorded seeing and input level values are used (no need to connect the photodiode)
#define SENSOR_SIMULATION  1

#include <LiquidCrystal.h>
#include <TimerThree.h>

#include "bar_meter.h"
#include "btn_handler.h"
#include "fonts/12x22.h"
#include "fonts/4x6.h"
#include "graph.h"
#include "rotenc_handler.h"
#include "SH1106.h"
#include "str_utils.h"
#if SENSOR_SIMULATION
#include "sim_data.h"
#endif


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

/// Number of seeing values used for calculating the running average
constexpr uint8_t AVG_COUNT = 30;

/// Decimal separator used for displaying values on screen
#define DEC_SEP ","

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

// --------------------------------------- Globals ---------------------------------------

volatile bool timerTriggered = false;
bool ledOn = false;

BUTTON_HANDLER(KnobPress, 7, 20_ms)
ROT_ENC_HANDLER(KnobRotate, 2, 3, 20_ms)

Display::SH1106 oled(10, 12, 11);
uint8_t oledContrast = 0x80;

const uint8_t bmpSun9x9[] PROGMEM =
{
    0x10, 0x92, 0x54, 0x38, 0xFF, 0x38, 0x54, 0x92, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
};

Graph graph(oled, AVG_COUNT/2,
            /*make room for Y axis marks */Fonts::Font4x6.width + 2 + oled.GetFirstVisibleCol(),
            oled.GetVisibleColCount());

struct
{
    uint8_t seeing[AVG_COUNT] = { 0 }; // Seeing values 0.0"-10.0" scaled to 0-255
    uint8_t nextPos = 0;
    uint16_t sum = 0;
} runningAvg;

struct
{
    float intensity, varying;
    unsigned long numSamples;
} sum;

// ---------------------------------------------------------------------------------------


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Timer3.initialize(CALCULATION_PERIOD);
    Timer3.attachInterrupt([]() { timerTriggered = true; });

    KnobPress::Init();
    KnobRotate::Init();

    Serial.begin(115200);

    oled.Init(oledContrast);
    oled.FillPattern(0x00);
    // Y axis marks of the seeing graph
    oled.DrawText(oled.GetFirstVisibleCol(), 64 - 1*40/3 - Fonts::Font4x6.height/2, "1", Fonts::Font4x6);
    oled.DrawText(oled.GetFirstVisibleCol(), 64 - 2*40/3 - Fonts::Font4x6.height/2, "2", Fonts::Font4x6);

    oled.DrawBitmap(84, 0, 9, 9, bmpSun9x9);
}

/// Shows info on screen
void showInfo(float seeingArcSec, float avgIntensity)
{
    oled.DrawText(2, 0, FormatStr("%01d" DEC_SEP "%02d\"", FLOAT_ARGS(seeingArcSec, 100)), Fonts::Font12x22);

    DrawMeter<8>(avgIntensity * 255/AVG_INTENSITY_MAX, 98, 0, oled);
    oled.DrawText(128 + 2 - 4*6, 12, FormatStr("%d" DEC_SEP "%02d V", FLOAT_ARGS(avgIntensity, 100)), Fonts::Font4x6);

    // Graph is 40 pixels high, max displayed value is 3.0"
    uint8_t displayedAvg = runningAvg.sum/AVG_COUNT * 10.0f/255  * 40/3.0f;
    graph.DrawValue(seeingArcSec * 40/3.0, displayedAvg);
}

/// Sends data to client over serial port
void sendData(float seeingArcSec, float avgIntensity)
{
    Serial.print(FormatStr("%s: %d.%02d\n"
                           "%s: %d.%02d\n"
                           "%s: %d\n",
                           CommProtocol::Input, FLOAT_ARGS(avgIntensity, 100),
                           CommProtocol::Seeing, FLOAT_ARGS(seeingArcSec, 100),
                           CommProtocol::Samples, sum.numSamples));
}

void loop()
{
    bool wasTimerTriggered = false;
    bool knobPressed = false;
    bool knobRotated = false;
    bool knobRotClockwise;

    { ATOMIC

        wasTimerTriggered = timerTriggered;
        timerTriggered = false;

        if (KnobPress::pressed)
        {
            knobPressed = true;
            KnobPress::pressed = false;
        }

        if (KnobRotate::flags.changed)
        {
            knobRotated = true;
            knobRotClockwise = KnobRotate::flags.clockwise;
            KnobRotate::flags.changed = false;
        }
    }

    if (knobRotated)
    {
        if (knobRotClockwise)
        {
            if (oledContrast <= 0xFF - 0x10)
                oledContrast += 0x10;
            else
                oledContrast = 0xFF;
        }
        else
        {
            if (oledContrast >= 0x10)
                oledContrast -= 0x10;
            else
                oledContrast = 0x00;
        }

        oled.SetContrast(oledContrast);
    }

    if (wasTimerTriggered)
    {
        digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
        ledOn = !ledOn;
        wasTimerTriggered = false;

#if !SENSOR_SIMULATION

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

#else // SENSOR_SIMULATION

        float seeingArcSec, avgIntensity;
        SimData::GetNextSeeingAndInputVals(seeingArcSec, avgIntensity);

#endif

        runningAvg.seeing[runningAvg.nextPos] = seeingArcSec * 255 / 10.0f;
        runningAvg.sum += runningAvg.seeing[runningAvg.nextPos];
        runningAvg.sum -= runningAvg.seeing[(runningAvg.nextPos + 1) % AVG_COUNT];
        runningAvg.nextPos = (runningAvg.nextPos + 1) % AVG_COUNT;

        showInfo(seeingArcSec, avgIntensity);

        sendData(seeingArcSec, avgIntensity);

        sum.intensity = 0;
        sum.varying = 0;
        sum.numSamples = 0;
    }

#if !SENSOR_SIMULATION

    const float input = Normalize(analogRead(VARYING_INTENSITY_INPUT));
    sum.varying   += sq(input);
    sum.intensity += Normalize(analogRead(AVERAGE_INTENSITY_INPUT));

    sum.numSamples++;

#endif
}
