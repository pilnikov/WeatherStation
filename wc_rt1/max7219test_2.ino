// This sketch is provided as an instruction on various features of
// the Simulator for Arduino

// Press F7 to Step

int8_t pos  = -7;
int8_t pos1 =  7;
byte buf[1] = {0b11001100};
byte buf1[1] = {0b0};

void setup()
{
  Serial.begin(115200);
  Serial.println();
}


void loop()
{
  shift_ud(true, buf, buf1, 0, 1, 0, pos - 1);
  shift_ud(true, buf, buf1, 0, 1, 0, pos);
  pos++;
  if (pos > 7) pos -= 15;
  pos1++;
  if (pos1 > 7) pos1 -= 15;

  Serial.print("Result ");
  Serial.println(buf1[0], BIN);
  delay(500);
}

void bit_replace(byte in, byte &out, uint8_t num)
{
  uint8_t _num = 0;
  if (num < 8) _num = num;
  byte mask = 1 << _num;
  out &= ~mask;
  if (in & mask) out |= mask;
}


void shift_ud(bool dwn, byte* in, byte* out, int8_t x1, int8_t x2, int8_t y1, int8_t y2)
{
  for (int8_t x = x1; x < x2; x++)
  {
    if (dwn)
    {
      byte s = (y1 > y2) ? in[x] << (y1 - y2) : in[x] >> (y2 - y1);
      uint8_t a = abs(y1 - y2); 
      uint8_t b = 8;
      
      if (y1 < y2)
      {
        a = 0;
        b = 8 - abs(y1 - y2);
      }
      
      for (uint8_t n = a; n < b; n++)
      {
         bit_replace(s, out[x], n);
      }
    }
  }
}
