
//--------------------------------------------------------------------------------------------------------------------------------HW define
#ifndef Scr_frm_h
#define Scr_frm_h

#include "conf.h"

#include <LiquidCrystal_I2C.h>
#include <Max72.h>
//#include <SHT1632_3.h>
#include <myHT16K33.h>
#include <myTM1637.h>
#include <HT1632.h>

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Adafruit_Protomatter.h>
#endif

#if defined(__AVR_ATmega2560__)
#include <RGBmatrixPanel.h>  // Hardware-specific library
#endif

//---------------------------------------------------------------------------HT1621
#include <HT1621.h>

//---------------------------------------------------------------------------ILI9341
#include <Adafruit_ILI9341.h>

//----------------------------------------------------------------------------------------------------------------------------------define



class SSCF  // Формирование содержимого для сегментных дисплеев (SegmentScreenContentFormer)
{
public:
  void
  seg_scr_frm(bool use_pm, bool blinkColon, bool end_run_st, rtc_time_data_t rtd, rtc_alm_data_t rta,
              byte *screen, main_cfg_t mcf, snr_data_t snr, uint16_t br);
private:
  uint8_t
  seg_add_data(uint8_t &mod, uint8_t _width, byte *in, uint8_t _offset, snr_data_t snr,
               rtc_time_data_t rt, rtc_alm_data_t rta, bool pm, uint16_t c_br);  // Формирует подстроку с доп данными (не текущее время)
protected:
};

class MSCF  // Формирование содержимого для символьных (матричных и LCD1602) дисплеев (MatrixScreenContentFormer)
{
public:
  void
  roll_string_cf(uint8_t &num, uint8_t _max, main_cfg_t mcf, snr_data_t snr, wf_data_t wf, wf_data_t wfc,
                 rtc_time_data_t rt, rtc_alm_data_t rta, String local_ip, uint8_t c_br, char out[], bool cur_cli, String newsClient),  // Формирует контент для бегущей строки

    h_scroll(uint8_t vdrv_t, uint8_t dsp_t, bool slow_mode, bool& _end, char* in_st, byte* out),  // Формирует бегущую строку
    h_scroll_restart(uint8_t &num, uint8_t _max, main_cfg_t mcf, snr_data_t snr, wf_data_t wf,
                     wf_data_t wfc, rtc_time_data_t rt, rtc_alm_data_t rta, String local_ip, uint16_t c_br, bool cli, String ns,
                     uint8_t &ni, bool &end, char *st1, byte *screen),  // ---------------------------- Перезапуск бегущей строки

    v_scroll_all(uint8_t, byte *);  // запуск вертушка для всех изменившихся позиций

  bool symbol_time_part_view(bool use_pm, bool end_run_st, rtc_time_data_t rtd,
                             rtc_alm_data_t rta, main_cfg_t mcf, byte *screen);  // Формирует часть фрейма с отображением текущего времени на символьных (матричных и LCD1602) дисплеях
private:

  bool
  time_m32_8(byte *in, uint8_t pos, unsigned char *rep_rec, const uint8_t *dposx,
             bool *change, uint16_t *buff, const byte *font, bool pm, const uint8_t q_dig, rtc_time_data_t rtd);  // Формирует часть фрейма(32x8) с отображением текущего времени на матричных  дисплеях


  const uint8_t
    q_dig = 6,  // Количество символов на матрице
    dposx[6] = { 0, 6, 13, 19, 25, 29 };  // Позиции символов на матрице по оси х
  unsigned char
    change[6];  // Буфер символов для вертикальной прокрутки
  uint16_t
    v_scroll_buff[32];  // Буфер для вертикальной прокрутки
  bool
    rep_rec[6];  // Флаг "Символ устарел и требует замены (replace required)"
protected:
};

//------------------------------------------------------------------------------------------------------------------------------- HW Sections

class MyDspHW {
public:
  void
  _init(byte vdrv_t, byte dsp_t, gpio_cfg_t gcf, byte ht_addr, byte lcd_addr, uint8_t &text_size, byte *screen, bool rus_lng),

    _write(bool from_time, bool time_up, uint8_t vdrv_t, uint8_t dsp_t, uint16_t br, 
           uint8_t text_size, uint8_t color_up, uint8_t color_dwn, byte *screen),

    _off(byte),
    _on(byte);

private:
  //----------------------------------------------------------------------------------------------------------------------------------Global Var
  void
    tm1637_init(byte, byte),
    pcf8574_init(byte, uint8_t, uint8_t, bool),
    m7219_init(byte, uint8_t, byte *),
    ht1621_init(uint8_t, uint8_t, uint8_t, byte *),
    ht1632_init(uint8_t, uint8_t, uint8_t, uint8_t),
    ht1633_init(byte),
    ili9341_init(),

#if defined(__AVR_ATmega2560__) || CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
    a595_init(byte, byte &, uint8_t &),
    m3216_ramFormer(byte *, uint8_t, uint8_t),
#endif

    m7219_ramFormer(byte *),
    m7219_ramFormer2(byte *, uint8_t, uint8_t),
    ht1632_ramFormer(byte *, const uint8_t, const uint8_t),
    ht1633_ramFormer(byte *, uint8_t, uint8_t),
    ht1633_ramFormer2(byte *, uint8_t, uint8_t),
    m7adopt(byte *, uint8_t, uint8_t),
    roll_seg(byte &),

    //==========================Not support
    ili_time(bool, bool),
    drawWifiQuality();
  int8_t
  getWifiQuality();
protected:
};

class MyDspBCF  // Функции для управления яркостью (BrightControlFunctions)
{
public:
  uint8_t
    auto_br(uint16_t, main_cfg_t);  // Авто регулировка яркости с расчетом пределов
  uint16_t
  ft_read(bool, uint16_t, uint8_t);  // Селектор источника измерения освещенности

private:
protected:
};

#endif
