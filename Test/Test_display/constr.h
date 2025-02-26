#ifndef global_construct_h
#define global_construct_h

// ------------------------------------------------------------- Include
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Dsp.h>
#include <Scr.h>
#include <BH1750.h>

// ---------------------------------------------------- LM
BH1750 lightMeter;

// ---------------------------------------------------- Other
FD f_dsp2; //For Display

MyDspHW mydsp_hw;

MSG msg_dsp;

#endif
