#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <SPI.h>
#include <inttypes.h>


#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

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

class HT1632C {
  public:
    HT1632C(const uint8_t clk_pin, const uint8_t cs_pin);
    ~HT1632C();
  
    inline void setChipHeight(uint8_t chip_height)
    {
      abortIfInit(isInit);
      this->chip_height = chip_height;
    }

    inline void setChipWidth(uint8_t chip_width)
    {
      abortIfInit(isInit);
      this->chip_width = chip_width;
    }

    inline void setChipsPerPanel(uint8_t chips_per_panel)
    {
      abortIfInit(isInit);
      this->chips_per_panel = chips_per_panel;
    }

    inline void setColors(uint8_t colors)
    {
      abortIfInit(isInit);
      this->colors = colors;
    }

    inline void setPanelWidth(uint8_t width)
    {
      abortIfInit(isInit);
      this->panel_width = width;
    }

    inline void setPanelHeight(uint8_t height)
    {
      abortIfInit(isInit);
      this->panel_height = height;
    }

    inline int getWidth()
    {
      return (rotation & 1) ? height : width;
    }

    inline int getHeight()
    {
      return (rotation & 1) ? width : height;
    }

    inline void ledOn()
    {
      sendCmd(CS_ALL, COMMAND_CODE::LEDON);
    }

    inline void ledOff()
    {
      sendCmd(CS_ALL, COMMAND_CODE::LEDOFF);
    }

    inline void blinkOn()
    {
      sendCmd(CS_ALL, COMMAND_CODE::BLON);
    }

    inline void blinkOff()
    {
      sendCmd(CS_ALL, COMMAND_CODE::BLOFF);
    }

    inline void pwm(const uint8_t value)
    {
      sendCmd(CS_ALL, COMMAND_CODE::PWM | (value & 0x0f));
    }

    inline void clear()
    {
      // clear buffer
      memset(framebuffer, 0, num_chips * chip_size);
      for (int i = 0; i < num_chips; ++i)
      {
        *framebufferPtr(i, 0) = ID_CODE::WR << (8 - ID_LEN);
      }
    }

    void ramSet(byte *in, uint8_t in_size);
    void sendFrame();

    // Plotting functions
    void plot(const int rx, const int ry, const uint8_t color);

  private:
    
    uint8_t* framebuffer;
 
    inline void abortIfInit(bool isInit) 
    {
      if (isInit) 
      {
        DBG_OUT_PORT.println(F("Display already initialized, unable to set value"));
        abort();
      }
    }

    int  init();
    
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
   
    uint16_t width;
    uint8_t height;
    uint8_t num_chips;
    uint8_t color_size;
    uint16_t chip_size;


    void sendCmd(const uint8_t chip, const uint8_t cmd);
    void chipSelect(const uint8_t cs);

    inline void clkPulse(int num)
    {
      while (num--) 
      {
        digitalWrite(clk_pin, HIGH);
        digitalWrite(clk_pin, LOW);
      }
    }

    inline void updateFramebuffer(const int chip, const int addr,
                                    const uint8_t target_bitval,
                                    const uint8_t pixel_bitval)
    {
      uint8_t* const v = framebufferPtr(chip, addr);
      target_bitval ? *v |= pixel_bitval : *v &= ~pixel_bitval;
    }

    inline uint8_t* framebufferPtr(uint8_t chip, uint8_t addr)
    {
      return framebuffer + (chip * chip_size + addr);
    }

    void debugFramebuffer();
};
