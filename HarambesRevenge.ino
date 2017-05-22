// Designed and written by Molly Carpenter and Danielle Morgan Lapis for the 1st Arduboy Game Jam ending May 22.
#include "assets.h"
#include "Harambe.h"
#include "Tourist.h"
#include "Keeper.h"
#include "Snake.h"
#include "Tank.h"
#include "Stairs.h"
#include <ArduboyTones.h>
#include "Music.h"
#ifdef USE_ARDVOICE
#include <ArdVoice.h>
#endif
#include "Strings.h"
#include "HighScore.h"

Arduboy2 arduboy;
#ifdef USE_ARDVOICE
ArdVoice ardvoice;
#endif
ArduboyTones sound(arduboy.audio.enabled);




void playTone(unsigned int frequency, unsigned long duration)
{
    if (arduboy.audio.enabled() == true)
    {
      sound.tone(frequency, duration);
    } 
}

#define ZOMBIFY_SCOREVALUE 3
#define TIMEBONUSSECONDS 15
#define ONEUPDIVISOR 100
#define INTROSCROLLSPEED ((10<<SUBPIXELBITS)/FPS)

typedef enum
{
  MODE_Intro = 0,
  MODE_Help,
  MODE_Start,
  MODE_Stage,
  MODE_Game,
  MODE_TimeBonus,
  MODE_GameOver
} GameModes;


typedef enum
{
  HELP_Harambe=0,
  HELP_Tourist,
  HELP_Keeper,
  HELP_Stairs,
  HELP_Snake,
  HELP_Tank
} HelpSections;

typedef enum
{
  MENU_sound = 0,
  MENU_start,
  MENU_help
} StartMenu;


uint32_t frameCount,seed=0;
uint16_t highScore=0,score;
int16_t scrollY=HEIGHT<<SUBPIXELBITS;
uint8_t stage,lives,lastMode=0xFF,mode=MODE_Intro;
bool waitRelease=false,playTheme;
uint8_t menuIdx=MENU_start,helpSection=HELP_Harambe;

