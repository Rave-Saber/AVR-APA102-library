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

Then just include the `src` folder in your compilation process:

    avr-gcc -I /path/to/repo/src ...

You can check out the example code which has been tested with a Sparkfun
Lumenati 8-LED strip & an Atmel ATmega168a microcontroller. To build the hex
file, run `make`. To flash the code using a `USBtinyISP`, run `make flash`.


## TODO

* Add an RGBAColor type that uses the alpha level to scale the RGB channels
  instead of the APA102's "brightness" bits(since those introduce stuttering).
* Color mixing/blending, building gradients between sequence of colors.
* More effect functions.


# License

GPL-3.0
