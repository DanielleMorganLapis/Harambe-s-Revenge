#ifndef GAMEMATH_H
#define GAMEMATH_H


#include <util/crc16.h>


#define FPS 30
#define SUBPIXELBITS 5

const Point directions[] PROGMEM =
{
  { 0,-1},
  { 1,-1},
  { 1, 0},
  { 1, 1},
  { 0, 1},
  {-1, 1},
  {-1, 0},
  {-1,-1},
  { 0, 0}//STOP
};


uint8_t crc8(const uint8_t *data,uint16_t len)
{
  uint8_t crc = 0;
  uint16_t idx;
  for (idx = 0; idx < len; idx++)
      crc = _crc_ibutton_update(crc, data[idx]);
  return crc; // must be 0
}


#endif //GAMEMATH_H

