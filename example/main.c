#include <avr/power.h>
#include <util/delay.h>
#include <apa102.h>
#include <apa102_simple_effects.h>
#include <apa102_patterns.h>


int main(void) {
    apa102_init_spi();
    clock_prescale_set(clock_div_1);

    /* Simple Effects */
    const RGBColor_t rgb_sequence[3] =
        { rgb(0xff0000)
        , rgb(0x00ff00)
        , rgb(0x0000ff)
        };

    const RGBColor_t rainbow_sequence[7] =
        { rgb(0xff0000)
        , rgb(0xff7f00)
        , rgb(0xffff00)
        , rgb(0x00ff00)
        , rgb(0x0000ff)
        , rgb(0x4b0082)
        , rgb(0x9400d3)
        };


    /* Patterns */
    const SolidArgs_t solid_red =
        rgb(0xff0000);

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
