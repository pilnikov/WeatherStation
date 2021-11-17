#include "SHT1632_3.h"

void abortIfInit(bool isInit) {
  if (isInit) {
    Serial.println(F("Display already initialized, unable to set value"));
    abort();
  }
}

HT1632C::HT1632C(const uint8_t clk_pin, const uint8_t cs_pin)
  : clk_pin(clk_pin),
    cs_pin(cs_pin)
{
  init();
}

HT1632C::~HT1632C()
{
  free(framebuffer);
}

void HT1632C::setChipHeight(uint8_t chip_height)
{
  abortIfInit(isInit);
  this->chip_height = chip_height;
}

void HT1632C::setChipWidth(uint8_t chip_width)
{
  abortIfInit(isInit);
  this->chip_width = chip_width;
}

void HT1632C::setChipsPerPanel(uint8_t chips_per_panel)
{
  abortIfInit(isInit);
  this->chips_per_panel = chips_per_panel;
}

void HT1632C::setColors(uint8_t colors)
{
  abortIfInit(isInit);
  this->colors = colors;
}

void HT1632C::setFrequency(uint32_t frequency)
{
  abortIfInit(isInit);
  this->frequency = frequency;
}

void HT1632C::setPanelWidth(uint8_t width)
{
  abortIfInit(isInit);
  this->panel_width = width;
}

void HT1632C::setPanelHeight(uint8_t height)
{
  abortIfInit(isInit);
  this->panel_height = height;
}

int HT1632C::init()
{
  width = panel_width * num_panels;
  height = panel_height;
  color_size = chip_width * chip_height / 8; // 16 * 8 / 8 = 16
  chip_size = (color_size * colors) + 2; // 16 * 2 = 32
  num_chips = chips_per_panel * num_panels; // 4 * 1 = 4
  DEBUGF("Chip Size: %d\n", chip_size);
  framebuffer = (uint8_t*)malloc(num_chips * chip_size); //4 * 32 = 128
  if (!framebuffer) return 3;

//  SPI.begin();
# if defined(__xtensa__)
  SPI.setFrequency(frequency);
#endif

  pinMode(clk_pin, OUTPUT);
  pinMode(cs_pin, OUTPUT);
 
 // init display
 
  sendCmd(CS_ALL, COMMAND_CODE::SYSDIS);
  sendCmd(CS_ALL, COMMAND_CODE::COMS00);
  sendCmd(CS_ALL, COMMAND_CODE::MSTMD);
  sendCmd(CS_ALL, COMMAND_CODE::RCCLK);
  sendCmd(CS_ALL, COMMAND_CODE::SYSON);
  sendCmd(CS_ALL, COMMAND_CODE::LEDON);

  clear();
  sendFrame();
  return 0;
}


int HT1632C::getWidth()
{
  return (rotation & 1) ? height : width;
}

int HT1632C::getHeight()
{
  return (rotation & 1) ? width : height;
}

void HT1632C::ledOn()
{
  sendCmd(CS_ALL, COMMAND_CODE::LEDON);
}

void HT1632C::ledOff()
{
  sendCmd(CS_ALL, COMMAND_CODE::LEDOFF);
}

void HT1632C::blinkOn()
{
  sendCmd(CS_ALL, COMMAND_CODE::BLON);
}

void HT1632C::blinkOff()
{
  sendCmd(CS_ALL, COMMAND_CODE::BLOFF);
}

void HT1632C::pwm(const uint8_t value)
{
  sendCmd(CS_ALL, COMMAND_CODE::PWM | (value & 0x0f));
}

void HT1632C::clear()
{
  // clear buffer
  memset(framebuffer, 0, num_chips * chip_size);
  for (int i = 0; i < num_chips; ++i)
  {
    *framebufferPtr(i, 0) = ID_CODE::WR << (8 - ID_LEN);
  }
}

void HT1632C::ramSet(byte *in, uint8_t in_size)
{
  uint8_t _size = num_chips * chip_size;
  if (in_size <= _size) _size = in_size;
  memcpy(framebuffer, in, _size);
  for (int i = 0; i < num_chips; ++i)
  {
    *framebufferPtr(i, 0) = ID_CODE::WR << (8 - ID_LEN);
  }
}


void HT1632C::sendFrame()
{
  for (int chip = 1; chip <= num_chips; ++chip) {
	chipSelect(chip);
    for (uint8_t addr = 0; addr < chip_size; addr++)
    # if defined(__xtensa__)
		SPI.write(*framebufferPtr(chip - 1, addr));
	# endif
    chipSelect(CS_NONE);
	yield();
  }
}

void HT1632C::plot(const int rx, const int ry, const uint8_t color)
{
  const int xc = rx / chip_width;
  const int yc = ry / chip_height;
  const int chip = xc + (xc & 0xfffe) + (yc * 2);

  const int xb = (rx % chip_width) + (chip_height / 8) - 1;
  const int yb = (ry % chip_height) + PANEL_HEADER_BITS;
  int addr = xb + (yb / 8);
  const uint8_t bitval = 128 >> (yb & 7);

  // first color
  updateFramebuffer(chip, addr, (color & 1), bitval);
  if (addr <= 1 && bitval > 2) {
    // special case: first bits must are 'wrapped' to the end
    updateFramebuffer(chip, addr + chip_size - 2, (color & 1), bitval);
  }
  // other colors
  for (int i = 1; i < colors; ++i) {
    addr += color_size;
    updateFramebuffer(chip, addr, (color & (1 << i)), bitval);
  }
}

void HT1632C::sendCmd(const uint8_t chip, const uint8_t cmd)
{
  chipSelect(chip);
  # if defined(__xtensa__)
  uint16_t data = ((ID_CODE::CMD << 8) | cmd) << 5;
  SPI.write16(data);
  # endif 
  chipSelect(CS_NONE);
}

void HT1632C::chipSelect(const uint8_t cs)
{
  switch (cs)
  {
    case CS_ALL:
      digitalWrite(cs_pin, LOW);
      clkPulse(num_chips);
      break;
    case CS_NONE:
      digitalWrite(cs_pin, HIGH);
      clkPulse(num_chips);
      break;
    default:
      digitalWrite(cs_pin, LOW);
      clkPulse(1);
      digitalWrite(cs_pin, HIGH);
      clkPulse(cs - 1);
      break;
  }
}

void HT1632C::clkPulse(int num)
{
  while (num--) 
  {
    digitalWrite(clk_pin, HIGH);
	digitalWrite(clk_pin, LOW);
  }
}

void HT1632C::updateFramebuffer(const int chip, const int addr,
                                const uint8_t target_bitval,
                                const uint8_t pixel_bitval)
{
  uint8_t* const v = framebufferPtr(chip, addr);
  if (target_bitval)
    *v |= pixel_bitval;
  else
    *v &= ~pixel_bitval;
}

uint8_t* HT1632C::framebufferPtr(uint8_t chip, uint8_t addr)
{
  return framebuffer + (chip * chip_size + addr);
}

void HT1632C::debugFramebuffer()
{
  for (int chip = 0; chip < num_chips; chip++) {
    DEBUGF("Chip %d: ", chip);
    uint8_t* ptr = framebufferPtr(chip, 0);
    for (uint8_t i = 0; i < chip_size; i++) {
      Serial.print(*(ptr + i), HEX);
      if (i != chip_size - 1) {
        DEBUGF(",");
      }
    }
    DEBUGF("\n");
  }
}