class Game
{
public:
  Game():
    numEntity(0),
    numTourist(stage>=10 ? 4 : 3),
    numZooKeeper(stage>=15 ? 2 : 1),
    numSnake(stage>=5 ? 1 : 0),
    numTank(stage>=20 ? 2 : (stage/10 ? stage/10 : 0)),
    numZombie(0),
    over(0),
    stageWin(0),
    entities({NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}),
    harambeEnt(NULL),
    tourists(NULL),
    bullets(NULL),
    zookeepers(NULL),
    snakes(NULL),
    shells(NULL),
    tanks(NULL),
    stageExit(NULL)      
  {
    uint16_t idx;
    int16_t tX,tY;
    //if (!entities) arduboy.setRGBled(0,0,1);
    memset(&entities,0,sizeof(entities));
    for (idx=0;idx<numTourist;idx++)
    {
      tX=(rand() % (64-tourist[0]));
      if (tX>32) tX=(tX-32)+96;
      tY=((rand() % 32-tourist[1]));
      if (tY>16) tY=(tY-16)+48;
      entities[numEntity]=new Tourist(tX<<SUBPIXELBITS,tY<<SUBPIXELBITS);
      if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
      //Serial.println((void*)entities[numEntity]);
      if (!tourists) tourists=&entities[numEntity];
      numEntity++;
    }
    for (idx=0;idx<numSnake;idx++)
    {
      tX=(rand() % (64-snake[0]));
      if (tX>32) tX=(tX-32)+96;
      tY=(rand() % (32-snake[1]));
      if (tY>16) tY=(tY-16)+48;
      entities[numEntity]=new Snake(tX<<SUBPIXELBITS,
                                     tY<<SUBPIXELBITS
                                     );
      if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
      if (!snakes)
      {
        snakes=&entities[numEntity];
        if (!tourists) tourists=&entities[numEntity];
      }
      numEntity++;
    }
    for (idx=0;idx<numZooKeeper;idx++)
    {
      entities[numEntity]=new Bullet(0,0);
      if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
      if (!bullets) bullets=&entities[numEntity];
      numEntity++;
    }
    for (idx=0;idx<numTank;idx++)
    {
      entities[numEntity]=new Shell(0,0);
      if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
      if (!shells) shells=&entities[numEntity];
      numEntity++;
    }
    //Tanks and Keepers are declared back to back to they can be fed into the zombie Tourist input method using zookeepers
    for (idx=0;idx<numZooKeeper;idx++)
    {
      tX=(rand() % (64-keeper[0]));
      if (tX>32) tX=(tX-32)+96;
      tY=(rand() % (32-keeper[1]));
      if (tY>16) tY=(tY-16)+48;
      entities[numEntity]=new Keeper(tX<<SUBPIXELBITS,
                                     tY<<SUBPIXELBITS,
                                     bullets[idx]
                                     );
      if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
      if (!zookeepers) zookeepers=&entities[numEntity];
      numEntity++;
    }
    for (idx=0;idx<numTank;idx++)
    {
      tX=(rand() % (64-tankUp[0]));
      if (tX>32) tX=(tX-32)+96;
      tY=(rand() % (32-tankUp[1]));
      if (tY>16) tY=(tY-16)+48;
      entities[numEntity]=new Tank(tX<<SUBPIXELBITS,
                                   tY<<SUBPIXELBITS,
                                   shells[idx]
                                   );
      if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
      if (!tanks)
      {
        tanks=&entities[numEntity];
        if (!zookeepers) zookeepers=&entities[numEntity];
      }
      numEntity++;
    }
    entities[numEntity]=new Harambe((64-4)<<SUBPIXELBITS,
                                    (32-4)<<SUBPIXELBITS
                                    #ifdef USE_ARDVOICE
                                    ,&ardvoice
                                    #endif
                                    );
    if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
    harambeEnt=&entities[numEntity];
    numEntity++;
    int stairsX, stairsY;
    switch (rand() % 4)
    {
      case 0:
        stairsX = 0;
        stairsY = 0;
        break;
      case 1:
        stairsX = (128-stairs[0])<<SUBPIXELBITS;
        stairsY = 0;
        break;
      case 2:
        stairsX = 0;
        stairsY = (64-stairs[1])<<SUBPIXELBITS;
        break;
      case 3:
        stairsX = (128-stairs[0])<<SUBPIXELBITS;
        stairsY = (64-stairs[1])<<SUBPIXELBITS;
        break;
    } 
    entities[numEntity]=new Stairs(stairsX,stairsY);
    if (!entities[numEntity]) arduboy.setRGBled(0,0,1);
    stageExit=&entities[numEntity];
    numEntity++;
  }    
  ~Game()
  {
    if (entities)
    {
      uint16_t idx;
      for (idx=0;idx<numEntity;idx++) if (entities[idx])
      {
        delete entities[idx];
        entities[idx]=NULL;
      }
      //free(entities);
      //entities=NULL;
    }
  }
  void DrawEntities()
  {
    uint16_t idx;
    for (idx=0;idx<numEntity;idx++) 
    {
      if (entities[idx])
      {
        entities[idx]->Draw(arduboy);
      }
    }
  }
  void MoveEntities()
  {
    uint16_t idx;
    for (idx=0;idx<numEntity;idx++) 
    {
      if (entities[idx]) entities[idx]->Move();
    }
  } 
  void CheckInput()
  {
    uint16_t idx;
    for (idx=0;idx<numEntity;idx++) 
    {
      if (entities[idx])
      {
        switch (entities[idx]->ImA())
        {
        case ID_Keeper:
        case ID_Tourist:
        case ID_Tank:
        case ID_Snake:
          entities[idx]->Input(arduboy,sound,1,harambeEnt);
          break;
        case ID_Zombie:
          entities[idx]->Input(arduboy,sound,numZooKeeper/*+numTank*/,zookeepers);
          break;
        default:
          entities[idx]->Input(arduboy,sound,numEntity,entities);
        }
      }
    }
  }

