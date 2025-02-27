/* Методы формируют часть фрейма, содержащее текущее время, для символьных (матричных и LCD1602) дисплеев */

#include "Scr_frm.h"
#include "fonts.h"

CT rtc;   //For RTC Common
FFF scr;  //For frame former funcions

// Формирует часть фрейма с отображением текущего времени на символьных (матричных и LCD1602) дисплеях
bool MSCF::symbol_time_part_view(bool use_pm, bool end_run_st, rtc_time_data_t rtd, rtc_alm_data_t rta, main_cfg_t mcf, byte *screen) {
  uint8_t h = use_pm && rtd.hour != 12 ? rtd.hour % 12 : rtd.hour;
  h = h % 100;

  bool _alarmed = rta.num < 7, m32_8time_act = false;

  uint8_t mod = 13, x1 = 0;
  if (!mcf.time_up) x1 = 32;
  if (end_run_st || rtd.nm_is_on) scr.CLS(screen, sizeof screen);
  switch (mcf.dsp_t) {
    case 19:
      // 2LINEx16D
      if (_alarmed) {
        rtc_hms_t alt = rtc.unix_to_hms(rta.time);
        if (mcf.rus_lng) sprintf_P((char *)screen, PSTR("%3d:%02d:%02d %2d:%02d\355"), h, rtd.min, rtd.sec, alt.h, alt.m);
        else sprintf_P((char *)screen, PSTR("%3d:%02d:%02d %2d:%02d"), h, rtd.min, rtd.sec, alt.h, alt.m);
      } else sprintf_P((char *)screen, PSTR("%3d:%02d:%02d --:-- "), h, rtd.min, rtd.sec);
      break;
    case 20:
      // M32x8MONO
      m32_8time_act = MSCF::time_m32_8(screen, 0, change, dposx, rep_rec, v_scroll_buff, font5x7, use_pm, q_dig, rtd);
      break;
    case 21:
      // m32x16MONO
      m32_8time_act = MSCF::time_m32_8(screen, x1, change, dposx, rep_rec, v_scroll_buff, font5x7, use_pm, q_dig, rtd);
      break;
    case 22:
      // M32x16BICOL
      m32_8time_act = MSCF::time_m32_8(screen, x1, change, dposx, rep_rec, v_scroll_buff, font5x7, use_pm, q_dig, rtd);
      break;
    case 23:
      // M32x16COLOR
      m32_8time_act = MSCF::time_m32_8(screen, x1, change, dposx, rep_rec, v_scroll_buff, font5x7, use_pm, q_dig, rtd);
      break;
    case 24:
      // M64x32COLOR
      m32_8time_act = MSCF::time_m32_8(screen, x1, change, dposx, rep_rec, v_scroll_buff, font5x7, use_pm, q_dig, rtd);
      break;
    case 25:
      // M64x64COLOR
      m32_8time_act = MSCF::time_m32_8(screen, x1, change, dposx, rep_rec, v_scroll_buff, font5x7, use_pm, q_dig, rtd);
      break;
    case 29:
      // 320x240COLOR
      //ili_time(mcf.rus_lng, use_pm);
      break;
      break;
    default:
      break;
  }
  return m32_8time_act;
}

bool MSCF::time_m32_8(byte *in, uint8_t pos, unsigned char *rep_rec, const uint8_t *dposx, bool *change, uint16_t *buff, const byte *font, bool pm, const uint8_t q_dig, rtc_time_data_t rtd) {
  //----------------------------------------------------------------- заполнение массива
  unsigned char d[q_dig];
  uint8_t font_wdt = 5;
  byte nbuf[256];

  uint8_t h = rtd.hour;
  // Do 24 hour to 12 hour format conversion when required.
  if (pm & (h != 12)) h = h % 12;

  d[0] = ' ';
  if (h > 9) d[0] = h / 10 + '0';
  d[1] = h % 10 + '0';
  d[2] = rtd.min / 10 + '0';
  d[3] = rtd.min % 10 + '0';
  d[4] = rtd.sec / 10 + '\x80';
  d[5] = rtd.sec % 10 + '\x80';

  for (uint8_t i = 0; i < q_dig; i++) {
    if (i > 3) font_wdt = 3;
    change[i] = d[i] != rep_rec[i];  // проверка изменений в буфере отображаемых символов
    if (change[i]) {
      scr.printCharacter(d[i], dposx[i], nbuf + pos, font, 5);                                    // запись символа в вертушок для изменившихся позиций
      scr.v_scroll(true, true, nbuf + pos, in + pos, buff + pos, dposx[i], dposx[i] + font_wdt);  // запись символа в вертушок для изменившихся позиций
    } else {
      scr.printCharacter(rep_rec[i], dposx[i], in + pos, font, 5);  // отображение символов
    }
    rep_rec[i] = d[i];  // перезапись предыдущих значений в буфер
  }
  //DBG_OUT_POrtd.println(F("time_m32_8");
  return true;
}


void MSCF::v_scroll_all(uint8_t pos, byte *screen) // Вертикальная прокрутка всех элементов на экране
{
  uint8_t font_wdt = 5;
  byte nbuf[64];

  for (uint8_t i = 0; i < q_dig; i++)
  {
    if (i > 3) font_wdt = 3;

    if (rep_rec[i])
    {
      scr.v_scroll(true, false, nbuf + pos, screen + pos,  v_scroll_buff + pos, dposx[i],  dposx[i] + font_wdt); // запуск вертушка для изменившихся позиций
    }
  }
}

