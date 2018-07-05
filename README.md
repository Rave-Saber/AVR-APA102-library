# AVR APA102 Library

This is a simple C library for driving a strip of APA102 LEDs using the
hardware SPI on an AVR microcontroller.

Some helper functions are included in a separate file for simple effects like
single colors, multiple colors, extension, retraction, & scrolling colors.

The rest of this library is still in active development & subject to change.
Eventually we'd like to support:
* Extension/retraction while playing effects
* Assigning different patterns to different LED ranges
* User-defined pattern playback


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

The pin defines should be for the HW SPI pins of your AVR microcontroller. This
library does not support using arbitrary I/O pins.

You can optionally define `STARTING_LED` macro to shift the first lit LED down
the strip. You should have at least `LED_COUNT + STARTING_LED - 1` LEDs in your
strip.

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
* LED powersaving - light only every Xth LED, or set a maximum current the LEDs
  can use & use that to scale LED duty cycles.
* Allow combining multiple effects as a single pattern - e.g., half of the
  LEDs as a scroll effect and the other as a flash effect.
* Color mixing/blending, building gradients between sequence of colors.
* More effects!
    * scrolling effect that widens or fades the color bands instead of repeating them
    * fixed number of scrolling bands
    * static repeating bands with fixed width or count.
* Add an RGBAColor type that uses the alpha level to scale the RGB channels
  instead of the APA102's "brightness" bits(since those introduce stuttering).


# License

GPL-3.0
