
#include <RtcDS3231.h>

void rtc_init();
void rtc_check();
void set_alarm();
bool Alarmed();
void ds3231_write();
void ISR_ATTR InteruptServiceRoutine();

// ----------------------------------- RTC
  #define RtcSquareWaveInterrupt 4 // Mega2560


// marked volatile so interrupt can safely modify them and
// other code can safely read and modify them

uint16_t interuptCount = 0;
static bool interuptFlag_int = false, interuptFlag_oth = false;

// ----------------------------------- Конструктор DS3231
RtcDS3231<TwoWire> DS3231(Wire);
