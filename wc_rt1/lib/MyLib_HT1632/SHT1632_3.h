#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>


#define ID_LEN 3
#define CMD_LEN 8
#define DATA_LEN 8
#define ADDR_LEN 7
#define PANEL_HEADER_BITS (ID_LEN + ADDR_LEN)

#define CS_NONE 0x00 /* None of ht1632c selected */
#define CS_ALL 0xff  /* All of ht1632c selected */

/*
   commands written to the chip consist of a 3 bit "ID", followed by
   either 9 bits of "Command code" or 7 bits of address + 4 bits of data.
*/
enum COMMAND_CODE {
  SYSDIS = 0x00, /* CMD= 0000-0000-x Turn off oscil */
  SYSON = 0x01,  /* CMD= 0000-0001-x Enable system oscil */
  LEDOFF = 0x02, /* CMD= 0000-0010-x Turn off LED duty cycle generator */
  LEDON = 0x03,  /* CMD= 0000-0011-x Turn on LED duty cycle generator */
  BLOFF = 0x08,  /* CMD= 0000-1000-x Blink OFF */
  BLON = 0x09,   /* CMD= 0000-1001-x Blink ON */
  SLVMD = 0x10,  /* CMD= 0001-00xx-x Slave Mode */
  MSTMD = 0x14,  /* CMD= 0001-01xx-x Master Mode */
  RCCLK = 0x18,  /* CMD= 0001-10xx-x Use on-chip clock */
  EXTCLK = 0x1C, /* CMD= 0001-11xx-x Use external clock */
  COMS00 = 0x20, /* CMD= 0010-ABxx-x commons options */
  COMS01 = 0x24, /* CMD= 0010-ABxx-x commons options */
  COMS10 = 0x28, /* CMD= 0010-ABxx-x commons options */
  COMS11 = 0x2C, /* CMD= 0010-ABxx-x commons options */
  PWM = 0xA0,    /* CMD= 101x-PPPP-x PWM duty cycle */
};

enum ID_CODE {
  CMD = 4,
  WR = 5,
  RD = 6,
};

//
// constants
//

#ifdef DEBUG
#define DEBUGF(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUGF(...)
#endif

class HT1632C {
  public:
    HT1632C(const uint8_t clk_pin, const uint8_t cs_pin);
    ~HT1632C();
    void setChipHeight(uint8_t chip_height);
    void setChipWidth(uint8_t chip_width);
    void setChipsPerPanel(uint8_t chips_per_panel);
    void setColors(uint8_t colors);
    void setFrequency(uint32_t frequency);
    void setCsClkDelay(uint8_t delay);
    void setPanelWidth(uint8_t width);
    void setPanelHeight(uint8_t height);

    void sendCmdonInit();
    int  getWidth();
    int  getHeight();
    void ledOn();
    void ledOff();
    void blinkOn();
    void blinkOff();
    void pwm(const uint8_t value);
    void clear();
    void ramSet(byte *in, uint8_t in_size);
    void sendFrame();

    // Plotting functions
    void plot(const int rx, const int ry, const uint8_t color);

  private:
    int  init();
    void updateFramebuffer(const int chip, const int addr,
                           const uint8_t target_bitval,
                           const uint8_t pixel_bitval);
	void setas3208();
	
    uint8_t num_panels = 1;
    uint8_t rotation = 0;
    uint8_t clk_pin;
    uint8_t cs_pin;
    bool isInit = false;

    uint8_t panel_width = 32;
    uint8_t panel_height = 16;
    uint8_t chip_height = 8;
    uint8_t chip_width = 16;
    uint8_t colors = 2;
    uint8_t chips_per_panel = 4;
    uint32_t frequency = 20000000;

    uint16_t width;
    uint8_t height;
    uint8_t num_chips;
    uint8_t color_size;
    uint16_t chip_size;

    void sendCmd(const uint8_t chip, const uint8_t cmd);
    void chipSelect(const uint8_t cs);
    void clkPulse(int num);

    uint8_t* framebuffer;
    inline uint8_t* framebufferPtr(uint8_t chip, uint8_t addr);

    void debugFramebuffer();
};
