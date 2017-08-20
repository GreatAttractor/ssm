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
 *     String utils.
 */

#pragma once

#include <stdarg.h>
#include <stdio.h>


/// Max length of string that can be created by FormatStr
constexpr int MAX_PRINTF_LENGTH = 64;

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
