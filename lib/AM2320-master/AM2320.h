#ifndef AM2320_H
#define AM2320_H

#include <Arduino.h>
#include <Wire.h>


#define AM2320_address (0xB8 >> 1) 

class AM2320
{
	public:
		AM2320();
                boolean begin(void);
		float t;
		float h;
		int Read(void); 
        private:
		unsigned int CRC16(byte *, byte);
};

#endif
