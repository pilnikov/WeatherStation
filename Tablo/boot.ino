//#include ".\headers\fonts.h"
#include "fonts.h"

static int cur_sym_pos[2] = {0, 0};
static uint16_t buffud[64];
static bool d_notequal[q_dig];
static const uint8_t digPos_x[q_dig] = {0, 6, 13, 19, 25, 29}; // позиции цифр на экране по оси x
static unsigned char oldDigit[q_dig];                       // убегающая цифра
static uint8_t  num_st = 0;
static const uint8_t max_st = 3; //номер и макс кол-во прокручиваемых строк

void irq_set()
{
  //------------------------------------------------------ interrupts
  /*  irq1.attach(3600, firq1);
    irq2.attach(1800, rtc_check);
    irq3.attach(conf_data.period * 60, GetSnr);
    irq4.attach(55, firq4);
    irq6.attach(0.5, firq6);
    //irq7.attach(0.2, firq7);
    irq8.attach(0.062, firq8);
    irq9.attach(0.04, firq9);
  */
  const unsigned long timers [5] = {3600000L, conf_data.period * 60000L, 500, 125, 25};

  uint8_t irq = 4;
  for (uint8_t i = 0; i < 5;  i++) if (millis() - irq_end[i] >  timers[i]) irq = i;
  switch (irq)
  {
    case 0: // one per hour
      rtc_check();
      irq_end[0] = millis();
      break;

    case 1: // one per 10 min
      GetSnr();
      irq_end[1] = millis();
      break;

    case 2: // one per 0.5 sec
      firq2();
      irq_end[2] = millis();
      break;

    case 3:
      firq3();  // one per 0.125 sec
      irq_end[3] = millis();
      break;

    case 4: // one per 0.04 sec
      firq4();
      irq_end[4] = millis();
      break;
    default:
      break;
  }

  if (end_run_st != end_run_st_buf)
  {
    end_run_st_buf = end_run_st;

    if (end_run_st)
    {
      if (num_st++ > max_st) num_st = 1;

      st1 = pr_str(num_st);

      f_dsp.utf8rus(st1);

      if (!nm_is_on)
      {
        cur_sym_pos[0] = 0;
        cur_sym_pos[1] = 0;
      }
    }
  }

  uint8_t h = hour();
  //m3216 -> fillScreen(0);
  disp_on = true;

  if (h < 7 || h >= 17) //гашение экрана в нерабочее время
  {
    //    bright = 50;
    m3216 -> drawPixel(h, 0, m3216 -> ColorHSV(500, 255, 100, true));
    m3216 -> drawPixel(minute() / 2, 15, m3216 -> ColorHSV(500, 255, 100, true));
    if (minute() % 10 == 0 && second() == 0) synchro();
    disp_on = false;
  }

  Buzz.play(pgm_read_word(&songs[rtc_data.a_muz]), conf_data.gpio_snd, play_snd, conf_data.snd_pola);   //inital sound card
  play_snd = false;

  //------------------------------------------------------  Верифицируем ночной режим
  if (conf_data.nm_start <  conf_data.nm_stop) nm_is_on = ((hour() >= conf_data.nm_start) & (hour() < conf_data.nm_stop));
  else nm_is_on = (hour() >= conf_data.nm_start || hour() < conf_data.nm_stop);
}


void firq2() // 0.5 sec main cycle
{
  if (disp_on)
  {
    //-------------Brigthness------------------
    if (conf_data.auto_br)
    {
      snr_data.f = ft_read(ram_data.bh1750_present);
      cur_br = auto_br(snr_data.f);
    }
    else
    {
      cur_br = conf_data.man_br;  // Man brigthness
      snr_data.f = cur_br;
    }

    //-----------------------------------------

    // run slowely time displays here
    m32_8time_act = time_m32_8(screen, 32, oldDigit, digPos_x, d_notequal, buffud, conf_data.use_pm);

  }
  if (Alarmed()) rtc_data.wasAlarm = true;
  if (rtc_data.wasAlarm & !play_snd)
  {
    set_alarm();
    rtc_data.wasAlarm = false;
  }
}


void firq3() // 0.125 sec
{
  const uint8_t pos = 32;

  if (m32_8time_act)
  {
    uint8_t font_wdt = 5;
    static byte nbuf[64];

    for (uint8_t i = 0; i < q_dig; i++)
    {
      if (i > 3) font_wdt = 3;

      if (d_notequal[i])
      {
        shift_ud(true, false, nbuf + pos, screen + pos,  buffud + pos, digPos_x[i],  digPos_x[i] + font_wdt); // запуск вертушка для изменившихся позиций
      }
    }
  }
}

void firq4() //0.04 sec running string is out switch to time view
{
  end_run_st = scroll_String(0, 31, st1, cur_sym_pos[0], cur_sym_pos[1], screen, font5x7, 5, 1, 1);
  m3216_ramFormer(screen);
  m3216 -> swapBuffers(true);
}
