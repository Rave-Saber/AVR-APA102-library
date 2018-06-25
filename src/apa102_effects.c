#include <util/delay.h>
#include <apa102.h>
#include <apa102_effects.h>

static inline void variable_delay(uint16_t ms_delay) {
    for (uint16_t i = 0; i < ms_delay; i++) {
        _delay_ms(1);
    }
}

void apa102_set_all(const RGBColor_t color) {
    apa102_start();
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        apa102_set_led(color);
    }
    apa102_end();
}

void flash(const RGBColor_t *sequence, const uint8_t sequence_length, const uint16_t color_delay, const uint16_t blank_delay) {
    for (uint8_t i = 0; i < sequence_length; i++) {
        apa102_set_all(*(sequence + i));
        variable_delay(color_delay);
        apa102_set_all(rgb(0x000000));
        variable_delay(blank_delay);
    }
}
