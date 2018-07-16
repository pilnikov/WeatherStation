/*
  HT1632.h - Library for communicating with the popular HT1632/HT1632C
  LED controllers. This library provides higher-level access (including
  text drawing) for these chips. Currently, the library supports writing
  to a single chip at a time, and has been tested with two
  Sure Electronics 3208 5mm red board.

  Created by Gaurav Manek, April 8, 2011.
  Released into the public domain.
*/
#ifndef SURE_HT1632_h
#define SURE_HT1632_h

#include <Arduino.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#else
#define PROGMEM
#endif

// Custom typedefs
typedef unsigned char uint8_t;
typedef unsigned char byte;
// typedef char int8_t;


/*
   USER OPTIONS
   Change these options
*/

#define COM_SIZE 16
#define OUT_SIZE 32
#define NUM_CHANNEL 2
#define USE_NMOS 1
// Number of chips in a single Bicolor board:
#define NUM_ACTIVE_CHIPS 4

/*
   END USER OPTIONS
   Don't edit anything below unless you know what you are doing!
*/


// Pixels in a single byte of the internal image representation:
#define PIXELS_PER_BYTE 8

// Address space size (number of 4-bit words in HT1632 memory)
// Exactly equal to the number of 4-bit address spaces available.
#define ADDR_SPACE_SIZE (COM_SIZE * OUT_SIZE / PIXELS_PER_BYTE)
#define GET_ADDR_FROM_X_Y(_x,_y) ((_x)*((COM_SIZE)/(PIXELS_PER_BYTE))+(_y)/(PIXELS_PER_BYTE))
#define GET_BIT_FROM_Y(_y) ( (0b1 << PIXELS_PER_BYTE-1) >> (_y % PIXELS_PER_BYTE) )

// NO-OP Definition
#define NOP(); __asm__("nop\n\t");
// The HT1632 requires at least 50 ns between the change in data and the rising
// edge of the WR signal. On a 16MHz processor, this provides 62.5ns per NOP.

// Standard command list.
// This list is modified from original code by Bill Westfield

#define HT1632_ID_CMD 0b100	/* ID = 100 - Commands */
#define HT1632_ID_RD  0b110	/* ID = 110 - Read RAM */
#define HT1632_ID_WR  0b101	/* ID = 101 - Write RAM */
#define HT1632_ID_LEN 3         /* IDs are 3 bits */

#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSEN  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-00xx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x14  /* CMD= 0001-01xx-x Master Mode, on-chip clock */
#define HT1632_CMD_RCCLK  0x18  /* CMD= 0001-10xx-x Master Mode, external clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM_T  0xA0	/* CMD= 101x-PPPP-x PWM duty cycle - template*/
#define HT1632_CMD_PWM(lvl) (HT1632_CMD_PWM_T | (lvl-1))
/* Produces the correct command from the given value of lvl. lvl = [0..15] */
#define HT1632_CMD_LEN    8	/* Commands are 8 bits long, excluding the trailing bit */
#define HT1632_ADDR_LEN   7	/* Addresses are 7 bits long */
#define HT1632_WORD_LEN   4     /* Words are 4 bits long */

class HT1632Class : public Adafruit_GFX {
  private:
    uint8_t _pinCS [4];
    uint8_t _numActivePins;
    uint8_t _pinWR;
    uint8_t _pinDATA;
    uint8_t _pinCLK;
    uint8_t _currSelectionMask;
    uint8_t _tgtRender;
    uint8_t _tgtChannel;
    byte * mem [5];
    void writeCommand(char);
    void writeData(byte, uint8_t);
    void writeDataRev(byte, uint8_t);
    void writeSingleBit();
    void select();
    void select(uint8_t mask);
    int getCharWidth(int font_end [], uint8_t font_height, uint8_t font_index);
    int getCharOffset(int font_end [], uint8_t font_index);
    inline void pulseCLK();

  public:
    HT1632Class(uint8_t pinCLK, uint8_t pinCS1, uint8_t pinWR, uint8_t pinDATA);

    void sendCommand(uint8_t command);
    void renderTarget(uint8_t targetScreen);
    void selectChannel(uint8_t channel);
    void render();
    void transition(uint8_t mode, int time = 1000); // Time is in milliseconds.
    void clear();
    void drawImage(const byte img [], uint8_t width, uint8_t height, int8_t x, int8_t y, int offset = 0);
    void drawText(const char text [], int x, int y, const byte font [], int font_end [], uint8_t font_height, uint8_t gutter_space = 1);
    int getTextWidth(const char text [], int font_end [], uint8_t font_height, uint8_t gutter_space = 1);
    void setBrightness(char brightness, char selectionmask = 0b00010000);

    void setPixel(uint8_t x, uint8_t y);
    void clearPixel(uint8_t x, uint8_t y);
    uint8_t getPixel(uint8_t x, uint8_t y);
    void setPixel(uint8_t x, uint8_t y, uint8_t channel);
    void clearPixel(uint8_t x, uint8_t y, uint8_t channel);
    uint8_t getPixel(uint8_t x, uint8_t y, uint8_t channel);
    void drawPixel(int16_t x, int16_t y, uint16_t channel);
    void drawPartChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, int8_t size);
    void fill();
    void fillAll();
};

//extern HT1632Class HT1632;

//#else
//#error "HT1632.h" already defined!
#endif
