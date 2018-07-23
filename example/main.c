#include <avr/power.h>
#include <util/delay.h>
#include <apa102.h>
#include <apa102_simple_effects.h>
#include <apa102_patterns.h>

/* Base Colors */
#define RED { 0x7f, 0x00, 0x00 }
#define GREEN { 0x00, 0x7f, 0x00 }
#define BLUE { 0x00, 0x00, 0x7f }
#define ORANGE { 0x7f, 0x20, 0x00 }
#define YELLOW { 0x7f, 0x7f, 0x00 }
#define INDIGO { 0x25, 0x00, 0x41 }
#define VIOLET { 0x4a, 0x00, 0x69 }

/* Red->Green->Blue Sequence */
static const RGBColor_t rgb_sequence[3] =
    { RED, GREEN, BLUE };

/* ROYGBIV Sequence */
static const RGBColor_t rainbow_sequence[7] =
    { RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET };

/* Series of Patterns */
/* This series starts with a RGB flash, slowly decreases the blank time until
 * it hits a ribbon pattern, which slowly grows the band width. The series
 * finishes by reversing - shrinking the ribbons and increasing the blank time.
 */
uint8_t rgb_spread_series_steps(void) {
    return 10 + 20 + 10;
}
GenericPattern_t rgb_spread_series_patterns(void) {
    static FlashArgs_t flash_args =
        { .sequence = rgb_sequence, .length = 3, .color_delay = 200, .blank_delay = 200 };
    static RibbonArgs_t ribbon_args =
        { .sequence = rgb_sequence, .length = 3, .delay = 200 };
    if (current_series_step < 10) {
        flash_args.blank_delay = 200 - (20 * current_series_step);
        GenericPattern_t current_pattern = FLASH_PATTERN(flash_args);
        return current_pattern;
    } else if (current_series_step < 20) {
        ribbon_args.delay = 200 + 20 * (current_series_step - 10);
        GenericPattern_t current_pattern = RIBBON_PATTERN(ribbon_args);
        return current_pattern;
    } else if (current_series_step < 30) {
        ribbon_args.delay = 400 - 20 * (current_series_step - 20);
        GenericPattern_t current_pattern = RIBBON_PATTERN(ribbon_args);
        return current_pattern;
    } else {
        flash_args.blank_delay = 20 * (current_series_step - 30);
        GenericPattern_t current_pattern = FLASH_PATTERN(flash_args);
        return current_pattern;
    }
}
static const SeriesArgs_t rgb_spread_series =
    { .total_series_steps_function = rgb_spread_series_steps, .get_pattern_for_step = rgb_spread_series_patterns };
/* This series starts as an RGB wide scroll, slowly adding colors until it
 * becomes a Rainbow wide scroll, then slowly removing colors until it becomes
 * RGB again.
 */
static RGBColor_t rgb_rainbow_sequence[7];
uint8_t rgb_rainbow_scroll_steps(void) {
    return 8;
}
GenericPattern_t rgb_rainbow_scroll_patterns(void) {
    static WideScrollArgs_t args =
        { .delay = 120, .sequence = rgb_rainbow_sequence, .length = 7 };
    switch (current_series_step) {
        case 0:
            rgb_rainbow_sequence[0] = (RGBColor_t) RED;
            rgb_rainbow_sequence[1] = (RGBColor_t) GREEN;
            rgb_rainbow_sequence[2] = (RGBColor_t) BLUE;
            args.length = 3;
            break;
        case 1: case 7:{
            rgb_rainbow_sequence[0] = (RGBColor_t) RED;
            rgb_rainbow_sequence[1] = (RGBColor_t) GREEN;
            rgb_rainbow_sequence[2] = (RGBColor_t) BLUE;
            rgb_rainbow_sequence[3] = (RGBColor_t) VIOLET;
            args.length = 4;
            break;
        }
        case 2: case 6: {
            rgb_rainbow_sequence[0] = (RGBColor_t) RED;
            rgb_rainbow_sequence[1] = (RGBColor_t) YELLOW;
            rgb_rainbow_sequence[2] = (RGBColor_t) GREEN;
            rgb_rainbow_sequence[3] = (RGBColor_t) BLUE;
            rgb_rainbow_sequence[4] = (RGBColor_t) VIOLET;
            args.length = 5;
            break;
        }
        case 3: case 5: {
            rgb_rainbow_sequence[0] = (RGBColor_t) RED;
            rgb_rainbow_sequence[1] = (RGBColor_t) YELLOW;
            rgb_rainbow_sequence[2] = (RGBColor_t) GREEN;
            rgb_rainbow_sequence[3] = (RGBColor_t) BLUE;
            rgb_rainbow_sequence[4] = (RGBColor_t) INDIGO;
            rgb_rainbow_sequence[5] = (RGBColor_t) VIOLET;
            args.length = 6;
            break;
        }
        case 4: default:
            rgb_rainbow_sequence[0] = (RGBColor_t) RED;
            rgb_rainbow_sequence[1] = (RGBColor_t) ORANGE;
            rgb_rainbow_sequence[2] = (RGBColor_t) YELLOW;
            rgb_rainbow_sequence[3] = (RGBColor_t) GREEN;
            rgb_rainbow_sequence[4] = (RGBColor_t) BLUE;
            rgb_rainbow_sequence[5] = (RGBColor_t) INDIGO;
            rgb_rainbow_sequence[6] = (RGBColor_t) VIOLET;
            args.length = 7;
            break;
    }
    GenericPattern_t pattern = WIDE_SCROLL_PATTERN(args);
    return pattern;
}
static const SeriesArgs_t rgb_rainbow_scroll_series =
    { .total_series_steps_function = rgb_rainbow_scroll_steps, .get_pattern_for_step = rgb_rainbow_scroll_patterns };


