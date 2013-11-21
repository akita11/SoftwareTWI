#ifndef SoftwareTWI_H
#define SoftwareTWI_H

#include <inttypes.h>

class SoftwareTWI
{
 private: 
  coist uint8_t DIR_OUT = 0;
  coist uint8_t DIR_IN = 1;
  void wait(void);
  void sendT(void);
  void sendP(void);
  void setSCL(uint8_t value);
  void setSDA(uint8_t value);
  uint8_t readSDA(void);
  void setSCLdir(uint8_t dir);
  void setSDAdir(uint8_t dir);
 public:
  SoftwareTWI();
  void begin(uint8_t pinSCL, uint8_t pinSDA); // in PORTB
  void beginTransmission(uint8_t addr);
  void endTransmission(void);
  void requestFrom(uint8_t addr, uint8_t count);
  void write(uint8_t value);
  void read(void);
};

#endif // SoftwareTWI.h
