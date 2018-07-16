
/*************************************************
* Public Constants
*************************************************/

#define OCTAVE_0  0x00
#define OCTAVE_1  0x10
#define OCTAVE_2  0x20
#define OCTAVE_3  0x30
#define OCTAVE_4  0x40
#define OCTAVE_5  0x50
#define OCTAVE_6  0x60
#define OCTAVE_7  0x70
#define OCTAVE_8  0x80
/*
#define NOTE_C	  0x00
#define NOTE_CS	  0x01
#define NOTE_D    0x02
#define NOTE_DS   0x03
#define NOTE_E	  0x04
#define NOTE_F	  0x05
#define NOTE_FS	  0x06
#define NOTE_G    0x07
#define NOTE_GS	  0x08
#define NOTE_A	  0x09
#define NOTE_AS	  0x0A
#define NOTE_B	  0x0B

// Lowest for ck/1024
#define NOTE_B0  (NOTE_B  | OCTAVE_0)
#define NOTE_C1  (NOTE_C  | OCTAVE_1)
#define NOTE_CS1 (NOTE_CS | OCTAVE_1)
#define NOTE_D1  (NOTE_D  | OCTAVE_1)
#define NOTE_DS1 (NOTE_DS | OCTAVE_1)
#define NOTE_E1  (NOTE_E  | OCTAVE_1)
#define NOTE_F1  (NOTE_F  | OCTAVE_1)
#define NOTE_FS1 (NOTE_FS | OCTAVE_1)
#define NOTE_G1  (NOTE_G  | OCTAVE_1)
#define NOTE_GS1 (NOTE_GS | OCTAVE_1)
#define NOTE_A1  (NOTE_A  | OCTAVE_1)
#define NOTE_AS1 (NOTE_AS | OCTAVE_1)
#define NOTE_B1  (NOTE_B  | OCTAVE_1)
#define NOTE_C2  (NOTE_C  | OCTAVE_2)
#define NOTE_CS2 (NOTE_CS | OCTAVE_2)
#define NOTE_D2  (NOTE_D  | OCTAVE_2)
#define NOTE_DS2 (NOTE_DS | OCTAVE_2)
#define NOTE_E2  (NOTE_E  | OCTAVE_2)
#define NOTE_F2  (NOTE_F  | OCTAVE_2)
#define NOTE_FS2 (NOTE_FS | OCTAVE_2)
#define NOTE_G2  (NOTE_G  | OCTAVE_2)
#define NOTE_GS2 (NOTE_GS | OCTAVE_2)
#define NOTE_A2  (NOTE_A  | OCTAVE_2)
#define NOTE_AS2 (NOTE_AS | OCTAVE_2)
// Lowest for ck/256
#define NOTE_B2  (NOTE_B  | OCTAVE_2)
#define NOTE_C3  (NOTE_C  | OCTAVE_3)
#define NOTE_CS3 (NOTE_CS | OCTAVE_3)
#define NOTE_D3  (NOTE_D  | OCTAVE_3)
#define NOTE_DS3 (NOTE_DS | OCTAVE_3)
#define NOTE_E3  (NOTE_E  | OCTAVE_3)
#define NOTE_F3  (NOTE_F  | OCTAVE_3)
#define NOTE_FS3 (NOTE_FS | OCTAVE_3)
#define NOTE_G3  (NOTE_G  | OCTAVE_3)
#define NOTE_GS3 (NOTE_GS | OCTAVE_3)
#define NOTE_A3  (NOTE_A  | OCTAVE_3)
#define NOTE_AS3 (NOTE_AS | OCTAVE_3)
// Lowest for ck/128
#define NOTE_B3  (NOTE_B  | OCTAVE_3)
#define NOTE_C4  (NOTE_C  | OCTAVE_4)
#define NOTE_CS4 (NOTE_CS | OCTAVE_4)
#define NOTE_D4  (NOTE_D  | OCTAVE_4)
#define NOTE_DS4 (NOTE_DS | OCTAVE_4)
#define NOTE_E4  (NOTE_E  | OCTAVE_4)
#define NOTE_F4  (NOTE_F  | OCTAVE_4)
#define NOTE_FS4 (NOTE_FS | OCTAVE_4)
#define NOTE_G4  (NOTE_G  | OCTAVE_4)
#define NOTE_GS4 (NOTE_GS | OCTAVE_4)
#define NOTE_A4  (NOTE_A  | OCTAVE_4)
#define NOTE_AS4 (NOTE_AS | OCTAVE_4)
#define NOTE_B4  (NOTE_B  | OCTAVE_4)
#define NOTE_C5  (NOTE_C  | OCTAVE_5)
#define NOTE_CS5 (NOTE_CS | OCTAVE_5)
#define NOTE_D5  (NOTE_D  | OCTAVE_5)
#define NOTE_DS5 (NOTE_DS | OCTAVE_5)
#define NOTE_E5  (NOTE_E  | OCTAVE_5)
#define NOTE_F5  (NOTE_F  | OCTAVE_5)
#define NOTE_FS5 (NOTE_FS | OCTAVE_5)
#define NOTE_G5  (NOTE_G  | OCTAVE_5)
#define NOTE_GS5 (NOTE_GS | OCTAVE_5)
#define NOTE_A5  (NOTE_A  | OCTAVE_5)
#define NOTE_AS5 (NOTE_AS | OCTAVE_5)
#define NOTE_B5  (NOTE_B  | OCTAVE_5)
#define NOTE_C6  (NOTE_C  | OCTAVE_6)
#define NOTE_CS6 (NOTE_CS | OCTAVE_6)
#define NOTE_D6  (NOTE_D  | OCTAVE_6)
#define NOTE_DS6 (NOTE_DS | OCTAVE_6)
#define NOTE_E6  (NOTE_E  | OCTAVE_6)
#define NOTE_F6  (NOTE_F  | OCTAVE_6)
#define NOTE_FS6 (NOTE_FS | OCTAVE_6)
#define NOTE_G6  (NOTE_G  | OCTAVE_6)
#define NOTE_GS6 (NOTE_GS | OCTAVE_6)
#define NOTE_A6  (NOTE_A  | OCTAVE_6)
#define NOTE_AS6 (NOTE_AS | OCTAVE_6)
#define NOTE_B6  (NOTE_B  | OCTAVE_6)
#define NOTE_C7  (NOTE_C  | OCTAVE_7)
#define NOTE_CS7 (NOTE_CS | OCTAVE_7)
#define NOTE_D7  (NOTE_D  | OCTAVE_7)
#define NOTE_DS7 (NOTE_DS | OCTAVE_7)
#define NOTE_E7  (NOTE_E  | OCTAVE_7)
#define NOTE_F7  (NOTE_F  | OCTAVE_7)
#define NOTE_FS7 (NOTE_FS | OCTAVE_7)
#define NOTE_G7  (NOTE_G  | OCTAVE_7)
#define NOTE_GS7 (NOTE_GS | OCTAVE_7)
#define NOTE_A7  (NOTE_A  | OCTAVE_7)
#define NOTE_AS7 (NOTE_AS | OCTAVE_7)
#define NOTE_B7  (NOTE_B  | OCTAVE_7)
#define NOTE_C8  (NOTE_C  | OCTAVE_8)
#define NOTE_CS8 (NOTE_CS | OCTAVE_8)
#define NOTE_D8  (NOTE_D  | OCTAVE_8)
#define NOTE_DS8 (NOTE_DS | OCTAVE_8)
*/

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

#define NOTE_C    0x00
#define NOTE_C8  (NOTE_C  | OCTAVE_8)

#define FREQ_C11	33488
#define FREQ_CS11	35479
#define FREQ_D11	37589
#define FREQ_DS11	39824
#define FREQ_E11	42192
#define FREQ_F11	44701
#define FREQ_FS11	47359
#define FREQ_G11	50175
#define FREQ_GS11	53159
#define FREQ_A11	56320
#define FREQ_AS11	59669
#define FREQ_B11	63217
