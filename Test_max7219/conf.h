// ------------------------------------------------------------- Include
#include "Udt.h"
#include <Fdsp.h>

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

// ------------------------------------------------------ GPIO


// ----------------------------------- Typedef
conf_data_t conf_data;
// ----------------------------------- Internal header files
#include "disp.h"

// ----------------------------------- Force define func name

// ---------------------------------------------------- LM

char tstr[25];

bool                disp_on  = true, nm_is_on = false;

uint8_t           disp_mode  = 0;
uint16_t             cur_br  = 0;


// ---------------------------------------------------- Constructors
FD f_dsp;
