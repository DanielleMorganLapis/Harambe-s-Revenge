#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "GameMath.h"

typedef struct
{
  uint8_t index;
  char initials[3];
  uint8_t stage;
  uint16_t score;
  uint8_t crc;
} HighScoreEntry;


// -1 return indicates no valid score entry was found
int16_t ReadHighScore(HighScoreEntry &tScore)
{
  //Serial.println("Entering ReadHighScore");
  int16_t address=EEPROM_STORAGE_SPACE_START,lastGood=-1;
  uint8_t *data=(uint8_t*)&tScore;
  int16_t ofs;
  uint8_t lastIndex=0;
  uint8_t tCrc;
  char tInitials[4];
  
  while ((address+sizeof(HighScoreEntry))<EEPROM.length())
  {
    /*Serial.print("address=0x");
    Serial.println(address,HEX);*/
    for (ofs=0;ofs<sizeof(HighScoreEntry);ofs++) data[ofs]=EEPROM[address+ofs];
    tCrc=crc8(data,sizeof(HighScoreEntry)-sizeof(tScore.crc));
    /*Serial.print("tScore.index=");
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
    if (tCrc==tScore.crc)
    {      
      if ((address==EEPROM_STORAGE_SPACE_START)
          ||
          (lastIndex==(tScore.index-1))
          )
      {
        //Serial.println("Found current entry.");
        lastIndex=tScore.index;
        lastGood=address;
      }
      else
      {
        //Serial.println("Found old entry.");
        // Since crc is valid this is likely an old entry
        // We have found the last good entry already unless lastGood is -1
        break;
      }        
      address+=sizeof(HighScoreEntry);
    }
    else
    {
      // Invalid crc
      //Serial.println("Invalid CRC.");
      // We have found the last good entry already unless lastGood is -1
      break;
    }
  }
  if ((lastGood>=EEPROM_STORAGE_SPACE_START)
      &&
      ((lastGood+sizeof(HighScoreEntry))<EEPROM.length())
      )
  {
    for (ofs=0;ofs<sizeof(HighScoreEntry);ofs++) data[ofs]=EEPROM[lastGood+ofs];
  }
  else
  {
    //Default entry to 0 score
    memset(data,0,sizeof(HighScoreEntry));
  }
  /*Serial.print("Exiting ReadHighScore(return=");
  Serial.print(lastGood);
  Serial.println(")");*/
  return lastGood;
}

// address should be the lastGood entry that ReadHighScore returns.  Index and crc are calculated automatically so user only needs to set initials, stage and score.
void WriteHighScore(int16_t address,HighScoreEntry &tScore)
{
  //Serial.println("Entering WriteHighScore");
  int16_t ofs;
  uint8_t *data=(uint8_t*)&tScore;
  //char tInitials[4];
  if ((address>=EEPROM_STORAGE_SPACE_START)
      &&
      // Making sure we have enough room after the last good entry
      ((address+(sizeof(HighScoreEntry)*2))<EEPROM.length())
      )
  {
    // Using valid entry to set index
    tScore.index=EEPROM[address]+1;
    // And of course we want the next slot
    address+=sizeof(HighScoreEntry);
  }
  else
  {
    if (address==-1)
    {
      tScore.index=0;
    }
    // Using valid entry to set index.  Trusting ReadHighScore won't give us anything but -1 if the address is invalid
    else
    {
      tScore.index=EEPROM[address]+1;
    }
    // Starting over at the beginning of user EEPROM
    address=EEPROM_STORAGE_SPACE_START;
  }
  /*Serial.print("address=0x");
  Serial.println(address,HEX);*/
  tScore.crc=crc8(data,sizeof(HighScoreEntry)-sizeof(tScore.crc));
  /*Serial.print("tScore.index=");
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
  for (ofs=0;ofs<sizeof(HighScoreEntry);ofs++)
  {
    //Heard update can reduce wear on the flash
    EEPROM.update(address+ofs,data[ofs]);
  }
  //Serial.println("Exiting WriteHighScore");
}

#endif //HIGHSCORE_H
