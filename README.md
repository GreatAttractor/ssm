# **Solar Scintillation Monitor firmware**

Copyright (C) 2017 Filip Szczerek <ga.software@yahoo.com>

This project is licensed under the terms of the MIT license (see the `LICENSE` file for details).

Based on:

  E. J. Seykora
  *An Inexpensive Solar Scintillation Seeing Monitor Circuit with Arduino Interface*

  https://www.ecu.edu/cs-cas/physics/upload/An-Inexpensive-Solar-Scintillation-Seeing-Monitor-Circuit-with-Arduino-Interface-final2.pdf


# Overview

Solar Scintillation Monitor (Solar Scintillation Seeing Monitor) measures the *seeing* value in arc seconds by monitoring the variability of Sun’s light intensity. Detailed derivation can be found in Dr. Seykora’s article *Solar Scintillation and the Monitoring of Solar Seeing*.

In the following code, rather than always taking 10 000 samples of the averaged and varying input, the *TimerThree* library is used to perform sampling over a specified duration (`CALCULATION_PERIOD`).

Building tested with PlatformIO framework (platformio.org).
