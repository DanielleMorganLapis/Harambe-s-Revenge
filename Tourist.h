#include "Entity.h"

#define TOURISTSPEEDMAX ((30<<SUBPIXELBITS)/FPS)
#define TOURISTDIRECTIONCHANGEFRAMEMIN FPS/4
#define TOURISTDIRECTIONCHANGEFRAMEMAX FPS/2
#define TOURISTSCAREDDIST (13<<SUBPIXELBITS)
#define ZOMBIESPEED ((10<<SUBPIXELBITS)/FPS)
#define ZOMBIEDIRECTIONCHANGEFRAMEMIN FPS/2
#define ZOMBIEDIRECTIONCHANGEFRAMEMAX FPS

class Tourist : public Entity
  {
  public:
    Tourist(int16_t tX, int16_t tY):
      Entity(tX,tY),
      framecount((rand() % (TOURISTDIRECTIONCHANGEFRAMEMAX-TOURISTDIRECTIONCHANGEFRAMEMIN))+TOURISTDIRECTIONCHANGEFRAMEMIN),
      scared(0),
      id(ID_Tourist)
    {
      
    }
    virtual ~Tourist()
    {
      
    }
    virtual EntityIdentification ImA()
    {
      return id;
    }
    virtual uint8_t *GetSprite()
    {
      if (id==ID_Tourist)
      {
        if (scared>FPS) return &tourist[2];
        else if (scared) return &touristSmirk[2];
        else return &touristCalm[2];
      }
      else return &touristZombie[2];
    }
    virtual uint8_t GetWidth()
    {
      return tourist[0];
    }
    virtual uint8_t GetHeight()
    {
      return tourist[1];
    }
    virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
    {
      if (ImA()==ID_Tourist)
      {
        int16_t x,y;
        if (tEntities && tEntities[0])
        {
          x=pos.x-tEntities[0]->pos.x,y=pos.y-tEntities[0]->pos.y;
          if (scared) scared--;
          if ((abs(x)<=TOURISTSCAREDDIST) && (abs(y)<=TOURISTSCAREDDIST)) scared=FPS*2;
        }
        if (!framecount)
        {
          /*if (scared)  // Game to hard when scared tourists run away
          {
            if (abs(x)<(tEntities[0]->GetWidth()<<SUBPIXELBITS)) velocity.x=0;
            else if (x>0) velocity.x=TOURISTSPEEDMAX;
            else velocity.x=TOURISTSPEEDMAX*-1;
            
            if (abs(y)<(tEntities[0]->GetHeight()<<SUBPIXELBITS)) velocity.y=0;
            else if (y>0) velocity.y=TOURISTSPEEDMAX;          
            else velocity.y=TOURISTSPEEDMAX*-1;
            framecount=(rand() % TOURISTDIRECTIONCHANGEFRAMEMAX-TOURISTDIRECTIONCHANGEFRAMEMIN)+TOURISTDIRECTIONCHANGEFRAMEMIN;
          }
          else*/
          {
            unsigned int direct=rand() % 9;
            Point tDirection;
            memcpy_P(&tDirection, &directions[direct], sizeof(Point));
            velocity.x=tDirection.x*TOURISTSPEEDMAX;
            velocity.y=tDirection.y*TOURISTSPEEDMAX;
            framecount=(rand() % TOURISTDIRECTIONCHANGEFRAMEMAX-TOURISTDIRECTIONCHANGEFRAMEMIN)+TOURISTDIRECTIONCHANGEFRAMEMIN;
          }
        }
        else framecount--;
      }
      else 
      {
        if (tNumEntity && tEntities && tEntities[0])
        {
          switch (rand() % 2)
          {
          default:
            {
              int16_t x,y,idx,bdx;
              int32_t dist,bDist=0x7FFFFFFF;
              
              for (idx=0;idx<tNumEntity;idx++)
              {
                x=tEntities[idx]->pos.x-pos.x;
                y=tEntities[idx]->pos.y-pos.y;
                dist=x*x+y*y;
                if (dist<bDist)
                {
                  bdx=idx;
                  bDist=dist;
                }
              }
              x=tEntities[bdx]->pos.x-pos.x;
              y=tEntities[bdx]->pos.y-pos.y;
              if (abs(x)<(tEntities[bdx]->GetWidth()<<SUBPIXELBITS)) velocity.x=0;
              else if (x>0) velocity.x=ZOMBIESPEED;
              else velocity.x=ZOMBIESPEED*-1;
              
              if (abs(y)<(tEntities[bdx]->GetHeight()<<SUBPIXELBITS)) velocity.y=0;
              else if (y>0) velocity.y=ZOMBIESPEED;          
              else velocity.y=ZOMBIESPEED*-1;
              framecount=(rand() % ZOMBIEDIRECTIONCHANGEFRAMEMAX-ZOMBIEDIRECTIONCHANGEFRAMEMIN)+ZOMBIEDIRECTIONCHANGEFRAMEMIN;
            }
            break;
          /*default:
            {
              unsigned int direct=rand() % 9;
              Point tDirection;
              memcpy_P(&tDirection, &directions[direct], sizeof(Point));
              velocity.x=tDirection.x*ZOMBIESPEED;
              velocity.y=tDirection.y*ZOMBIESPEED;
              framecount=(rand() % ZOMBIEDIRECTIONCHANGEFRAMEMAX-ZOMBIEDIRECTIONCHANGEFRAMEMIN)+ZOMBIEDIRECTIONCHANGEFRAMEMIN;
              break;
            }*/
          }
        }
      }
    }
    uint16_t framecount,scared;
    uint8_t id;
  };
