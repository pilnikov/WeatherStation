/*

 * **************Common*********************

*/

#include <TimeLib.h>

/*
 * **************RTC*********************

*/
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

unsigned long t_start = millis(), t_dsec = millis(), irq_end[2];
bool colon;
uint8_t disp_mode;

bool si7021_present = false, //SI7021 is present
     am2320_present = false, //AM2320 is present
     bmp180_present = false, //BMP180 is present
     bmp280_present = false, //BMP280 is present
     bme280_present = false, //BME280 is present
     bh1750_present = false, //BH1750 is present
     ds3231_present = true;  //DS3231 is present

int sh[3] = {0, 0, 0}, st[3] = { -99, -99, -99}, p1 = 700;

typedef struct
{
  byte dig[4];
} scr_buff_t;

typedef struct
{
  float t = -99, h = -99, p = 700 ;
} snr_para_t;

typedef struct
{
  uint8_t ch[3];
} snr_ch_t;

snr_ch_t sens = {3, 5, 9};




/*

 * ******************* Sensor*********************

*/

#include <Adafruit_Sensor.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <DHT_U.h>

#define SEALEVELPRESSURE_HPA (1013.25)


//----------------------------------------------------------dht


#define DHTPIN            6         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//----------------------------------------------------------htu
Adafruit_HTU21DF sht = Adafruit_HTU21DF();

//----------------------------------------------------------bmp2
Adafruit_BMP280 bmp2; // I2C

//----------------------------------------------------------bme

Adafruit_BME280 bme; // I2C

snr_para_t sht_read();
snr_para_t bmp280_read();
snr_para_t bme280_read();
void  dht_init();
snr_para_t dht_read();


snr_ch_t snr_init(snr_ch_t);
void read_snr(snr_ch_t);


