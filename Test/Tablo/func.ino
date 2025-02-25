String cur_time_str(rtc_data_t rt)
{
  char buf[25];

  const char* sdnr_1 = PSTR("ВС");
  const char* sdnr_2 = PSTR("ПН");
  const char* sdnr_3 = PSTR("ВТ");
  const char* sdnr_4 = PSTR("СР");
  const char* sdnr_5 = PSTR("ЧТ");
  const char* sdnr_6 = PSTR("ПТ");
  const char* sdnr_7 = PSTR("СБ");

  const char* const sdnr[] = {sdnr_1, sdnr_2, sdnr_3, sdnr_4, sdnr_5, sdnr_6, sdnr_7};

  sprintf_P(buf, PSTR("%S %02u.%02u.%04u %02u:%02u:%02u"), sdnr[rt.wday - 1], rt.day, rt.month, rt.year, rt.hour, rt.min, rt.sec);
  return String(buf);
}

//--------------------------------  Читаем конфигурацию из ЕЕПРОМ
conf_data_t loadConfig()
{
  conf_data_t _data;
  EEPROM.get(0, _data);
  return _data;
}

//--------------------------------  Пишем конфигурацию в ЕЕПРОМ
void saveConfig(conf_data_t _data)
{
  EEPROM.put(0, _data);
}

//--------------------------------  Настраиваем конфигурацию (дефолт)
conf_data_t defaultConfig()
{
  conf_data_t _data;
  if (debug_level == 3) DBG_OUT_PORT.println(F("Start inital conf_data with config.json"));

  strcpy(_data.ch1_name,   "Внутри");
  strcpy(_data.ch2_name,   "На улице");
  strcpy(_data.ch3_name,   "Канал3");


  _data.auto_corr        = true;
  _data.use_pm           = false;
  _data.every_hour_beep  = true;
  _data.snd_pola         = false;
  _data.rus_lng          = true;
  _data.time_zone        = 5;
  _data.type_vdrv        = 3;

  //---------------------------- Датчики
  _data.type_snr1        = 10;
  _data.type_snr2        = 4;
  _data.type_snr3        = 0;
  _data.type_snrp        = 10;

  _data.type_rtc         = 1;
  _data.nm_start         = 17;
  _data.nm_stop          = 7;
  _data.period           = 10;
  _data.man_br           = 14;
  _data.nmd_br           = 2;
  _data.auto_br          = true;

  _data.type_thermo      = 0;
  _data.src_thermo       = 0;
  _data.lb_thermo        = 0;
  _data.hb_thermo        = 0;

#if defined(__AVR_ATmega2560__)
  _data.led_pola         = true;
  _data.type_disp        = 23;

  _data.gpio_sda         = 20;
  _data.gpio_scl         = 21;
  _data.gpio_dio         = 57; //A3
  _data.gpio_clk         = 56; //A2
  _data.gpio_dcs         = 55; //A1
  _data.gpio_dwr         = 54; //A0
  _data.gpio_trm         = 60; //A6
  _data.gpio_sqw         = 19;
  _data.gpio_snd         = 61; //A7
  _data.gpio_led         = 60; //A6
  _data.gpio_btn         = 59; //A5
  _data.gpio_dht         = 63; //A9
  _data.gpio_ana         = 58; //A4
  _data.gpio_uar         = 65; //A11
  _data.gpio_bz2         = 64; //A10

#elif defined(ARDUINO_ARCH_ESP32)
  _data.type_snr1        = 6;
  _data.type_disp        = 24;

  _data.led_pola         = true;

  _data.gpio_sda         = 23;
  _data.gpio_scl         = 22;
  _data.gpio_dio         = 19;
  _data.gpio_clk         = 18;
  _data.gpio_dcs         =  5;
  _data.gpio_dwr         = 26;
  _data.gpio_trm         = 36;
  _data.gpio_sqw         = 32;
  _data.gpio_snd         =  2;
  _data.gpio_led         = 16;
  _data.gpio_btn         =  0;
  _data.gpio_dht         = 39;
  _data.gpio_ana         = 35;
  _data.gpio_uar         = 36;
  _data.gpio_bz2         = 36;
#endif

  //--------------------------------Уровни для автояркости
  _data.br_level[0]      = 300;
  _data.br_level[1]      = 1;
  _data.br_level[2]      = 1;
  _data.br_level[3]      = 254;

  //--------------------------------Будильники
  for (uint8_t i = 0; i <= 6; i++)
  {
    for (uint8_t j = 0; j <= 4; j++)
    {
      _data.alarms[i][j] = 0;
    }
  }

  _data.alarms[0][0] = 2;
  _data.alarms[0][1] = 16;
  _data.alarms[0][2] = 30;
  _data.alarms[0][3] = 13;
  _data.alarms[0][4] = 0;

  _data.alarms[1][0] = 2;
  _data.alarms[1][1] = 16;
  _data.alarms[1][2] = 38;
  _data.alarms[1][3] = 7;
  _data.alarms[1][4] = 0;

  _data.alarms[2][0] = 2;
  _data.alarms[2][1] = 11;
  _data.alarms[2][2] = 59;
  _data.alarms[2][3] = 8;
  _data.alarms[2][4] = 0;

  return _data;
}

