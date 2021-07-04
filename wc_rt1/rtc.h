
#include <RtcDS3231.h>
#include <RtcDS1307.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

void rtc_init();
void rtc_check();
void set_alarm();
bool Alarmed();
void ds3231_write();
void man_set_time();

// ----------------------------------- RTC
#if defined(__xtensa__)
void IRAM_ATTR InteruptServiceRoutine();
#define RtcSquareWaveInterrupt 5 // Номер прерывания
#elif defined (__AVR__)
void ISR_ATTR InteruptServiceRoutine();
#define RtcSquareWaveInterrupt 4 // Mega2560
#endif


// marked volatile so interrupt can safely modify them and
// other code can safely read and modify them

uint16_t interuptCount = 0;
static bool interuptFlag_int = false, interuptFlag_oth = false;

// ----------------------------------- Конструктор DS3231
RtcDS3231<TwoWire> * ds3231;

// ----------------------------------- Конструктор DS1307
RtcDS1307<TwoWire> * ds1307;

// ----------------------------------- Конструктор DS1302
RtcDS1302<ThreeWire> * ds1302;
ThreeWire * myTWire;
