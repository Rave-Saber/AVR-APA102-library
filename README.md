# AVR APA102 Library

This is a simple C library for driving a strip of APA102 LEDs using the
hardware SPI on an AVR microcontroller.

The `apa102` module contains the basic functions you need to control LEDS -
initializing the SPI peripherals & sending start, LED, & end frames in the
proper format.

The rest of this library is still in active development & subject to change.

There are functions to render color patterns like flashes, ribbons, scrolling,
extension, & retraction. The `apa102_simple_effects` module renders colors
directly to the LED strip, while the `apa102_patterns` module stores colors in
an array before rendering them.


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

* User-defined pattern functions via callbacks
* Make `current_sequence` array in `apa102_patterns` available to library
  consumers? Either by exposing the array or functions to modify it.
* Support assigning different patterns to specific LED ranges
    * E.g., rgb ribbon at ends, rgb flash in center
    * Probably requires re-writing patterns module to calculate single LED
      colors based on both step count and LED index. Is that too performance
      intensive?
* LED powersaving - light only every Xth LED, or set a maximum current the LEDs
  can use & use that to scale LED duty cycles.
* Color mixing/blending, building gradients between sequence of colors.
* More effects!
    * scrolling effect that widens or fades the color bands instead of repeating them
    * fixed number of scrolling bands
    * static repeating bands with fixed width or count.
* Add an RGBAColor type that uses the alpha level to scale the RGB channels
  instead of the APA102's "brightness" bits(since those introduce stuttering).


# License

GPL-3.0
