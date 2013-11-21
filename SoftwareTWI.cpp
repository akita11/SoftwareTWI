#include <pins_arduino.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <SoftwareTWI.h>

#define TWI_PORT PORTB
#define TWI_DDR  DDRB
#define TWI_PIN  PINB

SoftwareTWI::SoftwareTWI(void)
{
}

uint8_t pinSCLa, pinSDAa;
uint8_t bmSCL, bmSDA;

// SCL=PB6 SDA=PB7
void SoftwareTWI::wait(void)
{
  delayMicroseconds(10);
}

void SoftwareTWI::sendT(void)
{
  setSDAdir(DIR_OUT);
  setSCL(1); setSDA(1);
  wait();
  setSDA(0);
  wait();
  setSCL(0);
}

void SoftwareTWI::sendP(void)
{
  setSDAdir(DIR_OUT)();
  setSDA(0); setSCL(0);
  wait();
  setSCL(1);
  wait();
  setSDA(1);
  setSDAdir(DIR_IN);
}

void SoftwareTWI::setSCL(uint8_t value)
{
  if (value == 0) TWI_PORT &= ~bmSCL;
  else TWI_PORT |= bmSCL;
}

void SoftwareTWI::setSDA(uint8_t value)
{
  if (value == 0) TWI_PORT &= ~bmSDA;
  else TWI_PORT |= bmSDA;
}

uint8_t SoftwareTWI::readSDA(void)
{
  if (TWI_PIN & bmSDA) return(1); else return(0);
}

void SoftwareTWI::setSCLdir(uint8_t dir)
{
  if (dir == DIR_OUT) TWI_DDR |= bmSCL;
  else TWI_DDR &= ~bmSCL;
}

void SoftwareTWI::setSDAdir(uint8_t dir)
{
  if (dir == DIR_OUT) TWI_DDR |= bmSDA;
  else TWI_DDR &= ~bmSDA;
}

void SoftwareTWI::begin(uint8_t pinSCL, uint8_t pinSDA) // in PORTB
{
  setSCLdir(DIR_OUT);
  setSDAdir(DIR_IN);
  pinSCLa = pinSCL;
  pinSDAa = pinSDA;
  bmSCL = 1 << pinSCL;
  bmSDA = 1 << pinSDA;
}

void SoftwareTWI::beginTransmission(uint8_t addr)
{
  sendT();
  write((addr << 1) & 0xfe);
}

void SoftwareTWI::endTransmission(void)
{
  sendP();
}

void SoftwareTWI::requestFrom(uint8_t addr, uint8_t count) // arg:count ignored
{
  sendT();
  write((addr << 1) & 0xfe);
  setSDAdir(DIR_IN);
}

void SoftwareTWI::write(uint8_t value)
{
  int i;
  setSDAdir(DIR_OUT);
  for (i = 0; i < 8; i++){
    if (d & 0x80) setSDA(1); else setSDA(0);
    d = d << 1;
    wait();
    setSCL(1);
    wait();
    setSCL(0);
  }
  setSDAdir(DIR_IN);
  wait();
  setSCL(1);
  wait();
  if (readSDA() == 0) i = 0; else i = 1;
  setSCL(0);
  wait();
  setSDAdir(DIR_IN);
  return(i);
}

void SoftwareTWI::read(void)
{
  int i;
  byte d = 0;
  setSDAdir(DIR_IN);
  for (i = 0; i < 8; i++){
    d = d << 1;
    if (readSDA() == 1) d |= 0x01;
    wait();
    setSCL(1);
    wait();
    setSCL(0);
  }
  setSDAdir(DIR_OUT)();
  setSDA(0); // ACK
  wait();
  setSCL(1);
  wait();
  setSCL(0);
  wait();
  setSDAdir(DIR_IN);
  return(d);
}
