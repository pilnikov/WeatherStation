
void ht1621_init()
{
  ht21 = new HT1621(CS_PIN, CLK_PIN, DIO_PIN); // ss, rw, data
  ht21->begin();

  ht21->sendCommand(HT1621::RC256K);
  ht21->sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht21->sendCommand(HT1621::SYS_EN);
  ht21->sendCommand(HT1621::LCD_ON);

  uint8_t i = 0;

  ht21->clear_all();// clear memory
  for (i = 0; i < 32; i++)
  {
    buf[i] = 0x0;
  }

  digit(1, 1); /* 1 */
  digit(3, 2); /* 2 */
  digit(5, 6); /* 6 */
  digit(7, 1); /* 1 */

  for (i = 1; i < 8; i++)
  {
    ht21->write(i, buf[i]);

    bat(i - 1);
    ht21->write(21, buf, 2);
    delay(250);
  }

}

void tm1637_init()
{
  tm1637 = new TM1637(CLK_PIN, DIO_PIN);
  tm1637->clear();
  tm1637->set_br(7);
}
