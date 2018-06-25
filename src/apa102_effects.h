#ifndef APA102_EFFECTS_H
#define APA102_EFFECTS_H

#include "apa102.h"

/* Set all LEDs to a specific color */
void apa102_set_all(const RGBColor_t color);

/* Flash all LEDs through a sequence of colors with variable color & blank delays. */
void flash(const RGBColor_t *sequence, const uint8_t sequence_length, const uint16_t color_delay, const uint16_t blank_delay);

#endif
