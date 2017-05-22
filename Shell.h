#ifndef SHELL_H
#define SHELL_H

#include "Bullet.h"

#define SHELLTSPEED ((45<<SUBPIXELBITS)/FPS)
#define SHELLTRANGE (128<<SUBPIXELBITS)



class Shell : public Bullet
{
public:
  Shell(int16_t tX, int16_t tY):
    Bullet(tX,tY)
  {
    
  }
  virtual ~Shell()
  {
    
  }
  virtual EntityIdentification ImA()
  {
    return ID_Shell;
  }
  virtual void Move()
  {
    if (range>0)
    {
      Entity::Move();
      range-=SHELLTSPEED;
    }
    else range=0;
  }
  /*virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    
  }*/
};

#endif //SHELL_H
