#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

/*
   Read byte from EEPROM
*/
byte er_byte(int addr) {
#if DebugReadWrite == true
  Serial.print (F("R-> "));
  Serial.print (addr);
  Serial.print (F(" byte: "));
  Serial.println (EEPROM.read(addr));
#endif

  return EEPROM.read(addr);
}



/*
   Update byte in EEPROM
*/
void ew_byte(int addr, byte data) {
#if DebugReadWrite == true
  byte old = EEPROM.read(addr);
  Serial.print (F("U-> "));
  Serial.print (addr);
  Serial.print (F(" byte: "));
  Serial.print (old);
  Serial.print (F(" => "));
  Serial.println (data);
  if (data != old)
    EEPROM.write(addr, data);
#else
  if (data != EEPROM.read(addr))
    EEPROM.write(addr, data);
#endif
}



/*
   Read uint16_t from EEPROM
*/
uint16_t er_uint(int addr) {
#if DebugReadWrite == true
  Serial.print (F("R-> "));
  Serial.print (addr);
  Serial.print (F(" uint: "));
  Serial.println (word(EEPROM.read(addr), EEPROM.read(addr + 1)));
#endif

  return word(EEPROM.read(addr), EEPROM.read(addr + 1));
}



/*
   Update uint16_t in EEPROM
*/
void ew_uint(int addr, uint16_t data) {
#if DebugReadWrite == true
  uint16_t old = word(EEPROM.read(addr), EEPROM.read(addr + 1));
  Serial.print (F("U-> "));
  Serial.print (addr);
  Serial.print (F(" uint: "));
  Serial.print (old);
  Serial.print (F(" => "));
  Serial.println (data);
  if (data != old) {
    EEPROM.write(addr, highByte(data));
    EEPROM.write(addr + 1, lowByte(data));
  }
#else
  if (data != word(EEPROM.read(addr), EEPROM.read(addr + 1))) {
    EEPROM.write(addr, highByte(data));
    EEPROM.write(addr + 1, lowByte(data));
  }
#endif
}


#endif
