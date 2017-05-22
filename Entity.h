#ifndef ENTITY_H
#define ENTITY_H

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "GameMath.h"

typedef enum
{
  ID_Entity=0,
  ID_Harambe,
  ID_Keeper,
  ID_Tank,
  ID_Tourist,
  ID_Zombie,
  ID_Snake,
  ID_Bullet,
  ID_Shell,
  ID_Stairs
} EntityIdentification;


class Entity 
{
public:
  Entity(int16_t tX, int16_t tY):
    pos(Point{tX, tY}),
    velocity(Point{0,0}),
    dir(0)
  {
  }
  virtual ~Entity()
  {
    
  }
  // I know RTTI is bad, but I'm not sure of a better way right now.
  virtual EntityIdentification ImA()
  {
    return ID_Entity;
  }
  virtual void Draw(Arduboy2 &arduboy)
  {
    if (GetSprite()) 
    {
      arduboy.drawBitmap( pos.x>>SUBPIXELBITS,
                          pos.y>>SUBPIXELBITS,
                          GetSprite(),
                          GetWidth(),
                          GetHeight(),
                          WHITE
                          );
    }
  }
  virtual uint8_t *GetSprite()
  {
    return NULL;
  }
  virtual uint8_t GetWidth()
  {
    return 0;
  }
  virtual uint8_t GetHeight()
  {
    return 0;
  }
  /*virtual int16_t GetWidth16()
  {
    return GetWidth();
  }
  virtual int16_t GetHeight16()
  {
    return GetHeight();
  }*/
  virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
  }
  virtual void Move()
  {
    if (velocity.x) { if (velocity.x>0) { dir=1; } else { dir=0;} }
    pos.x+=velocity.x;
    pos.y+=velocity.y;
  }

  virtual bool Collide(const Entity &tTarget)
  {
    int16_t tX,tY,tWidth,tHeight,tX2,tY2,tWidth2,tHeight2;

    if (pos.x<tTarget.pos.x)
    {
      tX=pos.x;
      tWidth=GetWidth();
      tWidth<<=SUBPIXELBITS;
      tX2=tTarget.pos.x;
      tWidth2=tTarget.GetWidth();
      tWidth2<<=SUBPIXELBITS;
    }
    else
    {
      tX=tTarget.pos.x;
      tWidth=tTarget.GetWidth()<<SUBPIXELBITS;
      tX2=pos.x;
      tWidth2=GetWidth();
      tWidth2<<=SUBPIXELBITS;
    }

    if (pos.y<tTarget.pos.y)
    {
      tY=pos.y;
      tHeight=GetHeight();
      tHeight<<=SUBPIXELBITS;
      tY2=tTarget.pos.y;
      tHeight2=tTarget.GetHeight();
      tHeight2<<=SUBPIXELBITS;
    }
    else
    {
      tY=tTarget.pos.y;
      tHeight=tTarget.GetHeight();
      tHeight<<=SUBPIXELBITS;
      tY2=pos.y;
      tHeight2=GetHeight();
      tHeight2<<=SUBPIXELBITS;
    }
    
    if (((tX2-tX)<tWidth)
        &&
        ((tY2-tY)<tHeight)
        )
    {
      return true;
    }
    else return false;
  }
  Point pos,velocity;
  uint8_t dir;
};


#endif  //ENTITY_H