int main(void) {
    apa102_init_spi();
    clock_prescale_set(clock_div_1);

    /* Patterns */
    const SolidArgs_t solid_red =
        RED;

    const WideScrollArgs_t rainbow_wide_scroll =
        { .sequence = rainbow_sequence, .length = 7, .delay = 200 };

    const FlashArgs_t rainbow_flash =
        { .sequence = rainbow_sequence, .length = 7, .color_delay = 250, .blank_delay = 125 };

    const ScrollArgs_t rgb_scroll =
        { .sequence = rgb_sequence, .length = 3, .reverse = false, .delay = 225 };

    const RibbonArgs_t rainbow_ribbon =
        { .sequence = rainbow_sequence, .length = 7, .delay = 200 };

    const GenericPattern_t patterns[] = {
        SOLID_PATTERN(solid_red),
        WIDE_SCROLL_PATTERN(rainbow_wide_scroll),
        FLASH_PATTERN(rainbow_flash),
        SCROLL_PATTERN(rgb_scroll),
        RIBBON_PATTERN(rainbow_ribbon),
        SERIES_PATTERN(rgb_spread_series),
        SERIES_PATTERN(rgb_rainbow_scroll_series),
    };
    const uint8_t pattern_count = (sizeof(patterns)) / (sizeof(patterns[0]));


    while (1) {
        /* Simple Effects */
        extend(rgb(0xff0000), 500);
        for (uint8_t i = 0; i < 10; i++) {
            flash(rgb_sequence, 3, 150, 150 - (15 * i));
        }
        for (uint8_t i = 0; i < 20; i++) {
            ribbon(rgb_sequence, 3, 20 * (i + 1));
        }
        for (uint8_t i = 0; i < 10; i++) {
            flash(rgb_sequence, 3, 150, 15 * i);
        }
        for (uint8_t i = 0; i < 10; i++) {
            scroll(rainbow_sequence, 7, false, 200);
        }
        retract(rgb(0xff0000), 500);


        /* Patterns */
        initialize_pattern(patterns);
        extend_pattern(patterns, 500);
        for (uint8_t pattern_index = 0; pattern_index < pattern_count; pattern_index++) {
            if (pattern_index != 0) {
                initialize_pattern(patterns + pattern_index);
            }
            for (uint8_t i = 0; i < 5; i++) {
                if (playing_pattern_series) {
                    // Play through the first pattern
                    for (uint8_t s = current_pattern_step; s < total_pattern_steps; s++) {
                        run_step(patterns + pattern_index);
                    }
                    // Play through until the series is complete
                    while (current_series_step != 0) {
                        run_step(patterns + pattern_index);
                    }
                } else if (total_pattern_steps > 1) {
                    // Play the pattern
                    for (uint8_t s = current_pattern_step; s < total_pattern_steps; s++) {
                        run_step(patterns + pattern_index);
                    }
                } else {
                    // Only one step so we can just wait
                    _delay_ms(1000);
                }
            }
        }
        retract_pattern(patterns + pattern_count - 1, 500);


        _delay_ms(500);
    }
    return 0;

}
