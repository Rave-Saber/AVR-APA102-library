#include <avr/power.h>
#include <util/delay.h>
#include <apa102.h>
#include <apa102_effects.h>


int main(void) {
    apa102_init_spi();
    clock_prescale_set(clock_div_1);

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

    while (1) {
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
        _delay_ms(500);
    }
    return 0;

}
