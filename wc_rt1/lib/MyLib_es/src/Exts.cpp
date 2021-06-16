#include "Exts.h"

snr_data_t ES::get_ts(String inStr)
{
	// inStr {"created_at":"2017-06-07T08:32:19Z","entry_id":4,"field1":"24","field2":"23","field3":"29","field4":"40","field5":"735","field6":null,"field7":null,"field8":null}

	snr_data_t data;

	//  data.h1 = 0; data.t1 = 99; data.h2 = 0; data.t2 = 99; data.h3 = 0; data.t3 = 99; data.p = 0;

	DynamicJsonDocument jsonBuffer(512);

	DeserializationError error = deserializeJson(jsonBuffer, inStr);
	if (error)
	{
		DBG_OUT_PORT.println(F("Read data from TS failed!!! Check connections."));
	}
	else
	{
		JsonObject root = jsonBuffer.as<JsonObject>();

		data.t1 = root["field1"];
		data.t2 = root["field2"];
		data.t3 = root["field3"];
		data.h1 = root["field4"];
		data.h2 = root["field5"];
		data.h3 = root["field6"];
		 data.p = root["field7"];

		if (data.p > 700)
		{
			DBG_OUT_PORT.println("Read data from TS");
			DBG_OUT_PORT.print("TC1: ");
			DBG_OUT_PORT.print(data.t1);
			DBG_OUT_PORT.print(" C HC1: ");
			DBG_OUT_PORT.print(data.h1);
			DBG_OUT_PORT.println("%");
			DBG_OUT_PORT.print("TC2: ");
			DBG_OUT_PORT.print(data.t2);
			DBG_OUT_PORT.print(" C HC2: ");
			DBG_OUT_PORT.print(data.h2);
			DBG_OUT_PORT.println("%");
			DBG_OUT_PORT.print("TC3: ");
			DBG_OUT_PORT.print(data.t3);
			DBG_OUT_PORT.print(" C HC3: ");
			DBG_OUT_PORT.print(data.h3);
			DBG_OUT_PORT.println("%");
			DBG_OUT_PORT.print("Press: ");
			DBG_OUT_PORT.print(data.p);
			DBG_OUT_PORT.println("mm rt. st.");
		}
		else   DBG_OUT_PORT.println("Failed!");
	}
	return data;
}


String ES::put_ts(char api[17], bool use_tst1, bool use_tst2, bool use_tst3, bool use_tsh1, bool use_tsh2, bool use_tsh3, bool use_tsp, snr_data_t in_data)
{
	//String postStr = "https://api.thingspeak.com/update?api_key=" + String(api);
	String postStr = "/update?api_key=" + String(api);

	if (use_tst1 && in_data.t1 < 99)
	{
		postStr += "&field1=";
		postStr += String(in_data.t1);
	}

	if (use_tst1 && in_data.t2 < 99)
	{
		postStr += "&field2=";
		postStr += String(in_data.t2);
	}

	if (use_tst3 && in_data.t3 < 99)
	{
		postStr += "&field3=";
		postStr += String(in_data.t3);
	}

	if (use_tsh1 && in_data.h1 > 0 && in_data.h1 < 100)
	{
		postStr += "&field4=";
		postStr += String(in_data.h1);
	}

	if (use_tsh2 && in_data.h2 > 0 && in_data.h2 < 100)
	{
		postStr += "&field5=";
		postStr += String(in_data.h2);
	}

	if (use_tsh3 && in_data.h3 > 0 && in_data.h3 < 100)
	{
		postStr += "&field6=";
		postStr += String(in_data.h3);
	}

	if (use_tsp && in_data.p > 700 && in_data.p < 900)
	{
		postStr += "&field7=";
		postStr += String(in_data.p);
	}
	postStr += "\r\n\r\n";

	DBG_OUT_PORT.println(postStr);
	return postStr;
}

