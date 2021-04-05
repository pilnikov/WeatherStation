

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Adafruit_GFX.h"
#include "ESP32RGBmatrixPanel.h"

//G1	R1 |
//GND	B1 |
//G2	R2 |
//GND	B2  |
//B		A   |
//D		C  |
//LAT	CLK|
//GND	OE |


ESP32RGBmatrixPanel matrix(23, 14, 27, 26, 25, 04, 13, 02, 33, 15, 19, 18, 5); //Flexible connection

//Default connection
//uint8 OE = 23;
//uint8 CLK = 22;
//uint8 LAT = 03;
//uint8 CH_A = 21;
//uint8 CH_B = 19;
//uint8 CH_C = 18;
//uint8 CH_D = 5;
//uint8 R1 = 17;
//uint8 G1 = 16;
//uint8 BL1 = 4;
//uint8 R2 = 0;
//uint8 G2 = 2;
//uint8 BL2 = 15;


// ESP32RGBmatrixPanel matrix;

void getRGB(int hue, int sat, int val, int colors[3]) {
  int r;
  int g;
  int b;
  int base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0] = val;
    colors[1] = val;
    colors[2] = val;
  }
  else {

    base = ((255 - sat) * val) >> 8;

    switch (hue / 60) {
      case 0:
        r = val;
        g = (((val - base) * hue) / 60) + base;
        b = base;
        break;

      case 1:
        r = (((val - base) * (60 - (hue % 60))) / 60) + base;
        g = val;
        b = base;
        break;

      case 2:
        r = base;
        g = val;
        b = (((val - base) * (hue % 60)) / 60) + base;
        break;

      case 3:
        r = base;
        g = (((val - base) * (60 - (hue % 60))) / 60) + base;
        b = val;
        break;

      case 4:
        r = (((val - base) * (hue % 60)) / 60) + base;
        g = base;
        b = val;
        break;

      case 5:
        r = val;
        g = base;
        b = (((val - base) * (60 - (hue % 60))) / 60) + base;
        break;
    }

    colors[0] = r;
    colors[1] = g;
    colors[2] = b;
  }
}

auto str = "Hello from Vitali's ESP 32 @ Adafruit_GFX  4096 Colors/12 bit @ 50 fps";

void helloDemo()
{
  int pos = 0;
  matrix.setTextWrap(false);
  matrix.setTextSize(2);
  int colors[3];
//  while (pos > -900)
//  {
    matrix.black();
    matrix.setCursor(pos, 8);
    getRGB(abs(pos) % 255, 255, 255, colors);
    matrix.setTextColor(matrix.AdafruitColor(colors[0], colors[1], colors[2]));
    matrix.print(str);
 //   pos -= 1;
//    delay(1);
    //vTaskDelay(30);
//  }
}

void wheaterDemo()
{
  byte br = 127;
  auto white = ESP32RGBmatrixPanel::AdafruitColor(br, br, br);
  auto red = ESP32RGBmatrixPanel::AdafruitColor(br, 0, 0);
  auto cyan = ESP32RGBmatrixPanel::AdafruitColor(0, br, br);
  auto blue = ESP32RGBmatrixPanel::AdafruitColor(0, 0, br);

  matrix.black();
  matrix.setTextWrap(false);
  matrix.setTextSize(2);
  matrix.setCursor(0, 8);
  matrix.setTextColor(white);
  matrix.print("18:36");

  matrix.setTextSize(2);

  matrix.setCursor(0, 16);
  matrix.setTextColor(blue);
  matrix.print("-11.3");

  //vTaskDelay(6000);
}

//runs faster then default void loop(). why? runs on other core?
void loop2_task(void *pvParameter)
{
  while (true)
  {
    helloDemo();
    wheaterDemo();
  }
}

/* create a hardware timer 
hw_timer_t* displayUpdateTimer = NULL;

void IRAM_ATTR onDisplayUpdate()
{
  matrix.update();
}
*/

void setup() {
  Serial.begin(115200);
  matrix.setBrightness(6);
  matrix.cp437(true);
/*  xTaskCreate(&loop2_task, "loop2_task", 2048, NULL, 5, NULL);

   1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up 
  displayUpdateTimer = timerBegin(0, 80, true);

  Attach onTimer function to our timer 
  timerAttachInterrupt(displayUpdateTimer, &onDisplayUpdate, true);
  timerAlarmWrite(displayUpdateTimer, 2, true);
  timerAlarmEnable(displayUpdateTimer);
*/
  Serial.println("OK");
}



void loop()
{
    helloDemo();
    matrix.update();
}
