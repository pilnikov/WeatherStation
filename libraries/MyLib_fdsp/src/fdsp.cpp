
#include "Fdsp.h"

String FD::td_name_r(uint8_t tod ) // Время суток по русски
{
  String td = String();
  switch (tod)
  {
    case 0:
      td = " ночь ";
      break;
    case 1:
      td = " yтро ";
      break;
    case 2:
      td = " день ";
      break;
    case 3:
      td = " вечер ";
      break;
  }
  return td;
}

String FD::wind_dir_r(uint16_t wind_dir) // Направление ветра по русски
{
  String wn = String();

  switch (wind_dir)
  {
    case 0:
      wn = " северный ";
      break;
    case 1:
      wn = " северо-восточный ";
      break;
    case 2:
      wn = " восточный ";
      break;
    case 3:
      wn = " юго-восточный ";
      break;
    case 4:
      wn = " южный ";
      break;
    case 5:
      wn = " юго-западный ";
      break;
    case 6:
      wn = " западный ";
      break;
    case 7:
      wn = " северо-западный ";
      break;
  }
  return wn;
}

String FD::prc_type_r(uint8_t prec, bool sp, bool rp) // Тип осадков по русски
{
  String prc = String();
  switch (prec)
  {
    case 4:
      prc = (!rp ? "возможен " : "");
      prc += "дождь ";
      break;
    case 5:
      prc = (!rp ? "возможен " : "");
      prc += "ливень ";
      break;
    case 6:
      prc = (!rp ? "возможен " : "");
      prc += "снегопад ";
      break;
    case 7:
      prc = (!rp ? "возможен " : "");
      prc += "сильный снегопад ";
      break;
    case 8:
      prc = (!sp ? "возможна " : "");
      prc += "гроза ";
      break;
    case 10:
      prc = "без осадков ";
      break;
  }
  return prc;
}

String FD::dow_name_r(uint8_t wd) // Название Дня недели по русски
{
  String sDOW = String();
  switch (wd)
  {
    case 2:
      sDOW = " понедельник ";
      break;
    case 3:
      sDOW = " вторник ";
      break;
    case 4:
      sDOW = " среда ";
      break;
    case 5:
      sDOW = " четверг ";
      break;
    case 6:
      sDOW = " пятница ";
      break;
    case 7:
      sDOW = " суббота ";
      break;
    case 1:
      sDOW = " воскресенье ";
      break;
  }
  return sDOW;
}

String FD::month_name_r(uint8_t mn) // Название Месяца по русски
{
  String sMON = String();
  switch (mn)
  {
    case 1:
      sMON = " января ";
      break;
    case 2:
      sMON = " февраля ";
      break;
    case 3:
      sMON = " марта ";
      break;
    case 4:
      sMON = " апреля ";
      break;
    case 5:
      sMON = " мая ";
      break;
    case 6:
      sMON = " июня ";
      break;
    case 7:
      sMON = " июля ";
      break;
    case 8:
      sMON = " августа ";
      break;
    case 9:
      sMON = " сентября ";
      break;
    case 10:
      sMON = " октября ";
      break;
    case 11:
      sMON = " ноября ";
      break;
    case 12:
      sMON = " декабря ";
      break;
  }
  return sMON;
}

//-------------------------------------------------------------- Форматирование в одно знакоместо
String FD::render_number(int number)
{
  char msg[2];
  sprintf(msg, "%1u", number);
  return msg;
}

//-------------------------------------------------------------- Форматирование в  два знакоместа
String FD::render_00number(int number)
{
  char msg[2];
  sprintf(msg, "%02u", number);
  return msg;
}

//-------------------------------------------------------------- Формирование строки с текущим временем для LCD
String FD::form_time_string (uint8_t hr, uint8_t mn, uint8_t sec, uint8_t a_hour, uint8_t a_min, bool alm, bool rus)
{
  char msg[20];
  if (alm)
  {
    if (rus) sprintf(msg, " %2u:%02u:%02u %2u:%02u\355", hr, mn, sec, a_hour, a_min);
    else     sprintf(msg, " %2u:%02u:%02u %2u:%02u",     hr, mn, sec, a_hour, a_min);
  }
  else       sprintf(msg, " %2u:%02u:%02u  -:-  ",       hr, mn, sec);

  return msg;
}

//-------------------------------------------------------------- Формирование строки IP адреса
String FD::IP_to_str (byte ad0, byte ad1, byte ad2, byte ad3)
{
  char msg[15];
  sprintf(msg, "%d.%d.%d.%d", ad0, ad1, ad2, ad3);
  return msg;
}
