/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://gitlab.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
   1.0.1    26/09/2019  Modified for Moritz Numitron Clock
*/

#include "TLC591x.h"

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE)
{
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin = LE;
  OE_pin = OE;
  numchips = n;

  digitalWrite(OE_pin, HIGH);
  pinMode(OE_pin, OUTPUT);
  init();
}

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE)
{
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin = LE;
  OE_pin = NO_PIN;
  numchips = n;

  init();
}

void TLC591x::init()
{
  if (numchips < MINCHIPS)
    numchips = MINCHIPS;
  if (numchips > MAXCHIPS)
    numchips = MAXCHIPS;

  pinMode(SDI_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  digitalWrite(LE_pin, LOW);
  pinMode(LE_pin, OUTPUT);
}

uint16_t TLC591x::convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond)
{
  uint16_t dataBoth = 0x0000;

  dataBoth = dataFirst;
  dataBoth = dataBoth << 8;
  dataBoth |= dataSecond;
  return dataBoth;
}

uint8_t *TLC591x::convertFrom16To8(uint16_t dataAll)
{
  static uint8_t arrayData[2] = {0x00, 0x00};

  *(arrayData) = (dataAll >> 8) & 0x00FF;
  arrayData[1] = dataAll & 0x00FF;
  return arrayData;
}

void TLC591x::print(unsigned int number)
{
  printValue(convertFrom8To16(segements[(number / 10) % 10], segements[number % 10]));
}

void TLC591x::printValue(unsigned int n)
{
  for (byte i = 0; i < numchips; i++)
  {
    write(n);
    n = n / 256; // Get the next most significant byte
  }
  toggleLE();
}

void TLC591x::write(byte n)
{
  digitalWrite(SDI_pin, n & 0x01);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x02);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x04);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x08);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x10);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x20);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x40);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x80);
  toggleCLK();
}

void TLC591x::toggleLE()
{
  // TLC5916 minimum LE pulse time is 20 ns, so don't need to worry about
  // putting in a hard-coded delay.
  digitalWrite(LE_pin, HIGH);
  digitalWrite(LE_pin, LOW);
}

void TLC591x::toggleCLK()
{
  // TLC5916 minimum CLK pulse time is 20 ns, so don't need to worry aboutsplitT + 1
  // putting in a hard-coded delay.
  digitalWrite(CLK_pin, HIGH);
  digitalWrite(CLK_pin, LOW);
}