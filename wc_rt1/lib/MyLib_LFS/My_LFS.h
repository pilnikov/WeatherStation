#ifndef My_LFS_h
#define My_LFS_h

// ------------------------------------------------------------- Include

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <LittleFS.h>

#ifndef DBG_OUT_PORT
#define DBG_OUT_PORT Serial
#endif

#ifndef debug_level
#define debug_level 0
#endif

class LFS
{
  public:
	void
	begin(void),
	writeFile(const char*, const char*);	
	
	String 
	readFile(const char*);
  private:
	String
	formatBytes(uint32_t);
  protected:
};

#endif /*My_LFS_h*/
