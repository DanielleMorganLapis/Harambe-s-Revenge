#ifndef HARAMBE_H
#define HARAMBE_H

#include "Entity.h"
#include "Music.h"
#ifdef USE_ARDVOICE
#include <ArdVoice.h>
#endif

#define HARAMBEDASHRANGE (60<<SUBPIXELBITS)
#define HARAMBEDASHFRAMEMAX FPS*5
#define HARAMBERUNSPEED ((40<<SUBPIXELBITS)/FPS)
#define HARAMBEDASHSPEED ((60<<SUBPIXELBITS)/FPS)
#define HARAMBEDASHLEDINTENSITY 1


class Harambe : public Entity
{
public:
  Harambe(int16_t tX,
          int16_t tY
          #ifdef USE_ARDVOICE
          ,ArdVoice *tArdVoice
          #endif
          ):
    Entity(tX,tY),
    #ifdef USE_ARDVOICE
    ardvoice(tArdVoice),
    #endif
    range(0),
    framecount(0)
  {
  }
  virtual ~Harambe()
  {
  }
  virtual EntityIdentification ImA()
  {
    return ID_Harambe;
  }
  virtual uint8_t *GetSprite()
  {
    return dir ? &harambeRight[2] : &harambe[2];
  }
  virtual uint8_t GetWidth()
  {
    return harambe[0];
  }
  virtual uint8_t GetHeight()
  {
    return harambe[1];
  }
  virtual void Draw(Arduboy2 &arduboy)
  {
    Entity::Draw(arduboy);
    memset(&velocity,0,sizeof(velocity));
  }
  virtual void Input(Arduboy2 &arduboy,ArduboyTones &sound,uint16_t tNumEntity, Entity **tEntities)
  {
    //arduboy.pollButtons();    
    if (arduboy.pressed(LEFT_BUTTON))
    {
      if (range>0) velocity.x-=HARAMBEDASHSPEED; else velocity.x-=HARAMBERUNSPEED;
    }
    if (arduboy.pressed(RIGHT_BUTTON))
    {
      if (range>0) velocity.x+=HARAMBEDASHSPEED; else velocity.x+=HARAMBERUNSPEED;
    }
    if (arduboy.pressed(UP_BUTTON))
    {
      if (range>0) velocity.y-=HARAMBEDASHSPEED; else velocity.y-=HARAMBERUNSPEED;
    }
    if (arduboy.pressed(DOWN_BUTTON))
    {
      if (range>0) velocity.y+=HARAMBEDASHSPEED; else velocity.y+=HARAMBERUNSPEED;
    }
    if (!framecount)
    {
      if (arduboy.pressed(A_BUTTON)
          ||
          arduboy.pressed(B_BUTTON)
          &&
          !range
          )
      {
        range=HARAMBEDASHRANGE;
        arduboy.setRGBled(HARAMBEDASHLEDINTENSITY,HARAMBEDASHLEDINTENSITY,0);
        #ifdef USE_ARDVOICE
        ardvoice->playVoice(harambeDash);
        #else
        sound.tones(dashSfx);
        #endif
      }
    }
    else
    {
      framecount--;
      if (!framecount) arduboy.setRGBled(0,HARAMBEDASHLEDINTENSITY,0);
    }
        
    if (range && (velocity.x || velocity.y))
    {
      range-=HARAMBEDASHSPEED;
      if (range<0) range=0;
      if (!range)
      {
        framecount=HARAMBEDASHFRAMEMAX;
        arduboy.setRGBled(HARAMBEDASHLEDINTENSITY,0,0);
      }
    }
  }
  #ifdef USE_ARDVOICE
  ArdVoice *ardvoice;
  #endif
  int16_t range;
  uint16_t framecount;
};

#endif //HARAMBE_H