/*<MMWEATHER>
  <REPORT type="frc3">
  <TOWN index="28438" sname="%CC%E8%F5%E0%E9%EB%EE%E2%F1%EA" latitude="56" longitude="59">
  <FORECAST day="17" month="11" year="2016" hour="20" tod="3" predict="0" weekday="5">
  <PHENOMENA cloudiness="3" precipitation="10" rpower="0" spower="0"/>
  <PRESSURE max="756" min="754"/>
  <TEMPERATURE max="-19" min="-17"/>
  <WIND min="1" max="3" direction="2"/>
  <RELWET max="81" min="79"/>
  <HEAT min="-22" max="-20"/>
  </FORECAST>
  <FORECAST day="18" month="11" year="2016" hour="02" tod="0" predict="0" weekday="6">
  <PHENOMENA cloudiness="3" precipitation="10" rpower="0" spower="0"/>
  <PRESSURE max="756" min="754"/>
  <TEMPERATURE max="-20" min="-18"/>
  <WIND min="1" max="3" direction="2"/>
  <RELWET max="79" min="77"/>
  <HEAT min="-21" max="-19"/>
  </FORECAST>
  <FORECAST day="18" month="11" year="2016" hour="08" tod="1" predict="0" weekday="6">
  <PHENOMENA cloudiness="3" precipitation="10" rpower="0" spower="0"/>
  <PRESSURE max="758" min="756"/>
  <TEMPERATURE max="-21" min="-19"/>
  <WIND min="1" max="3" direction="2"/>
  <RELWET max="80" min="78"/>
  <HEAT min="-24" max="-22"/>
  </FORECAST>
  <FORECAST day="18" month="11" year="2016" hour="14" tod="2" predict="0" weekday="6">
  <PHENOMENA cloudiness="3" precipitation="10" rpower="0" spower="0"/>
  <PRESSURE max="759" min="757"/>
  <TEMPERATURE max="-17" min="-15"/>
  <WIND min="1" max="3" direction="2"/>
  <RELWET max="68" min="66"/>
  <HEAT min="-20" max="-18"/>
  </FORECAST>
  </TOWN>
  </REPORT>
  </MMWEATHER>

  Теперь посмотрим на описание формата:

	TOWN - информация о пункте прогнозирования:
	index - уникальный пятизначный код города
	sname - закодированное название города
	latitude - широта в целых градусах
	longitude - долгота в целых градусах

	FORECAST - информация о сроке прогнозирования:
	day, month, year - дата, на которую составлен прогноз в данном блоке
	hour - местное время, на которое составлен прогноз
	tod - время суток, для которого составлен прогноз: 0 - ночь 1 - утро, 2 - день, 3 - вечер
	weekday - день недели, 1 - воскресенье, 2 - понедельник, и т.д.
	predict - заблаговременность прогноза в часах

	PHENOMENA - атмосферные явления:
	cloudiness - облачность по градациям: 0 - ясно, 1- малооблачно, 2 - облачно, 3 - пасмурно
	precipitation - тип осадков: 4 - дождь, 5 - ливень, 6,7 – снег, 8 - гроза, 9 - нет данных, 10 - без осадков
	rpower - интенсивность осадков, если они есть. 0 - возможен дождь/снег, 1 - дождь/снег
	spower - вероятность грозы, если прогнозируется: 0 - возможна гроза, 1 - гроза

	PRESSURE - атмосферное давление, в мм.рт.ст.
	TEMPERATURE - температура воздуха, в градусах Цельсия
	WIND - приземный ветер min, max минимальное и максимальное значения средней скорости ветра, без порывов
	direction - направление ветра в румбах, 0 - северный, 1 - северо-восточный, и т.д.
	RELWET - относительная влажность воздуха, в %
	HEAT - комфорт - температура воздуха по ощущению одетого по сезону человека, выходящего на улицу
*/
wf_data_t ES::get_gm(String inStr)
{
	wf_data_t prog[4];
	char inChr; //Обрабатываемый символ
	byte i = 0;
	bool tag_t = false, tag_v = false;

	String nStr = String(); // Имя переменной
	String vStr = String(); // значение
	String tStr = String(); // Имя тега

	for (uint16_t n = 0; n < inStr.length(); n++)
	{
		if (!(inStr[n] == '"' || inStr[n] == '/'))
		{
			inChr = inStr[n];
			if (inStr[n] == ' ' || inStr[n] == '>')
			{
				if (vStr > "")
				{
					//DBG_OUT_PORT.print("name...") ; DBG_OUT_PORT.println(nStr);
					//DBG_OUT_PORT.print("value..."); DBG_OUT_PORT.println(vStr);

					byte vInd = 0;
					if (tStr == "FORECAST" && nStr == "day")           vInd = 1;
					else if (tStr == "FORECAST" && nStr == "month")         vInd = 2;
					else if (tStr == "FORECAST" && nStr == "year")          vInd = 3;
					else if (tStr == "FORECAST" && nStr == "tod")           vInd = 4;
					else if (tStr == "PHENOMENA" && nStr == "cloudiness")    vInd = 5;
					else if (tStr == "PHENOMENA" && nStr == "precipitation") vInd = 6;
					else if (tStr == "PHENOMENA" && nStr == "rpower")        vInd = 7;
					else if (tStr == "PHENOMENA" && nStr == "spower")        vInd = 8;
					else if (tStr == "PRESSURE" && nStr == "max")           vInd = 9;
					else if (tStr == "PRESSURE" && nStr == "min")           vInd = 10;
					else if (tStr == "TEMPERATURE" && nStr == "max")           vInd = 11;
					else if (tStr == "TEMPERATURE" && nStr == "min")           vInd = 12;
					else if (tStr == "WIND" && nStr == "max")           vInd = 13;
					else if (tStr == "WIND" && nStr == "min")           vInd = 14;
					else if (tStr == "WIND" && nStr == "direction")     vInd = 15;
					else if (tStr == "RELWET" && nStr == "max")           vInd = 16;
					else if (tStr == "RELWET" && nStr == "min")           vInd = 17;
					else if (tStr == "HEAT" && nStr == "min")           vInd = 18;
					else if (tStr == "HEAT" && nStr == "max")           vInd = 19;

					//DBG_OUT_PORT.print("vInd..."); DBG_OUT_PORT.println(vInd);
					if (vInd > 0) DBG_OUT_PORT.print(' ');

					switch (vInd)
					{
					case 1:
						prog[i].day = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].day);
						break;
					case 2:
						prog[i].month = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].month);
						break;
					case 3:
						prog[i].year = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].year);
						break;
					case 4:
						prog[i].tod = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].tod);
						break;
					case 5:
						prog[i].cloud = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].cloud);
						break;
					case 6:
						prog[i].prec = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].prec);
						break;
					case 7:
						prog[i].rpower = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].rpower);
						break;
					case 8:
						prog[i].spower = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].spower);
						break;
					case 9:
						prog[i].press_max = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].press_max);
						break;
					case 10:
						prog[i].press_min = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].press_min);
						break;
					case 11:
						prog[i].temp_max = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].temp_max);
						break;
					case 12:
						prog[i].temp_min = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].temp_min);
						break;
					case 13:
						prog[i].wind_min = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].wind_min);
						break;
					case 14:
						prog[i].wind_max = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].wind_max);
						break;
					case 15:
						prog[i].wind_dir = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].wind_dir);
						break;
					case 16:
						prog[i].hum_max = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].hum_max);
						break;
					case 17:
						prog[i].hum_min = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].hum_min);
						break;
					case 18:
						prog[i].heat_min = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].heat_min);
						break;
					case 19:
						prog[i].heat_max = vStr.toInt();
						//DBG_OUT_PORT.print(prog[i].heat_max);
						break;
					}
				}

				if (tStr == "HEAT" && inStr[n] == '>')
				{
					i++;
					DBG_OUT_PORT.println();
				}
				nStr = ""; // clear the string "low name" for new input:
				vStr = ""; // clear the string "value"    for new input:
				tag_v = false;
				tag_t = false;
			}
			else
			{
				if (tag_t && isAlpha(inChr) && isUpperCase(inChr)) tStr += (char)inChr; //Формируем Имя тега
				else
				{
					if (!tag_v && isAlpha(inChr) && isLowerCase(inChr)) nStr += (char)inChr; //Формируем Имя переменной
					else
					{
						if (tag_v && (isDigit(inChr) || inChr == '-')) vStr += (char)inChr; //Формируем значение
						if (inStr[n] == '=') tag_v = true;
					}
					if (inStr[n] == '<')
					{
						//DBG_OUT_PORT.print("Tag...");
						//DBG_OUT_PORT.println(tStr);
						tStr = "";
						tag_t = true;
					}
				}
			}
		}
	}

	i = 3;

	DBG_OUT_PORT.println("\nData from GM /\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/");
	DBG_OUT_PORT.print(" Date of forecasts..");
	DBG_OUT_PORT.print(prog[i].day);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].month);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].year);
	DBG_OUT_PORT.print(": tod..");
	DBG_OUT_PORT.print(prog[i].tod);
	DBG_OUT_PORT.print("\n cloud..");
	DBG_OUT_PORT.print(prog[i].cloud);
	DBG_OUT_PORT.print("\n prec..");
	DBG_OUT_PORT.print(prog[i].prec);
	DBG_OUT_PORT.print(" rpower..");
	DBG_OUT_PORT.print(prog[i].rpower);
	DBG_OUT_PORT.print(" spower..");
	DBG_OUT_PORT.print(prog[i].spower);
	DBG_OUT_PORT.print("\n press..");
	DBG_OUT_PORT.print(prog[i].press_max);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].press_min);
	DBG_OUT_PORT.print("\n temp..");
	DBG_OUT_PORT.print(prog[i].temp_max);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].temp_min);
	DBG_OUT_PORT.print("\n wind speed..");
	DBG_OUT_PORT.print(prog[i].wind_min);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].wind_max);
	DBG_OUT_PORT.print(" dir..");
	DBG_OUT_PORT.print(prog[i].wind_dir);
	DBG_OUT_PORT.print("\n hum..");
	DBG_OUT_PORT.print(prog[i].hum_max);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].hum_min);
	DBG_OUT_PORT.print("\n heat..");
	DBG_OUT_PORT.print(prog[i].heat_min);
	DBG_OUT_PORT.print(":");
	DBG_OUT_PORT.print(prog[i].heat_max);
	DBG_OUT_PORT.println("\n/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/");

	return prog[i];
}

