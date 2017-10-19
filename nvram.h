#ifndef NVRAM_H
#define NVRAM_H

#include <EEPROM.h>

#define EEPROM_MAP_SIZE  (32)
#define EEPROM_

void nvram_init()
{
  EEPROM.begin(EEPROM_MAP_SIZE);
}
void nvram_dump(){
  Serial.println("NVRAM");
  unsigned int value;
  unsigned int rowsize=0;
  for (unsigned int addr = 0; addr < EEPROM_MAP_SIZE; addr)
  {
    Serial.print(addr, HEX);
    rowsize=8;
    for (rowsize; rowsize && addr < EEPROM_MAP_SIZE; rowsize--)
    { 
      Serial.print("\t");
      value = EEPROM.read(addr);
      Serial.print(value, HEX);
      addr++;
    }
    Serial.println();
  }
}

#endif
