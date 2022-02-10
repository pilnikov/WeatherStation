
#include <SPI.h>

#include "MAX72xx.h"

static MAX72XX *m7219;

#include "Max72.h"

static Max72 * m7219_1;

void m7219_init();
void m7219_ramFormer2(byte *, uint8_t, uint8_t);

static byte screen[64]; // display buffer
