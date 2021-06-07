
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
void ISR_ATTR InteruptServiceRoutine();

// ----------------------------------- RTC
#if defined(__AVR__)
  #define RtcSquareWaveInterrupt 4 // Mega2560
#endif

#if defined(__xtensa__)
  #define RtcSquareWaveInterrupt 5 // Номер прерывания
#endif

// marked volatile so interrupt can safely modify them and
// other code can safely read and modify them

uint16_t interuptCount = 0;
static bool interuptFlag_int = false, interuptFlag_oth = false;

// ----------------------------------- Конструктор DS3231
RtcDS3231<TwoWire> DS3231(Wire);

// ----------------------------------- Конструктор DS1307
RtcDS1307<TwoWire> DS1307(Wire);

// ----------------------------------- Конструктор DS1302
ThreeWire myWire(DIO_PIN,CLK_PIN,CS_PIN); // IO, SCLK, CE
RtcDS1302<ThreeWire> DS1302(myWire);
