#ifndef STRINGS_H
#define STRINGS_H

#define CHARPERLINE 21;
const char menuStart[] PROGMEM = "Start\n";
const char menuSound[] PROGMEM = "Sound:";
const char menuOff[] PROGMEM = "Off\n";
const char menuOn[] PROGMEM = "On\n";
const char menuHelp[] PROGMEM = "Help";

const char storyLine1[] PROGMEM = "Everyone lost a hero";
const char storyLine2[] PROGMEM = "the day Harambe";
const char storyLine3[] PROGMEM = "died.";   
const char storyLine4[] PROGMEM = "";                 
const char storyLine5[] PROGMEM = "Some thought it was";
const char storyLine6[] PROGMEM = "unfair.";
const char storyLine7[] PROGMEM = "Others took action!";
const char storyLine9[] PROGMEM = "In a far corner of";
const char storyLine10[] PROGMEM = "the world a plan was";
const char storyLine11[] PROGMEM = "hatched to enable...";

const char * const storyLines[] PROGMEM =
{
  storyLine1,
  storyLine2,
  storyLine3,
  storyLine4,
  storyLine5,
  storyLine6,
  storyLine4,
  storyLine7,
  storyLine4,
  storyLine9,
  storyLine10,
  storyLine11
};

#define STORYLINES 12

const char helpHelpText[] PROGMEM = " Nav: Dpad Exit: A";
                                       
const char harambeHelpLine01[] PROGMEM = "You play as Harambe.";
const char harambeHelpLine02[] PROGMEM = "A phantasmal gorilla";
const char harambeHelpLine03[] PROGMEM = "returned from the";
const char harambeHelpLine04[] PROGMEM = "grave to haunt the";
const char harambeHelpLine05[] PROGMEM = "Cincinnati Zoo.";
const char harambeHelpLine06[] PROGMEM = "Harambe has returned";
const char harambeHelpLine07[] PROGMEM = "with the power to";
const char harambeHelpLine08[] PROGMEM = "convert those he";
const char harambeHelpLine09[] PROGMEM = "touches to mutant";
const char harambeHelpLine10[] PROGMEM = "gorilla zombies that";
const char harambeHelpLine11[] PROGMEM = "will do their best to";
const char harambeHelpLine12[] PROGMEM = "protect him. Harambe";
const char harambeHelpLine13[] PROGMEM = "has also been imbued";
const char harambeHelpLine14[] PROGMEM = "with a super speed";
const char harambeHelpLine15[] PROGMEM = "boost which makes him";
const char harambeHelpLine16[] PROGMEM = "temporarily faster";
const char harambeHelpLine17[] PROGMEM = "than a speeding";
const char harambeHelpLine18[] PROGMEM = "bullet.";

const char * const harambeHelpLines[] PROGMEM =
{
  harambeHelpLine01,
  harambeHelpLine02,
  harambeHelpLine03,
  harambeHelpLine04,
  harambeHelpLine05,
  harambeHelpLine06,
  harambeHelpLine07,
  harambeHelpLine08,
  harambeHelpLine09,
  harambeHelpLine10,
  harambeHelpLine11,
  harambeHelpLine12,
  harambeHelpLine13,
  harambeHelpLine14,
  harambeHelpLine15,
  harambeHelpLine16,
  harambeHelpLine17,
  harambeHelpLine18
};

const char touristHelpLine01[] PROGMEM = "Harambe's presence";
const char touristHelpLine02[] PROGMEM = "caused the zoo to";
const char touristHelpLine03[] PROGMEM = "close, but many";
const char touristHelpLine04[] PROGMEM = "tourists still need";
const char touristHelpLine05[] PROGMEM = "evacuated. Convert as";
const char touristHelpLine06[] PROGMEM = "many tourists as you";
const char touristHelpLine07[] PROGMEM = "can to exact";
const char touristHelpLine08[] PROGMEM = "Harambe's Revenge!";
const char touristHelpLine09[] PROGMEM = "Each tourist";
const char touristHelpLine10[] PROGMEM = "converted is worth 3";
const char touristHelpLine11[] PROGMEM = "points. Harambe gets";
const char touristHelpLine12[] PROGMEM = "an extra afterlife";
const char touristHelpLine13[] PROGMEM = "for every 100 points.";

const char * const  touristHelpLines[] PROGMEM =
{
  touristHelpLine01,
  touristHelpLine02,
  touristHelpLine03,
  touristHelpLine04,
  touristHelpLine05,
  touristHelpLine06,
  touristHelpLine07,
  touristHelpLine08,
  touristHelpLine09,
  touristHelpLine10,
  touristHelpLine11,
  touristHelpLine12,
  touristHelpLine13
};
                                      
const char KeeperHelpLine01[] PROGMEM = "The zoo keepers were";
const char KeeperHelpLine02[] PROGMEM = "prepared for Harambe.";
const char KeeperHelpLine03[] PROGMEM = "They wear anti";
const char KeeperHelpLine04[] PROGMEM = "phantasm rain gear";
const char KeeperHelpLine05[] PROGMEM = "that protects them";
const char KeeperHelpLine06[] PROGMEM = "from Harambe's cursed";
const char KeeperHelpLine07[] PROGMEM = "touch. The keeper's";
const char KeeperHelpLine08[] PROGMEM = "guns are loaded with";
const char KeeperHelpLine09[] PROGMEM = "anti phantasm bullets";
const char KeeperHelpLine10[] PROGMEM = "that only harm";
const char KeeperHelpLine11[] PROGMEM = "phantasmal creatures.";

