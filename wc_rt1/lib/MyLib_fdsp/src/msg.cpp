#include "Fdsp.h"

String MSG::callback(uint8_t disp, uint8_t type, uint8_t stage, bool rus)
{
  String bstr = String();
  switch (stage)
  {
    case 0:
      switch (disp)
      {
        case 0:
          switch (type)
          {
            case 0:
              bstr = "True sync with NTP server. Please wait!";
              break;
            case 1:
              bstr = "True Put data to TS. Please wait!";
              break;
            case 2:
              bstr = "True Get data from TS. Please wait!";
              break;
            case 3:
              bstr = "True connect to WiFi. Please wait!";
              break;
            case 4:
              bstr = "True create a AP. Please wait!";
              break;
			default:
			  break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              bstr = "Sync with NTP ";
              if (rus) bstr = "Сверяемся c НТП ";
              break;
            case 1:
              bstr = "Put data to TS ";
              if (rus) bstr = "Шлем инфу на TS ";
              break;
            case 2:
              bstr = "Get data from TS ";
              if (rus) bstr = "Берем инфу c TS ";
              break;
            case 3:
              bstr = "True con to WiFi ";
              if (rus) bstr = "Цепляемся к WiFi ";
              break;
            case 4:
              bstr = "True create a AP ";
              if (rus) bstr = "Создаем Точку ";
              break;
			default:
			  break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
              break;
 			default:
			  break;
         }
          break;
        case 5:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              bstr = "HtП-";
              break;
            case 1:
              bstr = "tS >";
              break;
            case 2:
              bstr = "< tS";
              break;
            case 3:
              bstr = "< CL";
              break;
            case 4:
              bstr = "AP >";
              break;
			default:
			  break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
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
      switch (disp)
      {
        case 0:
          switch (type)
          {
            case 0:
              bstr = "Sync with NTP server. Sucsess!";
              break;
            case 1:
              bstr = "Put data to TS. Sucsess!";
              break;
            case 2:
              bstr = "Get data from TS. Sucsess!";
              break;
            case 3:
              bstr = "Connect to WiFi. Sucsess!";
              break;
            case 4:
              bstr = "Create a AP. Sucsess!";
              break;
			default:
			  break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              bstr = "Time from NTP OK ";
              if (rus) bstr = "Сверка c НТП OK ";
              break;
            case 1:
              bstr = "Data to TS OK! ";
              if (rus) bstr = "Инфа на TS OK! ";
              break;
            case 2:
              bstr = "Data from TS OK! ";
              if (rus) bstr = "Инфа c TS OK! ";
              break;
            case 3:
              bstr = "WiFi connected! ";
              if (rus) bstr = "WiFi подключено! ";
              break;
            case 4:
              bstr = "AP created! ";
              if (rus) bstr = "Точка создана! ";
              break;
			default:
			  break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 5:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              bstr = "HtПu";
              break;
            case 1:
              bstr = "tS>u";
              break;
            case 2:
              bstr = "<tSu";
              break;
            case 3:
              bstr = "<CLu";
              break;
            case 4:
              bstr = "AP>u";
              break;
			default:
			  break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              bstr = "<-NTP";
            case 1:
              bstr = "TS ->";
              break;
            case 2:
              bstr = "TS ->";
              break;
            case 3:
              bstr = "<- CL";
              break;
            case 4:
              bstr = "AP ->";
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
      switch (disp)
      {
        case 0:
          switch (type)
          {
            case 0:
              bstr = "\nSync with NTP server failed!";
              break;
            case 1:
              bstr = "\nPut data to TS failed!";
              break;
            case 2:
              bstr = "\nGet data from TS failed!";
              break;
            case 3:
              bstr = "\nConnect to WiFi as client failed!";
              break;
            case 4:
              bstr = "\nCreate a AP failed!";
              break;
			default:
			  break;
          }
          break;
        case 1:
          switch (type)
          {
            case 0:
              bstr = "NTP Fail!";
              if (rus) bstr = "НТП Косяк! ";
              break;
            case 1:
              bstr = "Put TS Fail!";
              if (rus) bstr = "TS-> Косяк! ";
              break;
            case 2:
              bstr = "Get TS Fail!";
              if (rus) bstr = "<- TS Косяк! ";
              break;
            case 3:
              bstr = "Client WiFi Fail";
              if (rus) bstr = "<- WiFi Косяк! ";
              break;
            case 4:
              bstr = "AP Fail!";
              if (rus) bstr = "AP -> Косяк! ";
              break;
			default:
			  break;
          }
          break;
        case 2:
          switch (type)
          {
            case 0:
              bstr = "HtПn";
              break;
            case 1:
              bstr = "tS n";
              break;
            case 2:
              bstr = "n tS";
              break;
            case 3:
              bstr = "n CL";
              break;
            case 4:
              bstr = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 3:
          switch (type)
          {
            case 0:
              bstr = "HtПn";
              break;
            case 1:
              bstr = "tS n";
              break;
            case 2:
              bstr = "n tS";
              break;
            case 3:
              bstr = "n CL";
              break;
            case 4:
              bstr = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 4:
          switch (type)
          {
            case 0:
              bstr = "NTP-F";
            case 1:
              bstr = "F<-TS";
              break;
            case 2:
              bstr = "TS->F";
              break;
            case 3:
              bstr = "F<-CL";
              break;
            case 4:
              bstr = "AP->F";
              break;
			default:
			  break;
          }
          break;
        case 5:
          switch (type)
          {
            case 0:
              bstr = "HtПn";
              break;
            case 1:
              bstr = "tS n";
              break;
            case 2:
              bstr = "n tS";
              break;
            case 3:
              bstr = "n CL";
              break;
            case 4:
              bstr = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 6:
          switch (type)
          {
            case 0:
              bstr = "HtПn";
              break;
            case 1:
              bstr = "tS n";
              break;
            case 2:
              bstr = "n tS";
              break;
            case 3:
              bstr = "n CL";
              break;
            case 4:
              bstr = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 7:
          switch (type)
          {
            case 0:
              bstr = "HtПn";
              break;
            case 1:
              bstr = "tS n";
              break;
            case 2:
              bstr = "n tS";
              break;
            case 3:
              bstr = "n CL";
              break;
            case 4:
              bstr = "AP n";
              break;
			default:
			  break;
          }
          break;
        case 8:
          switch (type)
          {
            case 0:
              bstr = "NTP-F";
            case 1:
              bstr = "F<-TS";
              break;
            case 2:
              bstr = "TS->F";
              break;
            case 3:
              bstr = "F<-CL";
              break;
            case 4:
              bstr = "AP->F";
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
  return bstr;
}

String MSG::alarm_msg(uint8_t a_num, uint8_t disp, bool rus)
{
  String bstr = String();
  switch (disp) // Отображаем инфу на выбранном дисплее
  {
    case 0:
      bstr = ("\nAlarm #" +  String(a_num) + " is triggered!!!\n");
      break;
    case 1:
      bstr = "Alarm #" + String(a_num) + " ";
      if (rus) bstr = "Будильник N" + String(a_num);
      break;
    case 2:
      bstr = "AL-" + String(a_num);
      break;
    case 3:
      bstr = "AL-" + String(a_num);
      break;
    case 4:
      bstr = "AL-" + String(a_num);
      break;
    case 5:
      bstr = "AL-" + String(a_num);
      break;
    case 6:
      bstr = "AL-" + String(a_num);
      break;
    case 7:
      bstr = "AL-" + String(a_num);
      break;
    case 8:
      bstr = "AL-" + String(a_num);
      break;
    default:
	  break;
  }
 return bstr;
}

