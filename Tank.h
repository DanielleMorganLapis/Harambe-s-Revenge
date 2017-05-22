#ifndef TANK_H
#define TANK_H

#include "Shell.h"

#define TANKSPEEDMAX ((5<<SUBPIXELBITS)/FPS)
#define TANKDIRECTIONCHANGEFRAMEMIN FPS/4
#define TANKDIRECTIONCHANGEFRAMEMAX FPS/2
#define SHELLFRAMEMIN FPS*3
#define SHELLFRAMEMAX FPS*7

class Tank : public Entity
{
public:
  Tank(int16_t tX, int16_t tY, Shell *tMahShell):
    Entity(tX,tY),
    mahShell(tMahShell),
    framecount(0),
    shellframecount((rand() % (SHELLFRAMEMAX-SHELLFRAMEMIN))+SHELLFRAMEMIN)
  {
    
  }
  virtual ~Tank()
  {
    
  }
  virtual EntityIdentification ImA()
  {
    return ID_Tank;
  }
  virtual uint8_t *GetSprite()
  {
    switch(dir % 8)
    {
    case 0:
    case 1:
      return &tankUp[2];
      break;
    case 2:
    case 3:
      return &tankRight[2];
      break;
    case 4:
    case 5:
      return &tankDown[2];      
      break;
    case 6:
    case 7:
      return &tankLeft[2];
      break;
    default:
      return NULL;
    }
  }
  virtual uint8_t GetWidth()
  {
    switch(dir % 8)
    {
    case 0:
    case 1:
      return tankUp[0];
      break;
    case 2:
    case 3:
      return tankRight[0];
      break;
    case 4:
    case 5:
      return tankDown[0];      
      break;
    case 6:
    case 7:
      return tankLeft[0];
      break;
    default:
      return 0;
    }
  }
  virtual uint8_t GetHeight()
  {
    switch(dir % 8)
    {
    case 0:
    case 1:
      return tankUp[1];
      break;
    case 2:
    case 3:
      return tankRight[1];
      break;
    case 4:
    case 5:
      return tankDown[1];
      break;
    case 6:
    case 7:
      return tankLeft[1];
      break;
    default:
      return 0;
    }
  }
  virtual void Move()
  {
    pos.x+=velocity.x;
    pos.y+=velocity.y;
  }
  virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    if (framecount) framecount--;
    if (shellframecount) shellframecount--;
    switch(rand() % 6)
    {
    case 0://Move random
      if (!framecount)
      {
        unsigned int direct=rand() % 9;
        switch(direct)
        {
        case 0:
        case 1:
          dir=0;
          direct=0;
          break;
        case 2:
        case 3:
          dir=2;
          direct=2;
          break;
        case 4:
        case 5:
          dir=4;
          direct=4;
          break;
        case 6:
        case 7:
          dir=6;
          direct=6;
          break;
        case 8:
          break;        
        }
        Point tDirection;
        memcpy_P(&tDirection, &directions[direct], sizeof(Point));
        velocity.x=tDirection.x*TANKSPEEDMAX;
        velocity.y=tDirection.y*TANKSPEEDMAX;
        framecount=rand() % TANKDIRECTIONCHANGEFRAMEMAX;
        if (framecount<TANKDIRECTIONCHANGEFRAMEMIN) framecount=TANKDIRECTIONCHANGEFRAMEMIN;
      }
      break;
    case 1://Move toward
      if (!framecount)
      {
        if (tNumEntity && tEntities[0])
        {
          int16_t x=tEntities[0]->pos.x-pos.x,
                  y=tEntities[0]->pos.y-pos.y;
  
          switch (rand() % 2)
          {
          case 0:
            if (abs(x)<(1<<SUBPIXELBITS)) velocity.x=0;
            else if (x>0) 
            {
              dir=2;
              velocity.x=TANKSPEEDMAX;
            }
            else
            {
              dir=6;
              velocity.x=TANKSPEEDMAX*-1;
            }
            velocity.y=0;
            break;
          case 1:  
            if (abs(y)<(1<<SUBPIXELBITS))
            {
              velocity.y=0;
            }
            else if (y>0)
            {
              dir=4;
              velocity.y=TANKSPEEDMAX;          
            }
            else
            {
              dir=0;
              velocity.y=TANKSPEEDMAX*-1;
            }
            velocity.x=0;
            break;
          }
        }
        framecount=rand() % TANKDIRECTIONCHANGEFRAMEMAX;
        if (framecount<TANKDIRECTIONCHANGEFRAMEMIN) framecount=TANKDIRECTIONCHANGEFRAMEMIN;
      }
      break;
    case 2://Shoot
      if (!shellframecount)
      {
        if (tNumEntity && tEntities[0] && mahShell && mahShell->range<=0)
        {
          int16_t x=tEntities[0]->pos.x-pos.x,
                  y=tEntities[0]->pos.y-pos.y;

          memset(&velocity,0,sizeof(velocity));
          
          
          switch(dir)
          {
          case 0:
          case 1:
            mahShell->pos.x=pos.x+((tEntities[0]->GetWidth()<<SUBPIXELBITS)/2);
            mahShell->pos.y=pos.y;
            if (abs(y)<(tEntities[0]->GetHeight()<<SUBPIXELBITS)) mahShell->velocity.y=0;
            else mahShell->velocity.y=SHELLTSPEED*-1;
            mahShell->velocity.x=0;
            break;
          case 2:
          case 3:
            mahShell->pos.x=pos.x+(tEntities[0]->GetWidth()<<SUBPIXELBITS);
            mahShell->pos.y=pos.y+((tEntities[0]->GetHeight()<<SUBPIXELBITS)/2);
            if (abs(x)<(tEntities[0]->GetWidth()<<SUBPIXELBITS)) mahShell->velocity.x=0;
            else if (x>0) mahShell->velocity.x=SHELLTSPEED;
            mahShell->velocity.y=0;
            break;
          case 4:
          case 5:
            mahShell->pos.x=pos.x+((tEntities[0]->GetWidth()<<SUBPIXELBITS)/2);
            mahShell->pos.y=pos.y+(tEntities[0]->GetHeight()<<SUBPIXELBITS);
            if (abs(y)<(tEntities[0]->GetHeight()<<SUBPIXELBITS)) mahShell->velocity.y=0;
            else if (y>0) mahShell->velocity.y=SHELLTSPEED;
            mahShell->velocity.x=0;
            break;
          case 6:
          case 7:
            mahShell->pos.x=pos.x;
            mahShell->pos.y=pos.y+((tEntities[0]->GetHeight()<<SUBPIXELBITS)/2);
            if (abs(x)<(tEntities[0]->GetWidth()<<SUBPIXELBITS)) mahShell->velocity.x=0;
            else mahShell->velocity.x=SHELLTSPEED*-1;
            mahShell->velocity.y=0;
            break;
          default:
            break;
          }
          
          if (mahShell->velocity.x || mahShell->velocity.y)
          {
            sound.tones(tankFire);
            mahShell->range=SHELLTRANGE;
            shellframecount=rand() % SHELLFRAMEMAX;
            if (shellframecount<SHELLFRAMEMIN) shellframecount=SHELLFRAMEMIN;
          }
        }
      }
      break;
    default:
      break;
    }
  }
  Shell *mahShell;
  uint16_t framecount,shellframecount;
};

#endif //TANK_H
