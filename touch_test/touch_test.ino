#include "GyverButton.h"
// аналоговая клавиатура подключена на А7. Схему смотри на странице библиотеки
GButton myButt1;
GButton myButt2;
GButton myButt3;
GButton myButt4;

uint8_t value1 = 0, value2 = 0, value3 = 0, val_buff1 = 0, val_buff2 = 0, val_buff3 = 0;

void setup() {
  Serial.begin(115200);
  // меняем тип на LOW_PULL, потому что по умолчанию стоит HIGH_PULL
  myButt1.setType(LOW_PULL);
  myButt2.setType(LOW_PULL);
  myButt3.setType(LOW_PULL);
  myButt4.setType(LOW_PULL);

  myButt1.setStepTimeout(50);
  myButt2.setStepTimeout(50);
  myButt3.setStepTimeout(50);
  myButt4.setStepTimeout(50);

  pinMode(22, OUTPUT);
}

void loop() {

  myButt1.tick(touchRead(33) < 30 && touchRead(33) > 10);
  myButt2.tick(touchRead(32) < 30 && touchRead(32) > 10);
  myButt3.tick(touchRead(13) < 30 && touchRead(13) > 10);
  myButt4.tick(touchRead(27) < 30 && touchRead(27) > 10);
  /*
    if (myButt1.isHolded()) {
      Serial.println("hold 1");
    }
    if (myButt2.isHolded()) {
      Serial.println("hold 2");
    }
    if (myButt3.isHolded()) {
      Serial.println("hold 3");
    }

    if (myButt4.isHolded()) {
      Serial.println("hold 4");
    }
    delay(10);
  */

  if (myButt1.isClick()) {           // одиночное нажатие
    if (value1 < 255) value1++;                       // инкремент
  }

  if (myButt2.isClick()) {           // одиночное нажатие
    if (value1 > 0) value1--;                       // декремент
  }

  if (myButt1.isStep()) {            // обработчик удержания с шагами
    if (value1 < 255) value1++;                       // увеличивать/уменьшать переменную value с шагом и интервалом!
  }

  if (myButt2.isStep()) {            // обработчик удержания с шагами
    if (value1 > 0) value1--;                       // увеличивать/уменьшать переменную value с шагом и интервалом!
  }

  if (value1 != val_buff1)
  {
    // analogWrite(22, value2);         // для примера выведем в порт
    Serial.println(value1);         // для примера выведем в порт
    val_buff1 = value1;
    value2 = value1;
    value3 = value1;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (myButt3.isClick()) {           // одиночное нажатие
    if (value2 < value1) value2++;                       // инкремент
  }

  if (myButt4.isClick()) {           // одиночное нажатие
    if (value2 > 0) value2--;                       // декремент
  }

  if (myButt3.isStep()) {            // обработчик удержания с шагами
    if (value2 < value1) value2++;                       // увеличивать/уменьшать переменную value с шагом и интервалом!
  }

  if (myButt4.isStep()) {            // обработчик удержания с шагами
    if (value2 > 0) value2--;                       // увеличивать/уменьшать переменную value с шагом и интервалом!
  }

  if (value2 != val_buff2)
  {
    analogWrite(22, value2);         // для примера выведем в порт
    Serial.println(value2);         // для примера выведем в порт
    val_buff2 = value2;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (myButt3.isClick()) {           // одиночное нажатие
    if (value2 < value1) value2++;                       // инкремент
  }

  if (myButt4.isClick()) {           // одиночное нажатие
    if (value2 > 0) value2--;                       // декремент
  }

  if (myButt3.isStep()) {            // обработчик удержания с шагами
    if (value2 < value1) value2++;                       // увеличивать/уменьшать переменную value с шагом и интервалом!
  }

  if (myButt4.isStep()) {            // обработчик удержания с шагами
    if (value2 > 0) value2--;                       // увеличивать/уменьшать переменную value с шагом и интервалом!
  }

  if (value2 != val_buff2)
  {
//    analogWrite(22, value2);         // для примера выведем в порт
    Serial.println(value3);         // для примера выведем в порт
    val_buff3 = value3;
  }

}
