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
                run_step(patterns + pattern_index);
                if (total_pattern_steps > 1) {
                    while (current_pattern_step != 0) {
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
