#ifndef _space_
#define _space_

#include "SDL.h"
/*
 *  This file will define the data structures and function prototypes needed for rendering and animating
 *  the scrolling backgrounds for this game.
 *  The functions will create teh entities needed for an interactive level.
 */

#define SPACE_W     1600
#define SPACE_H     900
#define MAX_OBJ     128
#define MAX_ENT     128


typedef struct INFO_TAG
{
  char   name[40];      /*the name of the tag, to keep index hazards away*/
  Uint16 sx,sy;         /*coordinates of spawn point or info tag*/
  Uint16 UnitInfo;      /*used by some spawn types.*/
  Uint16 UnitType;      /*faction affiliation*/
  char   info[40];      /*info tags will need more info*/
  int zcount;
}Tag;

typedef struct LEVEL_STRUCT
{
  Uint8   tilemap[SPACE_H][SPACE_W];
  Uint16  width,height;
  Tag  infolist[MAX_OBJ];
  int infocount;
  Tag  spawnlist[MAX_ENT];
  int spawncount;
  Uint8 tileset;
  char levelname[40];
  char bgimage[40];
  //char bgmusic[40];
}Level;

void GenerateLevel(char *filename,int x, int y);
void GenerateLevel1(int x, int y);
void LoadLevel(char *filename);
void SaveLevel(char *filename);
void DrawLevel();
void DrawLevel1();
void CloseLevel();

#endif
