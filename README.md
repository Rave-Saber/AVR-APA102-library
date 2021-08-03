# AVR APA102 Library

[![Build Status](https://github.com/Rave-Saber/AVR-APA102-library/actions/workflows/main.yml/badge.svg?branch=master)](https://github.com/Rave-Saber/AVR-APA102-library)

This is a simple C library for driving a strip of APA102 LEDs using the
hardware SPI on an AVR microcontroller.

The `apa102` module contains the basic functions you need to control LEDS -
initializing the SPI peripherals & sending start, LED, & end frames in the
proper format.

The rest of this library is still in active development & subject to change.

There are functions to render color patterns like flashes, ribbons, scrolling,
extension, & retraction.

The `apa102_simple_effects` module renders colors directly to the LED strip and
contains limited functionality. For example, the extension/retraction effects
only support solid colors.

The `apa102_patterns` module contains more patterns, pattern chaining, & the
ability to define custom patterns. It supports extension & retraction for any
pattern type.


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

Then just include the `src` folder in your compilation process:

    avr-gcc -I /path/to/this/repo/src ...

You can check out the example code which has been tested with both a Sparkfun
Lumenati 8-LED strip & DotStar 144 LED/m strip using an Atmel ATmega168a
microcontroller. To build the hex file, run `make all`. To build & flash the
code using a `USBtinyISP`, run `make flash`.


## Additional Features

There are several optional features that can be enabled by defining macros.

Define the `STARTING_LED` macro to shift the first lit LED down the strip. You
should have at least `LED_COUNT + STARTING_LED - 1` LEDs in your strip.

Define the `CURRENT_LIMIT` macro to limit the amount of current used by the
strip. This exposes scaling functions in `apa102.h` and causes the simple
effects & patterns modules to automatically scale any colors passed to them.


## TODO

* Pattern for rendering images
    * Make a pattern for fixed arrays of colors
    * Script to convert images to xpm, transpose rows/cols, & print array for
      inclusion in patterns file.
* Use timer for delays instead of relying on delay functions?
    * Make it a compilation option in case no free timers? Or method that can
      re-use existing timer with different interrupt/ms rates.
    * Maybe just implement in firmware & example and have library continue to
      return delay amounts.
* Modify extension/retraction delay so it's only used for the LED
  (de)activation rate - respect the pattern step's delay amount for stepping
  through the pattern. I.e., blade size changes with extension delay, colors
  change with pattern delay.
* Allow nesting of `SERIES` patterns.
    * Would make re-using sequence transitions easier
    * Probably need to make series step globals into struct. Pause playback of
      parent series while playing sub-series.
    * Dynamic array to track all series/sub-series structs up to current depth?
* Pattern for rendering text
    * Need to scale font to `LED_COUNT`
    * Option for setting foreground/background colors, maybe color
      sequences or a background `GenericPattern`
* Make `current_sequence` array in `apa102_patterns` available to library
  consumers? Either by exposing the array or functions to modify it.
* Support assigning different patterns to specific LED ranges
    * E.g., rgb ribbon at ends, rgb flash in center
    * Probably requires re-writing patterns module to calculate single LED
      colors based on both step count and LED index. Is that too performance
      intensive?
    * Or pass the array pointer & led length to each pattern instead of using
      the `current_sequence` & `LED_COUNT` globals. That would allow offsetting
      & shrinking patterns.
    * How to handle patterns with different step counts & delays?
* More effects!
    * scrolling effect that fades the color bands instead of repeating them
    * fixed number of scrolling bands
    * static repeating bands with fixed width or count - ribbon & flash
* Add an RGBAColor type that uses the alpha level to scale the RGB channels
  instead of the APA102's "brightness" bits(since those introduce stuttering).


## References

* [Understanding the APA102 "Superled"](https://cpldcpu.wordpress.com/2014/11/30/understanding-the-apa102-superled/)


## License

GPL-3.0