snr_data_t ES::get_es(String inStr)
{
	snr_data_t data;
	data.h1 = 0; data.h2 = 0; data.h3 = 0; data.t1 = 99; data.t2 = 99; data.t3 = 99; data.p = 700; data.f = 255;

	DynamicJsonDocument jsonBuffer(1024);

	DeserializationError error = deserializeJson(jsonBuffer, inStr);
	if (error)
	{
		DBG_OUT_PORT.println(F("Read data from Ext server failed!!! Check connections."));
	}
	else
	{

		JsonObject root = jsonBuffer.as<JsonObject>();

		data.t1 = root["t1"];
		data.t2 = root["t2"];
		data.t3 = root["t3"];
		data.h1 = root["h1"];
		data.h2 = root["h2"];
		data.h3 = root["h3"];
		data.p = root["pres"];
		data.f = root["brig"];


		DBG_OUT_PORT.println("Data from ext server");
		DBG_OUT_PORT.print("TC1: ");
		DBG_OUT_PORT.print(data.t1);
		DBG_OUT_PORT.print(" C HC1: ");
		DBG_OUT_PORT.print(data.h1);
		DBG_OUT_PORT.println("%");
		DBG_OUT_PORT.print("TC2: ");
		DBG_OUT_PORT.print(data.t2);
		DBG_OUT_PORT.print(" C HC2: ");
		DBG_OUT_PORT.print(data.h2);
		DBG_OUT_PORT.println("%");
		DBG_OUT_PORT.print("TC3: ");
		DBG_OUT_PORT.print(data.t3);
		DBG_OUT_PORT.print(" C HC3: ");
		DBG_OUT_PORT.print(data.h3);
		DBG_OUT_PORT.println("%");
		DBG_OUT_PORT.print("Press: ");
		DBG_OUT_PORT.print(data.p);
		DBG_OUT_PORT.println(" mm rt. st.");
		DBG_OUT_PORT.print("Bright: ");
		DBG_OUT_PORT.print(data.f);
		DBG_OUT_PORT.println(" lum.");
	}
	return data;
}
