/////////////////////////////////////////////////////////////////////////////////////////
#include "conf.h"

void setup() {
  //------------------------------------------------------  Определяем консоль
  DBG_OUT_PORT.begin(115200);

  //------------------------------------------------------  Инициализируем встроенную файловую систему LittleFS
  lfs.begin();
  DBG_OUT_PORT.println(F("LittleFS started"));

  //------------------------------------------------------  Загружаем основную конфигурацию
  conf_f = "/conf_main.json";
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    DBG_OUT_PORT.println(F("Failed to open conf_main.json for reading. Using default configuration!!!"));
    conf_data = main_cfg.def_conf();
    from_client = main_cfg.to_json(conf_data);
    lfs.writeFile(conf_f, from_client.c_str());
  } else {
    conf_data = main_cfg.from_json(from_client);
  }
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //------------------------------------------------------  Загружаем конфигурацию WiFi
  conf_f = "/conf_wifi.json";
  from_client = lfs.readFile(conf_f);
  if (from_client == "Failed to open file for reading") {
    DBG_OUT_PORT.println(F("Failed to open conf_wifi.json for reading. Using default configuration!!!"));
    wifi_data = wifi_cfg.def_conf();
    from_client = wifi_cfg.to_json(wifi_data);
    lfs.writeFile(conf_f, from_client.c_str());
  } else {
    wifi_data = wifi_cfg.from_json(from_client);
  }
  DBG_OUT_PORT.print(conf_f);
  DBG_OUT_PORT.println(F(" loaded"));

  //--------------------------------------------------------  Запускаем сервер и веб морду
  web_setup();
  start_serv();

#if CONFIG_IDF_TARGET_ESP32
  Wire.setPins(21, 22);
#elif CONFIG_IDF_TARGET_ESP32S2
  Wire.setPins(1, 2);
#elif CONFIG_IDF_TARGET_ESP32C3
  Wire.setPins(5, 6);
#elif defined(ESP8266)
  Wire.begin(4, 5);
#endif

  hw_data = hw_chk.hw_present();
  if (hw_data.bh1750_present) lightMeter.begin();
  else pinMode(17, INPUT);

  conf_data.ch1_in ? pinMode(conf_data.ch1_gpio, INPUT) : pinMode(conf_data.ch1_gpio, OUTPUT);
  conf_data.ch2_in ? pinMode(conf_data.ch2_gpio, INPUT) : pinMode(conf_data.ch2_gpio, OUTPUT);
}

void loop() {
  //------------------------------------------------------ Распределяем системные ресурсы

  server.handleClient();

  if (hw_data.bh1750_present) ft = lightMeter.readLightLevel() / 4;
  //else ft = analogRead(17) / 4;

  //DBG_OUT_PORT.print(F("Lux.."));
  //DBG_OUT_PORT.println(lm);

  if (conf_data.ch1_gpio < 255) {
    if (conf_data.ch1_in) {
      if (!bumpless) {
        buttonState = digitalRead(conf_data.ch1_gpio);
        if (buttonState == LOW && !ch1_press) {
          DBG_OUT_PORT.println(F("Button 1 pressed.."));
          ch2_set = !ch2_set;
          ch2_auto = false;  // turn OFF the AUTO MODE for ch2
          ch1_press = true;
          bumpless = true;
          setting_ms = millis();
        }
        if (buttonState == HIGH && ch1_press) ch1_press = false;
      }
    } else {
      if (ch1_auto && ft > conf_data.lim_h && ch1_set) ch1_set = false;
      if (ch1_auto && ft < conf_data.lim_l && !ch1_set) ch1_set = true;

      ch1_val_buf = 0;
      if (ch1_set) conf_data.ch1_dig ? ch1_val_buf = 255 : ch1_val_buf = ch1_val;

      analogWrite(conf_data.ch1_gpio, ch1_val_buf);
    }
  }


  if (conf_data.ch2_gpio < 255) {
    if (conf_data.ch2_in) {
      if (!bumpless) {
        buttonState = digitalRead(conf_data.ch2_gpio);
        if (buttonState == LOW && !ch2_press) {
          DBG_OUT_PORT.println(F("Button 2 pressed.."));
          ch1_set = !ch1_set;
          ch1_auto = false;  // turn OFF the AUTO MODE for ch1
          ch2_press = true;
          bumpless = true;
          setting_ms = millis();
        }
        if (buttonState == HIGH && ch2_press) ch2_press = false;
      }
    } else {
      if (ch2_auto && ft > conf_data.lim_h && ch2_set) ch2_set = false;
      if (ch2_auto && ft < conf_data.lim_l && !ch2_set) ch2_set = true;

      ch2_val_buf = 0;
      if (ch2_set) conf_data.ch2_dig ? ch2_val_buf = 255 : ch2_val_buf = ch2_val;

      analogWrite(conf_data.ch2_gpio, ch2_val_buf);
    }
  }

  if ((millis() - setting_ms > 200) && bumpless) bumpless = !bumpless;

  //    delay (100);
}
