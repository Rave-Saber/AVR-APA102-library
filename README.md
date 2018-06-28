# AVR APA102 Library

This is a simple C library for driving a strip of APA102 LEDs using the
hardware SPI on an AVR microcontroller.

Some helper functions are included in a separate file for simple effects like
single colors, multiple colors, extension, retraction, & scrolling colors.


## Usage

You need to define some macro's to use this library:

* `SCK_DDR`
* `SCK`
* `MOSI_DDR`
* `MOSI_PORT`
* `MOSI`
* `SS_DDR`
* `SS_PORT`
* `SS`
* `LED_COUNT`

These defines should be for the HW SPI pins of your AVR microcontroller. This
library does not support using arbitrary I/O pins.

Then just include the `src` folder in your compilation process:

    avr-gcc -I /path/to/this/repo/src ...

You can check out the example code which has been tested with a Sparkfun
Lumenati 8-LED strip & an Atmel ATmega168a microcontroller. To build the hex
file, run `make`. To build & flash the code using a `USBtinyISP`, run `make
flash`.


## TODO

* Change effects so they generate color arrays for a given step, instead of
  directly outputting each step's colors to SPI. This would allow saving the
  current color array so we can retract and extend any pattern instead of just
  solid colors.
* Allow combining multiple effects as a single pattern - e.g., half of the
  LEDs as a scroll effect and the other as a flash effect.
* Color mixing/blending, building gradients between sequence of colors.
* More effects! E.g., scrolling effect that widens or fades the color bands
  instead of repeating them.
* Add an RGBAColor type that uses the alpha level to scale the RGB channels
  instead of the APA102's "brightness" bits(since those introduce stuttering).


# License

GPL-3.0
