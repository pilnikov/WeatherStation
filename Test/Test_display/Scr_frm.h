
#ifndef Scr_frm_h
#define Scr_frm_h

#include "conf.h"

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

    v_scroll_all(uint8_t, byte *);  // запуск вертушка для всех изменившихся позиций

  bool symbol_time_part_view(bool use_pm, bool end_run_st, rtc_time_data_t rtd,
                             rtc_alm_data_t rta, main_cfg_t mcf, byte *screen);  // Формирует часть фрейма с отображением текущего времени на символьных (матричных и LCD1602) дисплеях
private:

  bool
  time_m32_8(byte *in, uint8_t pos, unsigned char *rep_rec, const uint8_t *dposx,
             bool *change, uint16_t *buff, const byte *font, bool pm, const uint8_t q_dig, rtc_time_data_t rtd);  // Формирует часть фрейма(32x8) с отображением текущего времени на матричных  дисплеях


  uint8_t
    q_dig = 6;  // Количество символов на матрице
  bool
    rep_rec[6];  // Флаг "Символ устарел и требует замены (replace required)"
  const uint8_t
    dposx[6] = { 0, 6, 13, 19, 25, 29 };  // Позиции символов на матрице по оси х
  unsigned char
    change[6];  // Буфер символов для вертикальной прокрутки
  uint16_t
    v_scroll_buff[64];  // Буфер для вертикальной прокрутки
protected:
};


#endif