  void CheckCollision()
  {
    uint16_t idx,jdx;
    for (idx=0;idx<numEntity;idx++)
    {
      if (entities[idx]->pos.x<0)
      {
        entities[idx]->pos.x=0;
        entities[idx]->velocity.x*=-1;
        if (entities[idx]->ImA()==ID_Bullet) ((Bullet*)entities[idx])->range=0;
        if (entities[idx]->ImA()==ID_Shell) ((Shell*)entities[idx])->range=0;
      }
      if ((entities[idx]->pos.x+(entities[idx]->GetWidth()<<SUBPIXELBITS))>(127<<SUBPIXELBITS))
      {
        entities[idx]->pos.x=(127<<SUBPIXELBITS)-(entities[idx]->GetWidth()<<SUBPIXELBITS);
        entities[idx]->velocity.x*=-1;
        if (entities[idx]->ImA()==ID_Bullet) ((Bullet*)entities[idx])->range=0;
        if (entities[idx]->ImA()==ID_Shell) ((Shell*)entities[idx])->range=0;
      }
      if (entities[idx]->pos.y<0)
      {
        entities[idx]->pos.y=0;
        entities[idx]->velocity.y*=-1;
        if (entities[idx]->ImA()==ID_Bullet) ((Bullet*)entities[idx])->range=0;
        if (entities[idx]->ImA()==ID_Shell) ((Shell*)entities[idx])->range=0;
      }
      if ((entities[idx]->pos.y+(entities[idx]->GetHeight()<<SUBPIXELBITS))>(63<<SUBPIXELBITS))
      {
        entities[idx]->pos.y=(63<<SUBPIXELBITS)-(entities[idx]->GetHeight()<<SUBPIXELBITS);
        entities[idx]->velocity.y*=-1;
        if (entities[idx]->ImA()==ID_Bullet) ((Bullet*)entities[idx])->range=0;
        if (entities[idx]->ImA()==ID_Shell) ((Shell*)entities[idx])->range=0;
      }        
    }
    for (idx=0;idx<numEntity;idx++)
    {
      if ((entities[idx]!=harambeEnt[0])
          &&
          (
          ((entities[idx]->ImA()==ID_Bullet) && (((Bullet*)entities[idx])->range>0))
          ||
          ((entities[idx]->ImA()==ID_Shell) && (((Shell*)entities[idx])->range>0))
          ||
          (entities[idx]->ImA()!=ID_Bullet)
          )
          &&
          harambeEnt[0]->Collide(*entities[idx])
          )
      {
        if (entities[idx]->ImA()==ID_Tourist)
        {
          sound.tones(transform);
          ((Tourist*)entities[idx])->id=ID_Zombie;
          numZombie++;
          if ((score/ONEUPDIVISOR)<((score+ZOMBIFY_SCOREVALUE)/ONEUPDIVISOR))
          {
            sound.tones(oneUp);
            lives++;
          }
          score+=ZOMBIFY_SCOREVALUE;
          if (numZombie==numTourist) stageExit[0]->activated=true;
        }
        else if (((entities[idx]->ImA()==ID_Snake)
                  &&
                  (!((Snake*)entities[idx])->dead)
                  )
                  ||
                  (entities[idx]->ImA()==ID_Keeper)
                  ||
                  (entities[idx]->ImA()==ID_Bullet)
                  ||
                  (entities[idx]->ImA()==ID_Tank)
                  ||
                  (entities[idx]->ImA()==ID_Shell)
                  ||
                  stage==255// Kill screen
                  )
        {
          // draw last frame
          arduboy.clear();
          DrawEntities();
          arduboy.display();
          sound.tones(harambeShot);
          over=true;
        }
        else if ((entities[idx]->ImA()==ID_Stairs) && (numZombie==numTourist))
        {
          sound.tones(exitStage);
          stageWin=true;
        }
      }
    }

    for (jdx=0;jdx<numZooKeeper+numTank;jdx++)
    {
      if (bullets[jdx] && bullets[jdx]->range>0)
      {
        for (idx=0;idx<numTourist+numSnake;idx++)
        {
          if (((tourists[idx]
                &&
                ((tourists[idx]->ImA()==ID_Zombie)
                  ||
                  ((tourists[idx]->ImA()==ID_Snake)
                  &&
                  (!((Snake*)tourists[idx])->dead)
                  )
                  )
                )
               )              
              &&
              bullets[jdx]->Collide(*tourists[idx])
              )
          {
            bullets[jdx]->range=0;
            if (tourists[idx]->ImA()==ID_Snake)
            {              
              sound.tones(snakeShot);
              ((Snake*)tourists[idx])->dead=true;
            }
          }
        }
      }
    }
  }