//------------------------------------------------------  Получаем данные извне
void synchro()
{
  // --------------- Начальная инициализация
  rtc_data.ct = 1530687234; //Текущее время
  snr_data.h1  =   0; //Внутренняя влажность
  snr_data.t1  =  99; //Внутренняя температура
  snr_data.h2  =   0; //Внешнняя влажность
  snr_data.t2  =  99; //Внешнняя температура
  snr_data.p   = 700; //Давление
  ram_data.lb  =  15; //Яркость

#ifdef _debug
  DBG_OUT_PORT.println(F("True read data from COM port......"));
#endif

  String inString = Serial_Read();

#ifdef _debug
  DBG_OUT_PORT.println(F("\n inString.." + inString));
#endif
  parser(inString);
  if (wf_data.day == 0) conf_data.use_pp = 0;
  cur_br = ((ram_data.lb + 1) * 16) - 1;
  DBG_OUT_PORT.print(F("Cur_br ..."));  DBG_OUT_PORT.println(cur_br);
}

//------------------------------------------------------  Читаем из Serial
String Serial_Read()
{
  char c; // переменная для чтения сериал порта
  String S_str = String(); // Формируемая из символов строка
  uint8_t i = 0;

#if defined(__AVR_ATmega2560__)
  Serial3.begin(19200);
  digitalWrite(conf_data.gpio_uar, LOW);

  do
  {
    if (Serial3.available() > 0) c = Serial3.read(); // Читаем символ
    delay(1);
#ifdef _debug
    DBG_OUT_PORT.print(F("inbox char is ..."));  DBG_OUT_PORT.println(c);
#endif
    if (c == '{') S_str = String();
    S_str += c; //Формируем строку
    i++;
  }  while (c != '}' && i < 250); //пока не найдем "}"
  digitalWrite(conf_data.gpio_uar, HIGH);
  Serial3.end();
#endif
  return S_str;
}

//------------------------------------------------------  Парсим полученные извне данные
void parser(String inStr)
{
# ifdef _debug
  DBG_OUT_PORT.print(F("inStr is ..."));  DBG_OUT_PORT.println(inStr);
# endif

  DynamicJsonDocument jsonBuffer(500);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(jsonBuffer, inStr);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  JsonObject json = jsonBuffer.as<JsonObject>();

  rtc_data.ct         = json["T"];
  ram_data.lb         = json["U"];
  es_data1.t1         = json["V"];
  es_data1.t2         = json["W"];
  es_data1.h1         = json["X"];
  es_data1.h2         = json["Y"];
  es_data1.p          = json["Z"];

  wf_data.tod        = json ["A"];
  wf_data.cloud      = json ["B"];
  wf_data.prec       = json ["C"];
  wf_data.rpower     = json ["D"];
  wf_data.spower     = json ["E"];
  wf_data.press_max  = json ["F"];
  wf_data.temp_max   = json ["G"];
  wf_data.temp_min   = json ["H"];
  wf_data.wind_max   = json ["I"];
  wf_data.wind_min   = json ["J"];
  wf_data.wind_dir   = json ["K"];
  wf_data.hum_max    = json ["L"];
  wf_data.day        = json ["O"];
  wf_data.month      = json ["P"];

  //#ifdef _debug
  DBG_OUT_PORT.println(F("Data from ext server"));
  DBG_OUT_PORT.print(F("Current time is: ")); DBG_OUT_PORT.println(rtc_data.ct);
  DBG_OUT_PORT.print(F("Brightness ..."));  DBG_OUT_PORT.println(ram_data.lb);
  DBG_OUT_PORT.print(F("Temperature inside: "));  DBG_OUT_PORT.print(es_data1.t1); DBG_OUT_PORT.print(F(" degrees Celcius Humidity inside: ")); DBG_OUT_PORT.print(es_data1.h1); DBG_OUT_PORT.println(F(" % "));
  DBG_OUT_PORT.print(F("Temperature outside: ")); DBG_OUT_PORT.print(es_data1.t2); DBG_OUT_PORT.print(F(" degrees Celcius Humidity outside: ")); DBG_OUT_PORT.print(es_data1.h2); DBG_OUT_PORT.println(F(" % "));
  DBG_OUT_PORT.print(F("Pressure: ")); DBG_OUT_PORT.print(es_data1.p); DBG_OUT_PORT.println(F("mm rt. st."));
  DBG_OUT_PORT.print(F("Prognoz: ")); DBG_OUT_PORT.print(wf_data.day);
  DBG_OUT_PORT.print(F("Current alarm: ")); DBG_OUT_PORT.print(rtc_data.a_hour); DBG_OUT_PORT.print(':'); DBG_OUT_PORT.println(rtc_data.a_min);
  //#endif
}

