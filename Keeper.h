#ifndef KEEPER_H
#define KEEPER_H

#include "Bullet.h"

#define KEEPERSPEEDMAX ((30<<SUBPIXELBITS)/FPS)
#define KEEPERDIRECTIONCHANGEFRAMEMIN FPS/4
#define KEEPERDIRECTIONCHANGEFRAMEMAX FPS/2
#define BULLETFRAMEMIN FPS*3
#define BULLETFRAMEMAX FPS*7

class Keeper : public Entity
{
public:
  Keeper(int16_t tX, int16_t tY,Bullet *tMahBullet):
    Entity(tX,tY),
    mahBullet(tMahBullet),
    framecount(0),
    bulletframecount((rand() % (BULLETFRAMEMAX-BULLETFRAMEMIN))+BULLETFRAMEMIN)
  {
    
  }
  virtual ~Keeper()
  {
    
  }
  virtual EntityIdentification ImA()
  {
    return ID_Keeper;
  }
  virtual uint8_t *GetSprite()
  {
    return dir ? &keeperRight[2] : &keeper[2];
  }
  virtual uint8_t GetWidth()
  {
    return keeper[0];
  }
  virtual uint8_t GetHeight()
  {
    return keeper[1];
  }
  virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    if (framecount) framecount--;
    if (bulletframecount) bulletframecount--;
    switch(rand() % 6)
    {
    case 0://Move random
      if (!framecount)
      {
        unsigned int direct=rand() % 9;
        Point tDirection;
        memcpy_P(&tDirection, &directions[direct], sizeof(Point));
        velocity.x=tDirection.x*KEEPERSPEEDMAX;
        velocity.y=tDirection.y*KEEPERSPEEDMAX;
        framecount=rand() % KEEPERDIRECTIONCHANGEFRAMEMAX;
        if (framecount<KEEPERDIRECTIONCHANGEFRAMEMIN) framecount=KEEPERDIRECTIONCHANGEFRAMEMIN;
      }
      break;
    case 1://Move toward
      if (!framecount)
      {
        if (tNumEntity && tEntities[0])
        {
          int16_t x=tEntities[0]->pos.x-pos.x,
                  y=tEntities[0]->pos.y-pos.y;
  
          if (abs(x)<(1<<SUBPIXELBITS)) velocity.x=0;
          else if (x>0) velocity.x=KEEPERSPEEDMAX;
          else velocity.x=KEEPERSPEEDMAX*-1;
          
          if (abs(y)<(1<<SUBPIXELBITS)) velocity.y=0;
          else if (y>0) velocity.y=KEEPERSPEEDMAX;          
          else velocity.y=KEEPERSPEEDMAX*-1;
        }
        framecount=rand() % KEEPERDIRECTIONCHANGEFRAMEMAX;
        if (framecount<KEEPERDIRECTIONCHANGEFRAMEMIN) framecount=KEEPERDIRECTIONCHANGEFRAMEMIN;
      }
      break;
    case 2://Shoot
      if (!bulletframecount)
      {
        if (tNumEntity && tEntities[0] && mahBullet && mahBullet->range<=0)
        {
          int16_t x=tEntities[0]->pos.x-pos.x,
                  y=tEntities[0]->pos.y-pos.y;

          memset(&velocity,0,sizeof(velocity));
          mahBullet->pos.x=pos.x;
          mahBullet->pos.y=pos.y;

          if (abs(x)<(tEntities[0]->GetWidth()<<SUBPIXELBITS)) mahBullet->velocity.x=0;
          else if (x>0) mahBullet->velocity.x=BULLETTSPEED;          
          else mahBullet->velocity.x=BULLETTSPEED*-1;

          if (abs(y)<(tEntities[0]->GetHeight()<<SUBPIXELBITS)) mahBullet->velocity.y=0;
          else if (y>0) mahBullet->velocity.y=BULLETTSPEED;          
          else mahBullet->velocity.y=BULLETTSPEED*-1;
          if (mahBullet->velocity.x || mahBullet->velocity.y)
          {
            sound.tones(bulletFire);
            mahBullet->range=BULLETTRANGE;
            bulletframecount=rand() % BULLETFRAMEMAX;
            if (bulletframecount<BULLETFRAMEMIN) bulletframecount=BULLETFRAMEMIN;
          }
        }
      }
      break;
    default:
      break;
    }
  }
  Bullet *mahBullet;
  uint16_t framecount,bulletframecount;
};

#endif //KEEPER_H
