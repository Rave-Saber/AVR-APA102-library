#include "apa102.h"

extern inline RGBColor_t rgb(const uint32_t color);
extern inline void apa102_init_spi(void);
extern inline void apa102_transmit_byte(uint8_t data);
extern inline void apa102_start(void);
extern inline void apa102_end(void);
extern inline void apa102_set_led(const RGBColor_t color);