  Entity *entities[1+1+4+6+4];
  Harambe **harambeEnt;
  Tourist **tourists; 
  Bullet **bullets;
  Keeper **zookeepers;
  Snake **snakes;
  Shell **shells;
  Tank **tanks;
  Stairs **stageExit;    
  uint8_t numEntity,numTourist,numZooKeeper,numSnake,numTank,numZombie;
  bool over,stageWin; 
};


Game *harambesRevenge=NULL;


class Intro
{
public:
  ~Intro();
  Intro(); 

  void Draw(uint16_t page)
  {
    
  }  
};

void setup()
{
  arduboy.boot();
  arduboy.audio.begin();
  arduboy.setFrameRate(FPS);
  arduboy.setTextSize(1);
  //Start serial
  /*Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/

  //Print length of data to run CRC on.
  /*Serial.print("EEPROM length: ");
  Serial.println(EEPROM.length());*/
  HighScoreEntry tScore;
  int16_t address=ReadHighScore(tScore);
  if (address!=-1)
  {
    /*char tInitials[4];
    Serial.print("tScore.index=");
    Serial.println(tScore.index);
    Serial.print("tScore.stage=");
    Serial.println(tScore.stage);
    Serial.print("tScore.initials=");
    sprintf(tInitials,"%c%c%c",tScore.initials[0],tScore.initials[1],tScore.initials[2]);
    Serial.println(tInitials);
    Serial.print("tScore.score=");
    Serial.println(tScore.score);
    Serial.print("tScore.crc=0x");
    Serial.println(tScore.crc,HEX);*/
    highScore=tScore.score;
  }
  sound.tones(story);
}




