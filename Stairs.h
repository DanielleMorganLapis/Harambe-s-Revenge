#ifndef STAIRS_H
#define STAIRS_H

#include "Entity.h"

class Stairs : public Entity
{
public:
  Stairs(int16_t tX, int16_t tY):
    Entity(tX,tY),
    activated(false)
  {
    
  }
  virtual ~Stairs()
  {
    
  }
  virtual EntityIdentification ImA()
  {
    return ID_Stairs;
  }
  virtual void Move()
  {
  }
  virtual uint8_t *GetSprite()
  {
    return &stairs[2];
  }
  virtual uint8_t GetWidth()
  {
    return stairs[0];
  }
  virtual uint8_t GetHeight()
  {
    return stairs[1];
  }
  virtual void Draw(Arduboy2 &arduboy)
  {
    if (activated) Entity::Draw(arduboy);
  }
  /*virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    
  }*/
  bool activated;
};



#endif //STAIRS_H
