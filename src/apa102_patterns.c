#include <stdio.h>
#include "apa102_patterns.h"

uint8_t current_pattern_step = 0;
uint8_t total_pattern_steps = 0;

// The current color sequence for the LED strip.
static RGBColor_t current_sequence[LED_COUNT];

extern inline void variable_delay(uint16_t ms_delay);
extern inline void increment_current_step(void);

static inline void set_all(const RGBColor_t color);


/* Pattern Playback */

void initialize_pattern(const GenericPattern_t *pattern_data) {
    current_pattern_step = 0;
    const void *type_args = pattern_data->pattern_type_args;
    switch (pattern_data->pattern_type) {
        case SOLID: {
            total_pattern_steps = solid_step_count();
            break;
        }
        case FLASH: {
            FlashArgs_t args = *((FlashArgs_t *) type_args);
            total_pattern_steps = flash_step_count(args.length);
            break;
        }
        case RIBBON: {
            RibbonArgs_t args = *((RibbonArgs_t *) type_args);
            total_pattern_steps = ribbon_step_count(args.length);
            break;
        }
        case SCROLL: {
            ScrollArgs_t args = *((ScrollArgs_t *) type_args);
            total_pattern_steps = scroll_step_count(args.length);
            break;
        }
        case WIDE_SCROLL: {
            total_pattern_steps = wide_scroll_step_count();
            break;
        }
    }
}

uint16_t update_sequence(const GenericPattern_t *pattern_data) {
    const void *type_args = pattern_data->pattern_type_args;
    uint16_t delay = 0;
    switch (pattern_data->pattern_type) {
        case SOLID:
            delay = solid_set_sequence((SolidArgs_t *) type_args);
            break;
        case FLASH:
            delay = flash_set_sequence((FlashArgs_t *) type_args);
            break;
        case RIBBON:
            delay = ribbon_set_sequence((RibbonArgs_t *) type_args);
            break;
        case SCROLL:
            delay = scroll_set_sequence((ScrollArgs_t *) type_args);
            break;
        case WIDE_SCROLL:
            delay = wide_scroll_set_sequence((WideScrollArgs_t *) type_args);
            break;
    }
    return delay;
}

void output_current_sequence(void) {
    apa102_start();
#ifdef STARTING_LED
    shift_starting_led();
#endif
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        apa102_set_led(*(current_sequence + i));
    }
    apa102_end();
}



/* Pattern Steppers */


/* SOLID */
// solid is just a single color
uint8_t solid_step_count(void) {
    return 1;
}
uint16_t solid_set_sequence(const SolidArgs_t *args) {
    set_all(*args);
    return 0;
}


/* FLASH */
// flash shows a color & a blank for each color in the sequence.
uint8_t flash_step_count(const uint8_t sequence_len) {
    return sequence_len * 2;
}
uint16_t flash_set_sequence(const FlashArgs_t *args) {
    if ((current_pattern_step % 2) == 1) {
        set_all(rgb(0x000000));
        return args->blank_delay;
    } else {
        set_all(*(args->sequence + (current_pattern_step / 2)));
        return args->color_delay;
    }
}


/* RIBBON */
// ribbon just shows each color in the sequence with no blank gap
uint8_t ribbon_step_count(const uint8_t sequence_len) {
    return sequence_len;
}
uint16_t ribbon_set_sequence(const RibbonArgs_t *args) {
    set_all(*(args->sequence + current_pattern_step));
    return args->delay;
}


/* SCROLL */
// scroll constantly shifts the sequence up or down.
uint8_t scroll_step_count(const uint8_t sequence_len) {
    return sequence_len;
}

uint16_t scroll_set_sequence(const ScrollArgs_t *args) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        uint8_t sequence_offset = i + current_pattern_step;
        if (!(args->reverse)) {
            sequence_offset = args->length - sequence_offset;
        } else {
        }
        sequence_offset %= args->length;
        *(current_sequence + i) = *(args->sequence + sequence_offset);
    }
    return args->delay;
}


/* WIDE SCROLL */
// scroll, but widen each color instead of repeating the sequence.
uint8_t wide_scroll_step_count(void) {
    return LED_COUNT;
}

uint16_t wide_scroll_set_sequence(const WideScrollArgs_t *args) {
    uint8_t short_band_width = LED_COUNT / args->length;
    uint8_t long_bands = LED_COUNT % args->length;
    for (uint8_t band = 0; band < args->length; band++) {
        uint8_t band_start;
        uint8_t width;
        if (band < long_bands) {
            band_start = band * (short_band_width + 1);
            width = short_band_width + 1;
        } else {
            band_start = (short_band_width + 1) * long_bands
                + short_band_width * (band - long_bands);
            width = short_band_width;
        }
        for (uint8_t band_led = 0; band_led < width; band_led++) {
            uint8_t led_offset = (band_start + band_led + current_pattern_step) % LED_COUNT;
            *(current_sequence + led_offset) = *(args->sequence + band);
        }
    }
    return args->delay;
}


/* EXTEND & RETRACT */
// extend/retract output directly to hardware SPI
// TODO: if extend/retract triggered when sequence is all blank, increment
// until not so, then run action.
// TODO: alternatively, keep running pattern while turning LEDs on/off.
void extend_current_sequence(uint16_t delay) {
    uint8_t current_level = 0;
    while (current_level <= LED_COUNT) {
        apa102_start();
#ifdef STARTING_LED
        shift_starting_led();
#endif
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            if (i + 1 <= current_level) {
                apa102_set_led(*(current_sequence + i));
            } else {
                apa102_set_led(rgb(0x000000));
            }
        }
        apa102_end();
        variable_delay(delay);
        current_level++;
    }
}

void retract_current_sequence(uint16_t delay) {
    uint8_t current_level = LED_COUNT;
    while (current_level != 255) {
        apa102_start();
#ifdef STARTING_LED
        shift_starting_led();
#endif
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            if (i + 1 <= current_level) {
                apa102_set_led(*(current_sequence + i));
            } else {
                apa102_set_led(rgb(0x000000));
            }
        }
        apa102_end();
        variable_delay(delay);
        current_level--;
    }
}



/* Helper Functions */
// Set all the LEDs to the given color.
static inline void set_all(const RGBColor_t color) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        *(current_sequence + i) = color;
    }
}