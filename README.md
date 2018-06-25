# AVR APA102 Library

This is a simple C library for driving a strip of APA102 LEDs using the
hardware SPI on an AVR microcontroller.

Some helper functions are included in a separate file for simple effects like
single colors, multiple colors, extension, retraction, & scrolling colors.


## Usage

You need to define some macro's to use this library:

* TODO: add macros & meanings

Then just include this library in your compilation process:

* TODO: add example compile command

You can check out the example code which has been tested with a Sparkfun
Lumenati 8-LED strip & an Atmel ATmega168a microcontroller.

* TODO: Add example app with makefile

## TODO

* Add a RGBA type that uses the alpha level to scale the RGB channels instead
  of the APA102's "brightness" bits.


# License

GPL-3.0
