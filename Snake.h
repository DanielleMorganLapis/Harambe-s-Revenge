#ifndef SNAKE_H
#define SNAKE_H

#include "Entity.h"

#define SNAKESPEEDMAX ((10<<SUBPIXELBITS)/FPS)
#define SNAKEDIRECTIONCHANGEFRAMEMIN FPS/2
#define SNAKEDIRECTIONCHANGEFRAMEMAX FPS

class Snake : public Entity
{
public:
  Snake(int16_t tX, int16_t tY):
    Entity(tX,tY),
    framecount(0),
    dead(false)
  {
    
  }
  virtual ~Snake()
  {
    
  }
  virtual EntityIdentification ImA()
  {
    return ID_Snake;
  }
  virtual void Draw(Arduboy2 &arduboy)
  {
    if (!dead) 
    {
      Entity::Draw(arduboy);
    }
  }
  virtual uint8_t *GetSprite()
  {
    return dir ? &snakeRight[2] : &snake[2];
  }
  virtual uint8_t GetWidth()
  {
    return snake[0];
  }
  virtual uint8_t GetHeight()
  {
    return snake[1];
  }
  virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    if (!dead)
    {
      if (!framecount)
      {
        if (tNumEntity && tEntities[0])
        {
          int16_t x=tEntities[0]->pos.x-pos.x,
                  y=tEntities[0]->pos.y-pos.y;
  
          if (abs(x)<(1<<SUBPIXELBITS)) velocity.x=0;
          else if (x>0) velocity.x=SNAKESPEEDMAX;
          else velocity.x=SNAKESPEEDMAX*-1;
          
          if (abs(y)<(1<<SUBPIXELBITS)) velocity.y=0;
          else if (y>0) velocity.y=SNAKESPEEDMAX;          
          else velocity.y=SNAKESPEEDMAX*-1;
        }
        framecount=(rand() % (SNAKEDIRECTIONCHANGEFRAMEMAX-SNAKEDIRECTIONCHANGEFRAMEMIN))+SNAKEDIRECTIONCHANGEFRAMEMIN;
      }
      else framecount--;
    }
  }
  uint16_t framecount;
  bool dead;
};

#endif //SNAKE_H