//------------------------------------------------------  Инициализируем дисплей
void m3216_init()
{
#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)
  //G1  R1 | 12 04
  //GND B1 |  g 13
  //G2  R2 | 15 14
  //E   B2 | 25 21
  //B   A  | 05 26
  //D   C  | 19 18
  //LAT CLK| 32 27
  //GND OE |  g 33

#if defined(__AVR_ATmega2560__)
  uint8_t A_PIN =  54, //A0 Пин A
          B_PIN =  55, //A1 Пин B
          C_PIN =  56, //A2 Пин C
          D_PIN =  57, //A3 Пин D

          CLK_PIN =  11,  // Пин CLK MUST be on PORTB! (Use pin 11 on Mega)
          LAT_PIN =  10,  // Пин LAT
          OE_PIN =   9;   // Пин OE
#endif


  if (conf_data.type_disp == 23 || conf_data.type_disp == 24 || conf_data.type_disp == 25)
  {
    char tstr[255];
#if defined(__AVR_ATmega2560__)
    m3216 = new RGBmatrixPanel(A_PIN, B_PIN, C_PIN, CLK_PIN, LAT_PIN, OE_PIN, true);

#elif defined(ARDUINO_ARCH_ESP32)
    uint8_t rgbPins[] = {4, 12, 13, 14, 15, 21},
                        addrPins[] = {26, 5, 18, 19, 25},
                                     clockPin   = 27, // Must be on same port as rgbPins
                                     latchPin   = 32,
                                     oePin      = 33,
                                     naddr_pin  = 3,
                                     wide       = 32;
    if (conf_data.type_disp != 23) wide = 64;
    if (conf_data.type_disp == 24)
    {
      naddr_pin = 4;
      text_size = 2;
    }
    if (conf_data.type_disp == 25)
    {
      naddr_pin = 5;
      text_size = 4;
    }


    m3216 = new Adafruit_Protomatter(
      wide,        // Matrix width in pixels
      6,           // Bit depth -- 6 here provides maximum color options
      1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
      naddr_pin, addrPins, // # of address pins (height is inferred), array of pins
      clockPin, latchPin, oePin, // Other matrix control pins
      true);       // HERE IS THE MAGIG FOR DOUBLE-BUFFERING!
#endif

#if defined(__AVR_ATmega2560__)
    m3216 -> begin();

#elif defined(ARDUINO_ARCH_ESP32)

    ProtomatterStatus status = m3216 -> begin();

    if (status != PROTOMATTER_OK)
    {
      ram_data.type_vdrv = 0;
      DBG_OUT_PORT.print(F("Failed initialize display - set it to 0..."));
    }
#endif
  }
#endif
}

void m3216_ramFormer(byte *in, uint8_t c_br, uint8_t t_size)
{
#if defined(__AVR_ATmega2560__) || defined(ARDUINO_ARCH_ESP32)
  for (uint8_t x = 0; x < 32; x++)
  {
    uint8_t dt = 0b1;
    for (uint8_t y = 0; y < 8; y++)
    {
      for (uint8_t xz = 0; xz < t_size; xz++)
      {
        uint8_t _x = (x * t_size) + xz;
        for (uint8_t yz = 0; yz < t_size; yz++)
        {
          uint8_t _y = (y * t_size) + yz;
          uint8_t _yy = _y + (8 * t_size);
#if defined(__AVR_ATmega2560__)
          m3216 -> drawPixel(_x, _y, (in[x] & dt << y) ?  m3216 -> ColorHSV(700, 255, c_br, true) : 0);
          m3216 -> drawPixel(_x, _yy, (in[x + 32] & dt << y) ?  m3216 -> ColorHSV(400, 255, c_br, true) : 0);
#elif defined(ARDUINO_ARCH_ESP32)
          m3216 -> drawPixel(_x, _y, (in[x] & dt << y) ?  m3216 -> color565(c_br, 0 , 0) : 0);
          m3216 -> drawPixel(_x, _yy, (in[x + 32] & dt << y) ?  m3216 -> color565(0, c_br, 0) : 0);
#endif
        }
      }
    }
  }
#if defined(__AVR_ATmega2560__)
  m3216 -> swapBuffers(true);
#elif defined(ARDUINO_ARCH_ESP32)
  m3216 -> show();
#endif
#endif
}

//------------------------------------------------------  Получаем данные с датчиков
void GetSnr()
{
  snr_data.t1 = 99;
  snr_data.t2 = 99;
  snr_data.t3 = 99;
  snr_data.h1 = 0;
  snr_data.h2 = 0;
  snr_data.h3 = 0;
  snr_data.p = 700;

  ram_data.temp_rtc = 99;

  RtcTemperature t1 = ds3231 -> GetTemperature();
  ram_data.temp_rtc = round(t1.AsFloatDegC());

  snr_data = sens.read_snr(ram_data.type_snr1, ram_data.type_snr2, ram_data.type_snr3, ram_data.type_snrp, ram_data.temp_rtc, es_data1, es_data1, es_data1, wf_data_cur); // Заполняем матрицу данных с датчиков
}

void(* resetFunc) (void) = 0; //Programm reset
