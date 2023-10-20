// Slave
#include <Arduino.h>
#include <daly-bms-uart.h>
#define BMS_SERIAL Serial

Daly_BMS_UART bms(BMS_SERIAL);

void setup (void)
{
  bms.Init();
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= bit(SPE);

  // turn on interrupts
  SPCR |= bit(SPIE);
}  // end of setup

//volatile char buf [21] = "Hello, world!";
volatile char buf[21];
volatile int pos;
volatile bool active;

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;

  if (c == 0x64)  // starting new sequence?
    {
    active = true;
    pos = 0;
    SPDR = buf [pos++];   // send first byte
    return;
    }

  if (!active)
    {
    SPDR = 0;
    return;
    }

  SPDR = buf [pos];
  if (buf [pos] == 0 || ++pos >= sizeof (buf))
    active = false;
}  // end of interrupt service routine (ISR) SPI_STC_vect


void loop (void)
{
  bms.update();
  
  for (size_t i = 0; i < size_t(bms.get.numberOfCells); i++)
  {
    buf[i] = ((int)(float)bms.get.cellVmV[i]*100)-250;
  }

  
}  // end of loop
