#include <avr/power.h>
#include <apa102.h>
#include <apa102_effects.h>

const RGBColor_t rgb_sequence[3] = { {0xff,0x00,0x00}, {0x00,0xff,0x00}, {0x00,0x00,0xff} };

int main(void) {
    apa102_init_spi();
    clock_prescale_set(clock_div_1);
    while (1) {
        flash(rgb_sequence, 3, 150, 50);
    }
    return 0;

}
