#include <Udt.h>
#include "conf.h"

#if defined(__AVR_ATmega2560__)
void(* resetFunc) (void) = 0; //Programm reset
#endif
