
#ifndef Fdsp_h
#define Fdsp_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class FFF // Функции для формирования кадра (FrameFormerFunctions)
{
  public:
  void
	roll_seg (byte&),		// Переворот сегмента
	mir_seg (byte&), 		// Адаптация для мах7219
	prn7(String&),   		// Печать на семисегментном дисплее
	utf8rus(char*),			// Конвертация из UTF8 в СР1251 для матриц
	utf8rus(String&),		// Конвертация из UTF8 в СР1251 для матриц
	lcd_rus(char*),			// Конвертация из UTF8 в СР1251 для LCD
	lcd_rus(String&),		// Конвертация из UTF8 в СР1251 для LCD
	CLS(byte*, size_t),		// Очистка массива
	cleanPos(uint8_t, byte*),	// Очистка выбранной позиции в массиве
	printDot(uint8_t, byte*),	// Печать точки (для сегментных индикаторов)
	printCharacter(unsigned char, uint8_t, byte*, const byte*, uint8_t),   // Печать одного символа в буфер
	print_(char*, uint8_t, byte*, uint8_t, const byte*, uint8_t, uint8_t), // Печать строки из символов в буфер
	compressor7(byte*, uint8_t, uint8_t),
	h_scroll_init(),			 /* Устанавливает указатели на первый символ в прокручиваемой строке и на первый байт в буфере */
	h_scroll(bool, bool, uint8_t, uint8_t, bool, bool&, char*, byte*), // Горизонтальный скролинг
	v_scroll(bool, bool, byte*, byte*, uint16_t*, int8_t, int8_t); // Вертикальный скролинг области
 
   bool
	matrix_scroll_String(int8_t, int8_t, char*, byte*, const byte*, uint8_t, uint8_t, uint8_t), // Бегущая строка для матриц
	lcd_scroll_String(uint8_t, char*, byte*); // Бегущая строка для LCD
    
  private:
  uint8_t
 	icp,		/*pointer to character in the input string*/
   	fbp;		/*pointer to byte in font*/
  protected:
};

class HT // Для специализированного дисплея на HT1621
{
  public:
  void 
	dow_sel(uint8_t, byte*), 			//Выбор дня недели
	bat(uint8_t, byte*), 				//Батарейка  private:
    digit(uint8_t, uint8_t, byte*), 	// Большие цифры
    mon_day(uint8_t, uint8_t, byte*), 	//Маленькие цифры (Месяц, число, год)
	ala(uint8_t, byte*); 				//Будильник
  private:
  protected:
};

class MSG // Текстовые сообщения
{
  public:
  String 
	callback(uint8_t, uint8_t, uint8_t, bool), 
	alarm_msg(uint8_t, uint8_t, bool); // Номер активного будильника
};
#endif