const char * const  keeperHelpLines[] PROGMEM =
{
  KeeperHelpLine01,
  KeeperHelpLine02,
  KeeperHelpLine03,
  KeeperHelpLine04,
  KeeperHelpLine05,
  KeeperHelpLine06,
  KeeperHelpLine07,
  KeeperHelpLine08,
  KeeperHelpLine09,
  KeeperHelpLine10,
  KeeperHelpLine11
};

const char stairsHelpLine01[] PROGMEM = "Once all tourists on";
const char stairsHelpLine02[] PROGMEM = "a stage are converted";
const char stairsHelpLine03[] PROGMEM = "stairs leading to the";
const char stairsHelpLine04[] PROGMEM = "next stage will";
const char stairsHelpLine05[] PROGMEM = "appear. Harambe must";
const char stairsHelpLine06[] PROGMEM = "make it to the stairs";
const char stairsHelpLine07[] PROGMEM = "to complete a stage.";
const char stairsHelpLine08[] PROGMEM = "If a stage is";
const char stairsHelpLine09[] PROGMEM = "completed in less";
const char stairsHelpLine10[] PROGMEM = "than 15 seconds a";
const char stairsHelpLine11[] PROGMEM = "bonus point per";
const char stairsHelpLine12[] PROGMEM = "remaining second will";
const char stairsHelpLine13[] PROGMEM = "be applied. Remember";
const char stairsHelpLine14[] PROGMEM = "Harambe gets an extra";
const char stairsHelpLine15[] PROGMEM = "afterlife for every";
const char stairsHelpLine16[] PROGMEM = "100 points.";

const char * const  stairsHelpLines[] PROGMEM =
{
  stairsHelpLine01,
  stairsHelpLine02,
  stairsHelpLine03,
  stairsHelpLine04,
  stairsHelpLine05,
  stairsHelpLine06,
  stairsHelpLine07,
  stairsHelpLine08,
  stairsHelpLine09,
  stairsHelpLine10,
  stairsHelpLine11,
  stairsHelpLine12,
  stairsHelpLine13,
  stairsHelpLine14,
  stairsHelpLine15,
  stairsHelpLine16
};

const char snakeHelpLine01[] PROGMEM = "Harambe's continued";
const char snakeHelpLine02[] PROGMEM = "presence upsets the";
const char snakeHelpLine03[] PROGMEM = "balance of nature.";
const char snakeHelpLine04[] PROGMEM = "A phantasmal snake";
const char snakeHelpLine05[] PROGMEM = "will appear and";
const char snakeHelpLine06[] PROGMEM = "relentlessly hunt";
const char snakeHelpLine07[] PROGMEM = "Harambe. If the snake";
const char snakeHelpLine08[] PROGMEM = "touches Harambe he";
const char snakeHelpLine09[] PROGMEM = "will lose an";
const char snakeHelpLine10[] PROGMEM = "afterlife. The snakes";
const char snakeHelpLine11[] PROGMEM = "are vulnerable to the";
const char snakeHelpLine12[] PROGMEM = "keeper's anti";
const char snakeHelpLine13[] PROGMEM = "phantasm bullets.";

const char * const  snakeHelpLines[] PROGMEM =
{
  snakeHelpLine01,
  snakeHelpLine02,
  snakeHelpLine03,
  snakeHelpLine04,
  snakeHelpLine05,
  snakeHelpLine06,
  snakeHelpLine07,
  snakeHelpLine08,
  snakeHelpLine09,
  snakeHelpLine10,
  snakeHelpLine11,
  snakeHelpLine12,
  snakeHelpLine13
};
                                     
const char tankHelpLine01[] PROGMEM = "If the zoo keepers";
const char tankHelpLine02[] PROGMEM = "can't contain Harambe";
const char tankHelpLine03[] PROGMEM = "on their own. The";
const char tankHelpLine04[] PROGMEM = "national guard anti";
const char tankHelpLine05[] PROGMEM = "phantasm tank unit";
const char tankHelpLine06[] PROGMEM = "will be deployed to";
const char tankHelpLine07[] PROGMEM = "help with";
const char tankHelpLine08[] PROGMEM = "containment. The tank";
const char tankHelpLine09[] PROGMEM = "shoots anti phantasm";
const char tankHelpLine10[] PROGMEM = "shells that only harm";
const char tankHelpLine11[] PROGMEM = "phantasmal creatures.";

const char * const  tankHelpLines[] PROGMEM =
{
  tankHelpLine01,
  tankHelpLine02,
  tankHelpLine03,
  tankHelpLine04,
  tankHelpLine05,
  tankHelpLine06,
  tankHelpLine07,
  tankHelpLine08,
  tankHelpLine09,
  tankHelpLine10,
  tankHelpLine11
};


const uint8_t HelpSectionLines[] PROGMEM =
{
  18,
  13,
  11,
  16,
  13,
  11
};


const char highScoreText[] PROGMEM = ("High Score: ");
const char stageText[] PROGMEM = ("Stage: ");
const char livesText[] PROGMEM = ("Afterlives: ");
const char scoreText[] PROGMEM = ("Score: ");
const char timeBonusText[] PROGMEM = ("Time Bonus: ");
const char gameOverText[] PROGMEM ="GAME OVER!";
const char may28thText[] PROGMEM ="May 28th";
const char neverForgetText[] PROGMEM ="Never forget!";

#endif //STRINGS_H
