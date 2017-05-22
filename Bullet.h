#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

#define BULLETTSPEED ((45<<SUBPIXELBITS)/FPS)
#define BULLETTRANGE (50<<SUBPIXELBITS)



class Bullet : public Entity
{
public:
  Bullet(int16_t tX, int16_t tY):
    Entity(tX,tY),
    range(0)
  {
    
  }
  virtual ~Bullet()
  {
    
  }
  virtual EntityIdentification ImA()
  {
    return ID_Bullet;
  }
  virtual void Move()
  {
    if (range>0)
    {
      Entity::Move();
      range-=BULLETTSPEED;
    }
    else range=0;
  }
  virtual void Draw(Arduboy2 &arduboy)
  {
    if (range>0) arduboy.drawPixel(pos.x>>SUBPIXELBITS,pos.y>>SUBPIXELBITS,WHITE);
  }
  /*virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    
  }*/
  int16_t range;
};

#endif //BULLET_H