void loop() {
  if (!(arduboy.nextFrame()))
    return;

  if ((mode==MODE_Game) || (mode==MODE_Intro) || (mode!=lastMode))
  {
    lastMode=mode;
    arduboy.clear();
    switch (mode)
    {
    case MODE_Intro:
      {
        //arduboy.drawBitmap(0,0,&production[2],128,64,WHITE);
        char tBuffer[22];
        scrollY-=INTROSCROLLSPEED;
        int16_t tY=(scrollY>>SUBPIXELBITS);
        uint8_t startLine=0,endLine,idx;
        arduboy.setCursor(0,tY);
        if (tY<0)
        {
          startLine=(0-tY)/8;
          if (startLine>=STORYLINES)
          {
            mode=MODE_Start;
            playTheme=true;
            break;
          }
          for (idx=0;idx<startLine;idx++) arduboy.println("");          
        }
        endLine=startLine+((HEIGHT+8)/8);
        if (endLine>STORYLINES) endLine=STORYLINES;
        
        for (idx=startLine;idx<endLine;idx++)
        {
          arduboy.setCursor((WIDTH/2)-((strlen_P((char*)pgm_read_word(&(storyLines[idx])))*6)/2),arduboy.getCursorY());
          arduboy.println(strcpy_P(tBuffer, (char*)pgm_read_word(&(storyLines[idx]))));
        }
      }
      break;
    case MODE_Help:
      {
        char tBuffer[22];
        uint8_t endLine=scrollY+((HEIGHT-8)/8),idx;
        if (endLine>pgm_read_byte(&HelpSectionLines[helpSection])) endLine=pgm_read_byte(&HelpSectionLines[helpSection]);
        arduboy.setCursor(9,0);
        arduboy.drawBitmap(0,0,pgm_read_word(&(helpGFX[helpSection])),8,8);
        arduboy.println(strcpy_P(tBuffer,helpHelpText));
        for (idx=scrollY;idx<endLine;idx++)
        {
          arduboy.setCursor(1,arduboy.getCursorY());
          switch (helpSection)
          {
          case HELP_Harambe:
            arduboy.println(strcpy_P(tBuffer,(char*)pgm_read_word(&(harambeHelpLines[idx]))));
            break;
          case HELP_Tourist:
            arduboy.println(strcpy_P(tBuffer,(char*)pgm_read_word(&(touristHelpLines[idx]))));          
            break;
          case HELP_Keeper:
            arduboy.println(strcpy_P(tBuffer,(char*)pgm_read_word(&(keeperHelpLines[idx]))));          
            break;
          case HELP_Stairs:
            arduboy.println(strcpy_P(tBuffer,(char*)pgm_read_word(&(stairsHelpLines[idx]))));          
            break;
          case HELP_Snake:
            arduboy.println(strcpy_P(tBuffer,(char*)pgm_read_word(&(snakeHelpLines[idx]))));          
            break;
          case HELP_Tank:
            arduboy.println(strcpy_P(tBuffer,(char*)pgm_read_word(&(tankHelpLines[idx]))));
            break;
          }
        }
      }
      break;
    case MODE_Start:
      {
        char tBuffer[22];
        if (playTheme)
        {
          playTheme=false;
          sound.tones(mainTheme);
        }
        arduboy.drawBitmap(0,0,&start[2],128,64,WHITE);
        arduboy.setCursor(1,32);
        arduboy.setTextColor(menuIdx==MENU_sound ? 1 : 0);
        arduboy.setTextBackground(menuIdx==MENU_sound ? 0 : 1);
        arduboy.print(strcpy_P(tBuffer, menuSound));
        if (arduboy.audio.enabled()) arduboy.print(strcpy_P(tBuffer, menuOn));
        else arduboy.print(strcpy_P(tBuffer, menuOff));
        arduboy.setCursor(arduboy.getCursorX()+1,arduboy.getCursorY());
        arduboy.setTextColor(menuIdx==MENU_start ? 1 : 0);
        arduboy.setTextBackground(menuIdx==MENU_start ? 0 : 1);
        arduboy.print(strcpy_P(tBuffer, menuStart));
        arduboy.setCursor(arduboy.getCursorX()+1,arduboy.getCursorY());
        arduboy.setTextColor(menuIdx==MENU_help ? 1 : 0);
        arduboy.setTextBackground(menuIdx==MENU_help ? 0 : 1);
        arduboy.print(strcpy_P(tBuffer, menuHelp));
      }
      break;
    case MODE_Stage:
      {
        char tStr[22];
        char tBuffer[22];
        sprintf((char*)&tStr,"%hu",score > highScore ? score : highScore);
        arduboy.setCursor(64-(((strlen_P(highScoreText)+strlen(tStr))*6)/2),0);
        arduboy.print(strcpy_P(tBuffer, highScoreText));
        arduboy.print(tStr);
        sprintf((char*)&tStr,"%hhu",stage);
        arduboy.setCursor(64-(((strlen_P(stageText)+strlen(tStr))*6)/2),32-12);
        arduboy.print(strcpy_P(tBuffer, stageText));
        arduboy.print(tStr);
        arduboy.print("\n");
        sprintf((char*)&tStr,"%hhu",lives);
        arduboy.setCursor(64-(((strlen_P(livesText)+strlen(tStr))*6)/2),arduboy.getCursorY());
        arduboy.print(strcpy_P(tBuffer, livesText));
        arduboy.print(tStr);
        arduboy.print("\n");
        sprintf((char*)&tStr,"%u",score);
        arduboy.setCursor(64-(((strlen_P(scoreText)+strlen(tStr))*6)/2),arduboy.getCursorY());
        arduboy.print(strcpy_P(tBuffer, scoreText));
        arduboy.print(tStr);
        arduboy.setRGBled(0,HARAMBEDASHLEDINTENSITY,0);//Harmabe Dash is ready
      }
      break;
    case MODE_Game:
      if (harambesRevenge)
      {
        harambesRevenge->DrawEntities();
      }
      break;
    case MODE_TimeBonus:
      {
        char tStr[22];
        char tBuffer[22];
        int32_t secs=frameCount/FPS;
        secs=TIMEBONUSSECONDS-secs;   
        if ((score/ONEUPDIVISOR)<((score+secs)/ONEUPDIVISOR))
        {
          while (sound.playing());
          sound.tones(oneUp);
          lives++;         
        }
        score+=secs;
        sprintf((char*)&tStr,"%i",secs);
        arduboy.setCursor(64-(((strlen_P(timeBonusText)+strlen(tStr))*6)/2),32-4);
        arduboy.print(strcpy_P(tBuffer, timeBonusText));
        arduboy.print(tStr);
      }
      break;
    case MODE_GameOver:
      {
        char tStr[22];
        char tBuffer[22];
        while (sound.playing());
        sprintf((char*)&tStr,"%hhu",score > highScore ? score : highScore);
        arduboy.setCursor(64-(((strlen_P(highScoreText)+strlen(tStr))*6)/2),0);
        arduboy.print(strcpy_P(tBuffer, highScoreText));
        arduboy.print(tStr);
        sound.tones(gameOver);
        arduboy.setCursor(64-((strlen_P(gameOverText)*6)/2),32-8);
        arduboy.print(strcpy_P(tBuffer,gameOverText));
        arduboy.print("\n");
        sprintf((char*)&tStr,"%u",score);
        arduboy.setCursor(64-(((strlen_P(scoreText)+strlen(tStr))*6)/2),arduboy.getCursorY());
        arduboy.print(strcpy_P(tBuffer, scoreText));
        arduboy.print(tStr);
        if (score>highScore)
        {
          highScore=score;
          HighScoreEntry tScore;
          uint16_t lastGood=ReadHighScore(tScore);
          tScore.initials[0]='A';
          tScore.initials[1]='S';
          tScore.initials[2]='T';
          tScore.stage=stage;
          tScore.score=score;          
          WriteHighScore(lastGood,tScore);
        }
      }
      break;
    }
    //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
    arduboy.display();
    frameCount++;
  }

  if (mode==MODE_Game)
  {
    if (!harambesRevenge)
    {
      frameCount=0;
      harambesRevenge=new Game();
      if (!harambesRevenge) arduboy.setRGBled(0,0,1);
    }
    if (harambesRevenge)
    {
      harambesRevenge->CheckInput();      
      harambesRevenge->MoveEntities();          
      harambesRevenge->CheckCollision();
      if (harambesRevenge->stageWin || harambesRevenge->over)
      {
        if (harambesRevenge->over)
        {
          lives--;
          if (lives) mode=MODE_Stage; else mode=MODE_GameOver;
        }
        if (harambesRevenge->stageWin)
        {
          int32_t secs=frameCount/FPS;
          if (secs<TIMEBONUSSECONDS)
          {
            mode=MODE_TimeBonus;
          }
          else mode=MODE_Stage;     
          // increase difficulty
          stage++;
        }
        delete harambesRevenge;
        harambesRevenge=NULL;
      }
    }
  }
  else
  {
    arduboy.pollButtons();
    if (waitRelease)
    {
      if (arduboy.notPressed(LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON | DOWN_BUTTON | A_BUTTON | B_BUTTON))
      {
        waitRelease=false;
        if ((menuIdx==MENU_start) && (arduboy.justReleased(A_BUTTON) || arduboy.justReleased(B_BUTTON)))
        {
          if (mode==MODE_Intro)
          {
            playTheme=true;
            mode=MODE_Start;
          } 
          else if (mode==MODE_Start)
          {
              score=0;
              stage=1;
              lives=3;
              mode++;
              arduboy.setTextColor(1);
              arduboy.setTextBackground(0);
          }
          else if (mode==MODE_TimeBonus)
          {
            mode=MODE_Stage;
          }
          else mode++;
          if (mode==MODE_Game)
          {
            // Apply seed just before starting game
            srand(seed);        
          }
          else if (mode>MODE_GameOver)
          {
            mode=MODE_Start;
          }
        }
      }
    }
    else if (arduboy.justPressed(A_BUTTON))
    {
      if (mode==MODE_Help)
      {
        mode=MODE_Start;
      }
      else if (mode==MODE_Start)
      {
        switch (menuIdx)
        {
        case MENU_sound:
          if (arduboy.audio.enabled()) arduboy.audio.off();
          else arduboy.audio.on();
          arduboy.audio.saveOnOff();
          lastMode=0xFF;// Refresh menu
          break;
        case MENU_start:
          arduboy.drawBitmap(87,29,&harambeEyes[2],22,2);// are watching you
          arduboy.display();
          break;
        case MENU_help:
          scrollY=0;
          mode=MODE_Help;
          break;
        }
      }
    }
    else if (arduboy.justPressed(B_BUTTON))
    {
      if (mode==MODE_Start)
      {
        switch (menuIdx)
        {
        case MENU_sound:
          if (arduboy.audio.enabled()) arduboy.audio.off();
          else arduboy.audio.on();
          arduboy.audio.saveOnOff();
          lastMode=0xFF;// Refresh menu
          break;
        case MENU_start:
          arduboy.drawBitmap(87,29,&harambeEyes[2],22,2);// are watching you
          arduboy.display();
          break;
        case MENU_help:
          scrollY=0;
          mode=MODE_Help;
          break;
        }
      }
      //Serial.println("Waiting for button release");
      waitRelease=true;
    }
    else if (arduboy.justPressed(LEFT_BUTTON))
    {
      if (mode==MODE_Help)
      {
        if (helpSection>HELP_Harambe) helpSection--; else helpSection=HELP_Tank;
        scrollY=0;
        lastMode=0xFF;// Refresh
      }
    }
    else if (arduboy.justPressed(RIGHT_BUTTON))
    {
      if (mode==MODE_Help)
      {
        if (helpSection<HELP_Tank) helpSection++; else helpSection=HELP_Harambe;
        scrollY=0;
        lastMode=0xFF;// Refresh
      }
    }
    else if (arduboy.justPressed(UP_BUTTON))
    {
      if (mode==MODE_Help)
      {
        if (scrollY>0) scrollY--;
        lastMode=0xFF;// Refresh
      }
      else if (mode==MODE_Start)
      {
        if (menuIdx>MENU_sound) menuIdx--; else menuIdx=MENU_help;
        lastMode=0xFF;// Refresh
      }
      waitRelease=true;
    }
    else if (arduboy.justPressed(DOWN_BUTTON))
    {
      if (mode==MODE_Help)
      {
        if (scrollY+((HEIGHT-8)/8)<pgm_read_byte(&HelpSectionLines[helpSection])) scrollY++;
        lastMode=0xFF;// Refresh
      }
      else if (mode==MODE_Start)
      {
        if (menuIdx<MENU_help) menuIdx++; else menuIdx=MENU_sound;
        lastMode=0xFF;// Refresh menu
      }
      waitRelease=true;
    }
    //Serial.println(mode);
    seed+=rand();// Keep randomizing seed
  }
  return;  
}