/*
 * ***************************display****************************
*/
const byte charArray[] PROGMEM = {
  //GFEDCBA  Segments      7-segment map:
  0b0111111, // 0   "0"          AAA
  0b0000110, // 1   "1"         F   B
  0b1011011, // 2   "2"         F   B
  0b1001111, // 3   "3"          GGG
  0b1100110, // 4   "4"         E   C
  0b1101101, // 5   "5"         E   C
  0b1111101, // 6   "6"          DDD
  0b0000111, // 7   "7"
  0b1111111, // 8   "8"
  0b1101111, // 9   "9"
  0b1110111, // 10  "A"
  0b1111100, // 11  "b"
  0b0111001, // 12  "C"
  0b1011110, // 13  "d"
  0b1111001, // 14  "E"
  0b1110001, // 15  "F"
  0b0000000, // 16  NO DISPLAY
  0b0000000, // 17  NO DISPLAY
  0b0000000, // 18  NO DISPLAY
  0b0000000, // 19  NO DISPLAY
  0b0000000, // 20  NO DISPLAY
  0b0000000, // 21  NO DISPLAY
  0b0000000, // 22  NO DISPLAY
  0b0000000, // 23  NO DISPLAY
  0b0000000, // 24  NO DISPLAY
  0b0000000, // 25  NO DISPLAY
  0b0000000, // 26  NO DISPLAY
  0b0000000, // 27  NO DISPLAY
  0b0000000, // 28  NO DISPLAY
  0b0000000, // 29  NO DISPLAY
  0b0000000, // 30  NO DISPLAY
  0b0000000, // 31  NO DISPLAY
  0b0000000, // 32  ' '
  0b0000000, // 33  '!'  NO DISPLAY
  0b0100010, // 34  '"'
  0b0000000, // 35  '#'  NO DISPLAY
  0b0000000, // 36  '$'  NO DISPLAY
  0b0000000, // 37  '%'  NO DISPLAY
  0b0000000, // 38  '&'  NO DISPLAY
  0b0000010, // 39  '''
  0b0111001, // 40  '('
  0b0001111, // 41  ')'
  0b0000000, // 42  '*'  NO DISPLAY
  0b0000000, // 43  '+'  NO DISPLAY
  0b0010000, // 44  ','
  0b1000000, // 45  '-'
  0b10000000,// 46  '.'
  0b0000000, // 47  '/'  NO DISPLAY
  0b0111111, // 48  "0"
  0b0000110, // 49  "1"
  0b1011011, // 50  "2"
  0b1001111, // 51  "3"
  0b1100110, // 52  "4"
  0b1101101, // 53  "5"
  0b1111101, // 54  "6"
  0b0000111, // 55  "7"
  0b1111111, // 56  "8"
  0b1101111, // 57  "9"
  0b1000001, // 58  ':'  NO DISPLAY
  0b1001000, // 59  ';'  NO DISPLAY
  0b1000110, // 60  '<'  NO DISPLAY
  0b0000000, // 61  '='  NO DISPLAY
  0b1110000, // 62  '>'  NO DISPLAY
  0b0000000, // 63  '?'  NO DISPLAY
  0b0000000, // 64  '@'  NO DISPLAY
  0b1110111, // 65  "A"
  0b1111100, // 66  "b"
  0b0111001, // 67  "C"
  0b1011110, // 68  "d"
  0b1111001, // 69  "E"
  0b1110001, // 70  "F"
  0b1101111, // 71  'G'
  0b1110110, // 72  'H'
  0b0000110, // 73  'I'
  0b0011110, // 74  'J'
  0b1110110, // 75  'K'  NO DISPLAY
  0b0111000, // 76  'L'
  0b0010101, // 77  'M'  NO DISPLAY
  0b1010100, // 78  'n'
  0b0111111, // 79  'O'
  0b1110011, // 80  'P'
  0b1100111, // 81  'q'
  0b1010000, // 82  'r'
  0b1101101, // 83  'S'
  0b1111000, // 84  't'
  0b0111110, // 85  'U'
  0b0011100, // 86  'V'  NO DISPLAY
  0b0101010, // 87  'W'  NO DISPLAY
  0b1110110, // 88  'X'  NO DISPLAY
  0b1101110, // 89  'y'
  0b1011011, // 90  'Z'  NO DISPLAY
  0b0111001, // 91  '['
  0b0000000, // 92  '\'  NO DISPLAY
  0b0001111, // 93  ']'
  0b0000001, // 94  '^'  NO DISPLAY
  0b0001000, // 95  '_'
  0b0000010, // 96  '`'
  0b1110111, // 97  'a' SAME AS CAP
  0b1111100, // 98  'b' SAME AS CAP
  0b1011000, // 99  'c'
  0b1011110, // 100 'd' SAME AS CAP
  0b1111011, // 101 'e'
  0b1110001, // 102 'F' SAME AS CAP
  0b0111101, // 103 'G' SAME AS CAP
  0b1110100, // 104 'h'
  0b0000100, // 105 'i'
  0b0011110, // 106 'j' SAME AS CAP
  0b0000000, // 107 'k'  NO DISPLAY
  0b0000110, // 108 'l'
  0b0000000, // 109 'm'  NO DISPLAY
  0b1010100, // 110 'n' SAME AS CAP
  0b1011100, // 111 'o'
  0b1110011, // 112 'p' SAME AS CAP
  0b1100111, // 113 'q' SAME AS CAP
  0b1010000, // 114 'r' SAME AS CAP
  0b1101101, // 115 'S' SAME AS CAP
  0b1111000, // 116 't' SAME AS CAP
  0b0011100, // 117 'u'
  0b0000000, // 118 'b'  NO DISPLAY
  0b0000000, // 119 'w'  NO DISPLAY
  0b0000000, // 120 'x'  NO DISPLAY
  0b0000000, // 121 'y'  NO DISPLAY
  0b0000000, // 122 'z'  NO DISPLAY
  0b0000000, // 123 '0b' NO DISPLAY
  0b0000000, // 124 '|'  NO DISPLAY
  0b0000000, // 125 ','  NO DISPLAY
  0b0000000, // 126 '~'  NO DISPLAY
  0b0000000, // 127 'DEL'  NO DISPLAY
  0b0000000, // 128 NO DISPLAY
  0b0000000, // 129 NO DISPLAY
  0b0000000, // 130 NO DISPLAY
  0b0000000, // 131 NO DISPLAY
  0b0000000, // 132 NO DISPLAY
  0b0000000, // 133 NO DISPLAY
  0b0000000, // 134 NO DISPLAY
  0b0000000, // 135 NO DISPLAY
  0b0000000, // 136 NO DISPLAY
  0b0000000, // 137 NO DISPLAY
  0b0000000, // 138 NO DISPLAY
  0b0000000, // 139 NO DISPLAY
  0b0000000, // 140 NO DISPLAY
  0b0000000, // 141 NO DISPLAY
  0b0000000, // 142 NO DISPLAY
  0b0000000, // 143 NO DISPLAY
  0b0000000, // 144 NO DISPLAY
  0b0000000, // 145 NO DISPLAY
  0b0000000, // 146 NO DISPLAY
  0b0000000, // 147 NO DISPLAY
  0b0000000, // 148 NO DISPLAY
  0b0000000, // 149 NO DISPLAY
  0b0000000, // 150 NO DISPLAY
  0b0000000, // 151 NO DISPLAY
  0b0000000, // 152 NO DISPLAY
  0b0000000, // 153 NO DISPLAY
  0b0000000, // 154 NO DISPLAY
  0b0000000, // 155 NO DISPLAY
  0b0000000, // 156 NO DISPLAY
  0b0000000, // 157 NO DISPLAY
  0b0000000, // 158 NO DISPLAY
  0b0000000, // 159 NO DISPLAY
  0b0000000, // 160 NO DISPLAY
  0b0000000, // 161 NO DISPLAY
  0b0000000, // 162 NO DISPLAY
  0b0000000, // 163 NO DISPLAY
  0b0000000, // 164 NO DISPLAY
  0b0000000, // 165 NO DISPLAY
  0b0000000, // 166 NO DISPLAY
  0b0000000, // 167 NO DISPLAY
  0b1111001, // 168 'Ё'
  0b0000000, // 169 NO DISPLAY
  0b0000000, // 170 NO DISPLAY
  0b0000000, // 171 NO DISPLAY
  0b0000000, // 172 NO DISPLAY
  0b0000000, // 173 NO DISPLAY
  0b0000000, // 174 NO DISPLAY
  0b0000000, // 175 NO DISPLAY
  0b1100011, // 176 GRADUS
  0b0000000, // 177 NO DISPLAY
  0b0000000, // 178 NO DISPLAY
  0b0000000, // 179 NO DISPLAY
  0b0000000, // 180 NO DISPLAY
  0b0000000, // 181 NO DISPLAY
  0b0000000, // 182 NO DISPLAY
  0b0000000, // 183 NO DISPLAY
  0b1111011, // 184 'ё'
  0b0000000, // 185 NO DISPLAY
  0b0000000, // 186 NO DISPLAY
  0b0000000, // 187 NO DISPLAY
  0b0000000, // 188 NO DISPLAY
  0b0000000, // 189 NO DISPLAY
  0b0000000, // 190 NO DISPLAY
  0b0000000, // 191 NO DISPLAY
  0b1110111, // 192 "A"
  0b1111101, // 193 "Б"
  0b1111111, // 194 "В"
  0b0110001, // 195 "Г"
  0b1011111, // 196 "Д"
  0b1111001, // 197 "Е"
  0b0110110, // 198 'Ж'
  0b1001111, // 199 '3'
  0b0000000, // 200 'И
  0b0000000, // 201 'Й'
  0b0000000, // 202 'К'  NO DISPLAY
  0b0000000, // 203 'Л'
  0b0000000, // 204 'M'  NO DISPLAY
  0b1110110, // 205 'Н'
  0b0111111, // 206 'O'
  0b0110111, // 207 'П'
  0b1110011, // 208 'Р'
  0b0111001, // 209 'С'
  0b1101101, // 210 'Т'
  0b1101110, // 211 'У'
  0b0000000, // 212 'Ф'
  0b0000000, // 213 'Х'  NO DISPLAY
  0b0111110, // 214 'Ц'  NO DISPLAY
  0b1100110, // 215 'Ч'  NO DISPLAY
  0b0000000, // 216 'Ш'
  0b0000000, // 217 'Щ'  NO DISPLAY
  0b1111100, // 218 'Ь'
  0b1111100, // 219 'Ы'  NO DISPLAY
  0b1111100, // 220 'Ъ'
  0b1001111, // 221 'Э'  NO DISPLAY
  0b0000000, // 222 'Ю'
  0b1110111, // 223 'Я'
  0b1110111, // 224 'a' SAME AS CAP
  0b1111101, // 225 'б' SAME AS CAP
  0b1111111, // 226 'в'
  0b1010000, // 227 'г' SAME AS CAP
  0b0000000, // 228 'д'
  0b1111011, // 229 'е' SAME AS CAP
  0b0000000, // 230 'ж' SAME AS CAP
  0b1001111, // 231 'з'
  0b0000000, // 232 'и'
  0b0000000, // 233 'й' SAME AS CAP
  0b0000000, // 234 'к'  NO DISPLAY
  0b0000000, // 235 'л'
  0b0000000, // 236 'м'  NO DISPLAY
  0b0110110, // 237 'н' SAME AS CAP
  0b1011100, // 238 'o'
  0b1010100, // 239 'п' SAME AS CAP
  0b1110011, // 240 'р' SAME AS CAP
  0b1011000, // 241 'с' SAME AS CAP
  0b0000000, // 242 'т' SAME AS CAP
  0b1101110, // 243 'у' SAME AS CAP
  0b0000000, // 244 'ф'
  0b0000000, // 245 'х'  NO DISPLAY
  0b0011100, // 246 'ц'  NO DISPLAY
  0b1100110, // 247 'ч'  NO DISPLAY
  0b0000000, // 248 'ш'  NO DISPLAY
  0b0000000, // 249 'щ'  NO DISPLAY
  0b1111100, // 250 'ь'
  0b0000000, // 251 'ы'  NO DISPLAY
  0b0000000, // 252 'ъ'  NO DISPLAY
  0b0000000, // 253 'э'  NO DISPLAY
  0b0000000, // 254 'ю'  NO DISPLAY
  0b0000000, // 255 'я'  NO DISPLAY
};


#include <TM1637.h>

#define CLK 5//pins definitions for TM1637 and can be changed to other ports
#define DIO 4

TM1637 tm1637(CLK, DIO);

int8_t TimeDisp[] = {0xff, 0xff, 0xff, 0xff};
scr_buff_t s7dig;                               // разряды индикаторов
