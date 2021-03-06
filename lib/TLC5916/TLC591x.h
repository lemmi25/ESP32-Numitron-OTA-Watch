/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://gitlab.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
*/
#ifndef TLC591x_LIBRARY
#define TLC591x_LIBRARY

#include <Arduino.h>

class TLC591x
{
public:
  enum
  {
    NO_PIN = 255
  };

  TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE);
  TLC591x(byte n, byte SDI, byte CLK, byte LE);

  void print(unsigned int number);
  void printValue(unsigned int n);

private:
  enum POWER_MODE
  {
    WAKEUP = 1,
    SHUTDOWN = 0
  };
  enum
  {
    NO_DATA_COMING = 0,
    DATA_COMING = 1
  };
  enum
  {
    MINCHIPS = 1,
    MAXCHIPS = 2
  };
  byte SDI_pin, CLK_pin, LE_pin, OE_pin, numchips;

  uint16_t convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond);
  uint8_t *convertFrom16To8(uint16_t dataAll);

  void write(byte n);
  void toggleLE();
  void toggleCLK();
  void init();
};

// Conversion of letters to LED segments.
// 0x00 is a blank character and is used for unsupported values.
// Save memory by not defining first 32 ascii characters, since they
// are all control characters
// number 11 in array is - for weather
const uint8_t segements[12] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 230, 2, 0};
#endif
/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://gitlab.com/Andy4495/TLC591x
*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
*/
/* #ifndef TLC591x_LIBRARY
#define TLC591x_LIBRARY

#include <Arduino.h>

class TLC591x
{
public:
  enum
  {
    NO_PIN = 255
  };

  TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE);
  TLC591x(byte n, byte SDI, byte CLK, byte LE);

  void print(const char *s);
  void print(unsigned int n);
  void displayEnable();
  void displayDisable();

private:
  enum POWER_MODE
  {
    WAKEUP = 1,
    SHUTDOWN = 0
  };
  enum
  {
    NO_DATA_COMING = 0,
    DATA_COMING = 1
  };
  enum
  {
    MINCHIPS = 1,
    MAXCHIPS = 2
  };
  byte SDI_pin, CLK_pin, LE_pin, OE_pin, numchips;

  void write(byte n);
  void toggleLE();
  void toggleCLK();
  void init();
};

// Conversion of letters to LED segments.
// 0x00 is a blank character and is used for unsupported values.
// Save memory by not defining first 32 ascii characters, since they
// are all control characters
const unsigned char ICM7218_segment_map[96] = {
    //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 00: Control characters
    //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 08: Control characters
    //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 10: Control characters
    //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 18: Control characters
    0x00, 0x67, 0x22, 0x41, 0x18, 0x12, 0x45, 0x20, // 20: spc ! " # $ % & '
    0x49, 0x51, 0x63, 0x28, 0x10, 0x04, 0x00, 0x2c, // 28:   ( ) * + , - . /
    0x7b, 0x30, 0x6d, 0x75, 0x36, 0x57, 0x5f, 0x70, // 30:   0 1 2 3 4 5 6 7
    0x7f, 0x77, 0x44, 0x5d, 0x0d, 0x05, 0x15, 0x6c, // 38:   8 9 : ; < = > ?
    0x00, 0x7e, 0x1f, 0x4b, 0x3d, 0x4f, 0x4e, 0x5b, // 40:   @ A B C D E F G
    0x3e, 0x0a, 0x39, 0x0f, 0x0b, 0x5c, 0x1c, 0x7b, // 48:   H I J K L M N O
    0x6e, 0x76, 0x0c, 0x57, 0x4a, 0x3b, 0x3b, 0x59, // 50:   P Q R S T U V W
    0x3a, 0x37, 0x7d, 0x4b, 0x16, 0x71, 0x62, 0x01, // 58:   X Y Z [ \ ] ^ _
    0x02, 0x7e, 0x1f, 0x0d, 0x3d, 0x4f, 0x4e, 0x5b, // 60:   ` a b c d e f g
    0x1e, 0x08, 0x39, 0x0f, 0x0b, 0x5c, 0x1c, 0x1d, // 68:   h i j k l m n o
    0x6e, 0x76, 0x0c, 0x57, 0x4a, 0x19, 0x19, 0x59, // 70:   p q r s t u v w
    0x3a, 0x37, 0x7d, 0x4d, 0x08, 0x55, 0x66, 0x00  // 78:   x y z { | } ~ DEL
};
#endif */