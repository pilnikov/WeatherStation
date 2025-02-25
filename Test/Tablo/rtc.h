
#include <RtcDS3231.h>
#include <RtcDS1307.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

void rtc_init();
void set_alarm();
bool Alarmed();
void man_set_time(const RtcDateTime&);
void GetTime();

// ----------------------------------- interrupt
#if defined(ESP8266)
void IRAM_ATTR InteruptServiceRoutine();
#define RtcSquareWaveInterrupt 5 // Номер прерывания
#elif defined(__AVR__)
void ISR_ATTR InteruptServiceRoutine();
#define RtcSquareWaveInterrupt 4 // Mega2560
#elif defined(ARDUINO_ARCH_ESP32)
void ARDUINO_ISR_ATTR isr();
#endif


// marked volatile so interrupt can safely modify them and
// other code can safely read and modify them

// ----------------------------------- Конструктор DS3231
RtcDS3231<TwoWire> * ds3231;

// ----------------------------------- Конструктор DS1307
RtcDS1307<TwoWire> * ds1307;

// ----------------------------------- Конструктор DS1302
RtcDS1302<ThreeWire> * ds1302;
ThreeWire * myTWire;

static bool wasAlarmed_int = false;
static uint32_t prev_ms;
