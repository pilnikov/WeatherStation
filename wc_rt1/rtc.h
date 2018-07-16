
#include <RtcDS3231.h>
#include <RtcDS1307.h>
#include <DS1302RTC.h>

void rtc_init();
void rtc_check();
void set_alarm();
bool Alarmed();
void ds3231_write();
void man_set_time();
void InteruptServiceRoutine();

// ----------------------------------- RTC
const int RtcSquareWaveInterrupt =  5; // Номер прерывания

// marked volatile so interrupt can safely modify them and
// other code can safely read and modify them
uint16_t interuptCount = 0;
bool interuptFlag = false;

// ----------------------------------- Конструктор DS3231
RtcDS3231<TwoWire> DS3231(Wire);       

// ----------------------------------- Конструктор DS1307
RtcDS1307<TwoWire> DS1307(Wire);

// ----------------------------------- Конструктор DS1302
DS1302RTC DS1302(kCePin, kDiPin, kClPin);


