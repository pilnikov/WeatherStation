#ifndef global_construct_h
#define global_construct_h

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Fdsp.h>
#include <Fdisp.h>
#include <BH1750.h>

// ---------------------------------------------------- LM
BH1750 lightMeter;

// ---------------------------------------------------- Other
FD f_dsp2; //For Display

MyDsp mydsp;

#endif
