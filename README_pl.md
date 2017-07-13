# **Firmware monitora scyntylacji słonecznej**

Copyright (C) 2017 Filip Szczerek <ga.software@yahoo.com>

Niniejszy projekt objęty jest licencją MIT (szczegóły w pliku `LICENSE`).

Oparty na:

  E. J. Seykora
  *An Inexpensive Solar Scintillation Seeing Monitor Circuit with Arduino Interface*

  https://www.ecu.edu/cs-cas/physics/upload/An-Inexpensive-Solar-Scintillation-Seeing-Monitor-Circuit-with-Arduino-Interface-final2.pdf


# Podsumowanie

Monitor scyntylacji słonecznej (*Solar Scintillation Monitor*, *Solar Scintillation Seeing Monitor*) mierzy wartość *seeingu* w sekundach łuku w oparciu o poziom zmian natężenia światła słonecznego. Wyprowadzenie tej zależności można znaleźć w artykule dra Seykory: *Solar Scintillation and the Monitoring of Solar Seeing*.

W niniejszym kodzie, zamiast zbierania po 10 000 próbek uśrednionego i zmiennego napięcia wejściowego, próbkowanie odbywa się przez określony czas (`CALCULATION_PERIOD`) dzięki bibliotece *TimerThree*.

Budowanie: za pomocą frameworku PlatformIO (platformio.org).
