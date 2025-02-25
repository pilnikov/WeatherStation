#include "Scr.h"

String MSG::callback(uint8_t dsp_t, uint8_t type, uint8_t stage, bool rus)
{
  String _ret = String();
  switch (stage)
  {
    case 0:
      switch (dsp_t)
      {
        case 0:
          switch (type)
          {
            case 0:
              _ret = "True sync with NTP server. Please wait!";
              break;
            case 1:
              _ret = "True Put data to TS. Please wait!";
              break;
            case 2:
              _ret = "True Get data from TS. Please wait!";
              break;
            case 3:
              _ret = "True connect to WiFi. Please wait!";
              break;
            case 4:
              _ret = "True create a AP. Please wait!";
              break;
			default:
			  break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              _ret = "Sync with NTP ";
              if (rus) _ret = "Сверяемся c НТП ";
              break;
            case 1:
              _ret = "Put data to TS ";
              if (rus) _ret = "Шлем инфу на TS ";
              break;
            case 2:
              _ret = "Get data from TS ";
              if (rus) _ret = "Берем инфу c TS ";
              break;
            case 3:
              _ret = "True con to WiFi ";
              if (rus) _ret = "Цепляемся к WiFi ";
              break;
            case 4:
              _ret = "True create a AP ";
              if (rus) _ret = "Создаем Точку ";
              break;
			default:
			  break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              _ret = "HtП-";
              break;
            case 1:
              _ret = "tS >";
              break;
            case 2:
              _ret = "< tS";
              break;
            case 3:
              _ret = "< CL";
              break;
            case 4:
              _ret = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              _ret = "HtП-";
              break;
            case 1:
              _ret = "tS >";
              break;
            case 2:
              _ret = "< tS";
              break;
            case 3:
              _ret = "< CL";
              break;
            case 4:
              _ret = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              _ret = "<-NTP";
              break;
            case 1:
              _ret = "TS ->";
              break;
            case 2:
              _ret = "TS ->";
              break;
            case 3:
              _ret = "<- CL";
              break;
            case 4:
              _ret = "AP ->";
              break;
 			default:
			  break;
         }
          break;
        case 5:
          switch (type)
          {
            case 0:
              _ret = "HtП-";
              break;
            case 1:
              _ret = "tS >";
              break;
            case 2:
              _ret = "< tS";
              break;
            case 3:
              _ret = "< CL";
              break;
            case 4:
              _ret = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              _ret = "HtП-";
              break;
            case 1:
              _ret = "tS >";
              break;
            case 2:
              _ret = "< tS";
              break;
            case 3:
              _ret = "< CL";
              break;
            case 4:
              _ret = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              _ret = "HtП-";
              break;
            case 1:
              _ret = "tS >";
              break;
            case 2:
              _ret = "< tS";
              break;
            case 3:
              _ret = "< CL";
              break;
            case 4:
              _ret = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              _ret = "<-NTP";
              break;
            case 1:
              _ret = "TS ->";
              break;
            case 2:
              _ret = "TS ->";
              break;
            case 3:
              _ret = "<- CL";
              break;
            case 4:
              _ret = "AP ->";
              break;
			default:
			  break;
          }
          break;
        default:
          break;
      }
      break;

    case 1:
      switch (dsp_t)
      {
        case 0:
          switch (type)
          {
            case 0:
              _ret = "Sync with NTP server. Sucsess!";
              break;
            case 1:
              _ret = "Put data to TS. Sucsess!";
              break;
            case 2:
              _ret = "Get data from TS. Sucsess!";
              break;
            case 3:
              _ret = "Connect to WiFi. Sucsess!";
              break;
            case 4:
              _ret = "Create a AP. Sucsess!";
              break;
			default:
			  break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              _ret = "Time from NTP OK ";
              if (rus) _ret = "Сверка c НТП OK ";
              break;
            case 1:
              _ret = "Data to TS OK! ";
              if (rus) _ret = "Инфа на TS OK! ";
              break;
            case 2:
              _ret = "Data from TS OK! ";
              if (rus) _ret = "Инфа c TS OK! ";
              break;
            case 3:
              _ret = "WiFi connected! ";
              if (rus) _ret = "WiFi подключено! ";
              break;
            case 4:
              _ret = "AP created! ";
              if (rus) _ret = "Точка создана! ";
              break;
			default:
			  break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              _ret = "HtПu";
              break;
            case 1:
              _ret = "tS>u";
              break;
            case 2:
              _ret = "<tSu";
              break;
            case 3:
              _ret = "<CLu";
              break;
            case 4:
              _ret = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              _ret = "HtПu";
              break;
            case 1:
              _ret = "tS>u";
              break;
            case 2:
              _ret = "<tSu";
              break;
            case 3:
              _ret = "<CLu";
              break;
            case 4:
              _ret = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              _ret = "HtПu";
              break;
            case 1:
              _ret = "tS>u";
              break;
            case 2:
              _ret = "<tSu";
              break;
            case 3:
              _ret = "<CLu";
              break;
            case 4:
              _ret = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 5:
          switch (type)
          {
            case 0:
              _ret = "HtПu";
              break;
            case 1:
              _ret = "tS>u";
              break;
            case 2:
              _ret = "<tSu";
              break;
            case 3:
              _ret = "<CLu";
              break;
            case 4:
              _ret = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              _ret = "HtПu";
              break;
            case 1:
              _ret = "tS>u";
              break;
            case 2:
              _ret = "<tSu";
              break;
            case 3:
              _ret = "<CLu";
              break;
            case 4:
              _ret = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              _ret = "HtПu";
              break;
            case 1:
              _ret = "tS>u";
              break;
            case 2:
              _ret = "<tSu";
              break;
            case 3:
              _ret = "<CLu";
              break;
            case 4:
              _ret = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              _ret = "<-NTP";
              break;
            case 1:
              _ret = "TS ->";
              break;
            case 2:
              _ret = "TS ->";
              break;
            case 3:
              _ret = "<- CL";
              break;
            case 4:
              _ret = "AP ->";
              break;
			default:
			  break;
          }
          break;
        default:
          break;
      }
      break;

    case 2:
      switch (dsp_t)
      {
        case 0:
          switch (type)
          {
            case 0:
              _ret = "\nSync with NTP server failed!";
              break;
            case 1:
              _ret = "\nPut data to TS failed!";
              break;
            case 2:
              _ret = "\nGet data from TS failed!";
              break;
            case 3:
              _ret = "\nConnect to WiFi as client failed!";
              break;
            case 4:
              _ret = "\nCreate a AP failed!";
              break;
			default:
			  break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              _ret = "NTP Fail!";
              if (rus) _ret = "НТП Косяк! ";
              break;
            case 1:
              _ret = "Put TS Fail!";
              if (rus) _ret = "TS-> Косяк! ";
              break;
            case 2:
              _ret = "Get TS Fail!";
              if (rus) _ret = "<- TS Косяк! ";
              break;
            case 3:
              _ret = "Client WiFi Fail";
              if (rus) _ret = "<- WiFi Косяк! ";
              break;
            case 4:
              _ret = "AP Fail!";
              if (rus) _ret = "AP -> Косяк! ";
              break;
			default:
			  break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              _ret = "HtПn";
              break;
            case 1:
              _ret = "tS n";
              break;
            case 2:
              _ret = "n tS";
              break;
            case 3:
              _ret = "n CL";
              break;
            case 4:
              _ret = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              _ret = "HtПn";
              break;
            case 1:
              _ret = "tS n";
              break;
            case 2:
              _ret = "n tS";
              break;
            case 3:
              _ret = "n CL";
              break;
            case 4:
              _ret = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              _ret = "NTP-F";
              break;
            case 1:
              _ret = "F<-TS";
              break;
            case 2:
              _ret = "TS->F";
              break;
            case 3:
              _ret = "F<-CL";
              break;
            case 4:
              _ret = "AP->F";
              break;
			default:
			  break;
          }
          break;
        case 5:
          switch (type)
          {
            case 0:
              _ret = "HtПn";
              break;
            case 1:
              _ret = "tS n";
              break;
            case 2:
              _ret = "n tS";
              break;
            case 3:
              _ret = "n CL";
              break;
            case 4:
              _ret = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              _ret = "HtПn";
              break;
            case 1:
              _ret = "tS n";
              break;
            case 2:
              _ret = "n tS";
              break;
            case 3:
              _ret = "n CL";
              break;
            case 4:
              _ret = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              _ret = "HtПn";
              break;
            case 1:
              _ret = "tS n";
              break;
            case 2:
              _ret = "n tS";
              break;
            case 3:
              _ret = "n CL";
              break;
            case 4:
              _ret = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              _ret = "NTP-F";
              break;
            case 1:
              _ret = "F<-TS";
              break;
            case 2:
              _ret = "TS->F";
              break;
            case 3:
              _ret = "F<-CL";
              break;
            case 4:
              _ret = "AP->F";
              break;
			default:
			  break;
          }
          break;
        default:
          break;
      }
      break;

    default:
      break;
  }
  return _ret;
}

String MSG::alarm_msg(uint8_t alm_n, uint8_t dsp_t, bool rus)
{
  String _ret = String();
  switch (dsp_t) // Отображаем инфу на выбранном дисплее
  {
    case 0:
      _ret = ("\nAlarm #" +  String(alm_n) + " is triggered!!!\n");
      break;
    case 1:
      _ret = "Alarm #" + String(alm_n) + " ";
      if (rus) _ret = "Будильник N" + String(alm_n);
      break;
    case 2:
      _ret = "AL-" + String(alm_n);
      break;
    case 3:
      _ret = "AL-" + String(alm_n);
      break;
    case 4:
      _ret = "AL-" + String(alm_n);
      break;
    case 5:
      _ret = "AL-" + String(alm_n);
      break;
    case 6:
      _ret = "AL-" + String(alm_n);
      break;
    case 7:
      _ret = "AL-" + String(alm_n);
      break;
    case 8:
      _ret = "AL-" + String(alm_n);
      break;
    default:
	  break;
  }
 return _ret;
}

