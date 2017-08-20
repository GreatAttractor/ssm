#pragma once

#include <avr/pgmspace.h>


static uint16_t currentIdx = 0;

namespace SimData
{

// Multiply by 10.0/255 to obtain seeing value in arc seconds
const uint8_t Seeing[] PROGMEM =
{
    28,
    30,
    44,
    37,
    39,
    21,
    23,
    27,
    31,
    34,
    24,
    35,
    30,
    30,
    33,
    28,
    27,
    28,
    27,
    26,
    28,
    28,
    29,
    28,
    30,
    29,
    26,
    32,
    36,
    22,
    30,
    28,
    14,
    32,
    40,
    35,
    35,
    21,
    24,
    34,
    43,
    53,
    46,
    56,
    37,
    35,
    47,
    34,
    39,
    37,
    50,
    47,
    45,
    50,
    64,
    58,
    39,
    55,
    39,
    34,
    52,
    78,
    55,
    77,
    77,
    48,
    34,
    31,
    39,
    40,
    43,
    29,
    37,
    46,
    63,
    59,
    114,
    35,
    30,
    30,
    35,
    42,
    46,
    40,
    52,
    40,
    36,
    52,
    46,
    61,
    46,
    41,
    27,
    30,
    27,
    39,
    43,
    39,
    58,
    64,
    54,
    42,
    46,
    43,
    46,
    52,
    40,
    37,
    29,
    36,
    28,
    28,
    34,
    40,
    39,
    34,
    29,
    34,
    37,
    35,
    46,
    44,
    40,
    34,
    43,
    36,
    37,
    54,
    39,
    45,
    37,
    45,
    33,
    28,
    31,
    32,
    34,
    33,
    32,
    28,
    34,
    31,
    34,
    35,
    34,
    29,
    37,
    54,
    63,
    41,
    43,
    41,
    34,
    30,
    51,
    40,
    40,
    32,
    24,
    26,
    29,
    42,
    47,
    40,
    34,
    32,
    36,
    37,
    32,
    40,
    33,
    32,
    40,
    43,
    37,
    35,
    30,
    40,
    36,
    39,
    38,
    28,
    29,
    30,
    30,
    36,
    33,
    37,
    32,
    26,
    31,
    33,
    36,
    45,
    37,
    33,
    39,
    29,
    26,
    32,
    28,
    28,
    31,
    28,
    34,
    37,
    26,
    44,
    48,
    35,
    43,
    34,
    30,
    33,
    26,
    24,
    36,
    33,
    47,
    35,
    35,
    33,
    44,
    30,
    41,
    35,
    44,
    48,
    61,
    53,
    44,
    41,
    37,
    31,
    30,
    31,
    36,
    35,
    22,
    22,
    24,
    28,
    54,
    34,
    48,
    45,
    53,
    34,
    25,
    37,
    47,
    47,
    44,
    34,
    38,
    38,
    47,
    37,
    44,
    64,
    56,
    50,
    59,
    75,
    74,
    65,
    32,
    38,
    52,
    58,
    58,
    44,
    41,
    27,
    37,
    31,
    24,
    28,
    35,
    33,
    37,
    30,
    34,
    32,
    29,
    42,
    32,
    36,
    42,
    37,
    33,
    35,
    28,
    30,
    43,
    21,
    22,
    27,
    33,
    39,
    47,
    35,
    42,
    41,
    33,
    21,
    34,
    37,
    30,
    24,
    29,
    20,
    42,
    35,
    29,
    34,
    40,
    24,
    18,
    28,
    32,
    42,
    32,
    38,
    38,
    43,
    33,
    55,
    61,
    46,
    57,
    57,
    64,
    46,
    56,
    61,
    59,
    47,
    62,
    41,
    38,
    45,
    47,
    93,
    36,
    49,
    44,
    39,
    42,
    34,
    33,
    49,
    51,
    53,
    36,
    36,
    36,
    33,
    35,
    35,
    31,
    35,
    37,
    33,
    28,
    30,
    30,
    39,
    58,
    55,
    24,
    36,
    29,
    21,
    36,
    41,
    33,
    29,
    20,
    30,
    33,
    33,
    22,
    40,
    33,
    30,
    27,
    33,
    30,
    31,
    35,
    35,
    40,
    41,
    35,
    36,
    31,
    28,
    25,
    32,
    67,
    35,
    29,
    28,
    31,
    24,
    29,
    30,
    32,
    37,
    49,
    34,
    28,
    32,
    34,
    33,
    32,
    31,
    20,
    25,
    27,
    31,
    34,
    31,
    39,
    37,
    33,
    34,
    37,
    30,
    37,
    24,
    35,
    33,
    30,
    32,
    29,
    32,
    26,
    27,
    28,
    41,
    33,
    44,
    43,
    31,
    43,
    31,
    36,
    39,
    28,
    14,
    15,
    18,
    38,
    108,
    41,
    25,
    24,
    32,
    29,
    32,
    26,
    29,
    45,
    32,
    29,
    23,
    25,
    25,
    24,
    20,
    30,
    24,
    16,
    22,
    21,
    20,
    25,
    21,
    24,
    22,
    26,
    23,
    28,
    25,
    27,
    30,
    38,
    40,
    44,
    37,
    35,
    31,
    30,
    35,
    34,
    35,
    29,
    37,
    27,
    32,
    33,
    43,
    26,
    33,
    18,
    20,
    26,
    23,
    28,
    39,
    44,
    48,
    40,
    40,
    30,
    29,
    38,
    34,
    33,
    34,
    33,
    45,
    33,
    34,
    32,
    43,
    42,
    29,
    24,
    24,
    33,
    28,
    19,
    21,
    21,
    19,
    11,
    23,
    21,
    28,
    28,
    33,
    30,
    29,
    25,
    26,
    30,
    27,
    33,
    32,
    30,
    35,
    28,
    41,
    37,
    24,
    17,
    20,
    24,
    22,
    32,
    22,
    41,
    34,
    44,
    35,
    24,
    29,
    34,
    28,
    35,
    30,
    32,
    24,
    17,
    37,
    52,
    36,
    46,
    55,
    28,
    26,
    30,
    28,
    29,
    28,
    29,
    24,
    43,
    40,
    32,
    31,
    30,
    36,
    30,
    36,
    28,
    28,
    29,
    39,
    31,
    38,
    39,
    45,
    42,
    33,
    26,
    27,
    40,
    29,
    29,
    45,
    34,
    51,
    36,
    38,
    42,
    44,
    41,
    28,
    32,
    33,
    31,
    33,
    34,
    35,
    32,
    31,
    25,
    18,
    28,
    24,
    19,
    27,
    29,
    27,
    27,
    24,
    26,
    29,
    43,
    33,
    22,
    27,
    20,
    30,
    30,
    47,
    18,
    15,
    17,
    14,
    23,
    29,
    26,
    19,
    16,
    23,
    24,
    25,
    14,
    17,
    23,
    18,
    23,
    35,
    40,
    50,
    51,
    65,
    51,
    35,
    33,
    27,
    27,
    27,
    17,
    20,
    29,
    33,
    44,
    44,
    69,
    54,
    57,
    40,
    39,
    35,
    52,
    43,
    52,
    41,
    36,
    47,
    44,
    36,
    46,
    45,
    44,
    58,
    45,
    53,
    56,
    58,
    35,
    32,
    25,
    19,
    20,
    23,
    17,
    22,
    36,
    37,
    53,
    43,
    41,
    48,
    40,
    38,
    25,
    24,
    21,
    29,
    31,
    33,
    17,
    26,
    27,
    26,
    35,
    38,
    33,
    30,
    37,
    27,
    22,
    33,
    24,
    21,
    30,
    35,
    32,
    45,
    27,
    36,
    32,
    38,
    47,
    53,
    61,
    68,
    58,
    55,
    46,
    55,
    53,
    48,
    44,
    36,
    37,
    34,
    39,
    30,
    30,
    31,
    31,
    28,
    32,
    32,
    33,
    34,
    31,
    22,
    31,
    44,
    34,
    46,
    50,
    36,
    30,
    21,
    34,
    18,
    29,
    40,
    27,
    29,
    34,
    37,
    24,
    41,
    41,
    44,
    40,
    32,
    31,
    44,
    33,
    32,
    26,
    30,
    27,
    35,
    37,
    27,
    26,
    29,
    50,
    43,
    31,
    23,
    24,
    26,
    21,
    16,
    15,
    23,
    23,
    31,
    38,
    41,
    56,
    48,
    28,
    42,
    34,
    36,
    43,
    52,
    43,
    40,
    42,
    39,
    31,
    28,
    25,
    21,
    23,
    30,
    31,
    33,
    29,
    33,
    35,
    29,
    19,
    26,
    28,
    25,
    24,
    39,
    42,
    49,
    51,
    70,
    63,
    54,
    61,
    63,
    50,
    64,
    48,
    49,
    45,
    48,
    41,
    66,
    61,
    62,
    49,
    40,
    58,
    48,
    32,
    35,
    33,
    28,
    33,
    34,
    31,
    30,
    29,
    29,
    28,
    29,
    38,
    31,
    34,
    40,
    47,
    30,
    47,
    44,
    42,
    37,
    37,
    38,
    38,
    32,
    35,
    45,
    39,
    47,
    32,
    33,
    36,
    29,
    43,
    44,
    44,
    39,
    49,
    35,
    43,
    31,
    31,
    35,
    32,
    33,
    29,
    31,
    32,
    27,
    31,
    28,
    30,
    34,
    36,
    37,
    35,
    34,
    33,
    35,
    29,
    28,
    28,
    32,
    23,
    29,
    22,
    18,
    20,
    24,
    34,
    23,
    28,
    26,
    21,
    25,
    34,
    33,
    24,
    31,
    40,
    36,
    29,
    32,
    28,
    31,
    45,
    44,
    38,
    46,
    42,
    30,
    33,
    40,
    31,
    23,
    17,
    22,
    20,
    25,
    27,
    25,
    36,
    41,
    56,
    41,
    49,
    63,
    55,
    41,
    29,
    46,
    38,
    44,
    41,
    43,
    32,
    36,
    39,
    42,
    41,
    57,
    45,
    40,
    33,
    36,
    31,
    31,
    32,
    30,
    55,
    58,
    35,
    24,
    27,
    27,
    29,
    47,
    52,
    45,
    18,
    28,
    16,
    21,
    29,
    28,
    32,
    23,
};

// Multiply by 1.2/255 to obtain input voltage
const uint8_t Input[] PROGMEM =
{
    213,
    213,
    213,
    213,
    213,
    213,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    213,
    210,
    210,
    210,
    210,
    210,
    210,
    208,
    213,
    210,
    210,
    210,
    210,
    208,
    210,
    208,
    210,
    208,
    210,
    208,
    210,
    208,
    210,
    208,
    208,
    208,
    210,
    208,
    210,
    208,
    210,
    208,
    208,
    208,
    208,
    208,
    208,
    206,
    208,
    208,
    208,
    206,
    208,
    206,
    208,
    206,
    208,
    206,
    208,
    206,
    208,
    206,
    208,
    206,
    208,
    206,
    208,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    206,
    204,
    204,
    204,
    206,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    204,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    204,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    200,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    202,
    202,
    200,
    202,
    200,
    202,
    202,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    202,
    202,
    200,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    200,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    202,
    202,
    202,
    202,
    200,
    202,
    202,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    202,
    200,
    200,
    200,
    200,
    200,
    200,
    200,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    200,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    198,
    196,
    198,
    196,
    198,
    198,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    198,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    193,
    193,
    193,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    193,
    193,
    193,
    193,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    196,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    193,
    191,
    193,
    193,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    191,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    193,
    191,
    191,
    191,
    193,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    189,
    191,
    189,
    191,
    191,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    191,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    189,
    191,
    191,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    189,
    191,
    191,
    191,
    191,
    191,
    189,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    191,
    189
};

static_assert(sizeof(Seeing) < 0x10000, "SimData::Seeing cannot have more than 2^16=65536 elements");
static_assert(sizeof(Input)  < 0x10000, "SimData::Input cannot have more than 2^16=65536 elements");
static_assert(sizeof(Seeing) == sizeof(Input), "SimData::Input and SimData::Seeing must have the same element count");

void GetNextSeeingAndInputVals(float &seeing, float &input)
{
    uint8_t seeing8b = pgm_read_byte(Seeing + currentIdx),
            input8b  = pgm_read_byte(Input + currentIdx);

    seeing = (float)seeing8b * 10.0/255;
    input = (float)input8b * 1.2/255;

    currentIdx = (currentIdx + 1) % sizeof(SimData::Seeing);
}

}
