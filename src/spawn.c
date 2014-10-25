#include <string.h>
#include "spawn.h"
#include "zombies.h"
//#include "worldents.h"
#include "player.h"
#include "space.h"
#include "armor.h"
#include "healthpack.h"
#include "ammo.h"


extern SDL_Surface *screen;
extern SDL_Rect Camera;
extern Level level;
extern Entity *ThePlayer;
extern int zcount;
int MaxSpawns;

Spawn GameSpawns[] = 
{
  {
    {8,12,36,26},             /*bounding box for wall detection*/
    "player_start",                 /*the name of the entity*/
    "images/191x237girl.png",      /*the sprite for the main part of the entity*/
    191,237,                    /*width and height of sprite dimensions*/
  /*  {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },*/                        
    NULL,                /*spawn function*/
    {
      0,0                     /*offset coordinates to draw the legs at*/
    },    
    NULL           
  },
  /*{
    {1,30,1,126},             /*bounding box for wall detection
    "func_door",                 /*the name of the entity
    "images/door.png",      /*the sprite for the main part of the entity
    32,128,                    /*width and height of sprite dimensions
    /*{                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },                        
    NULL,                /*spawn function
   /* {
      0,0                     /*offset coordinates to draw the legs at
    },    
    NULL           
  },*/
  {
    {8,12,36,26},             /*bounding box for wall detection*/
    "zombie",                 /*the name of the entity*/
    "images/191x237zombie.png",      /*the sprite for the main part of the entity*/
    191,237,                    /*width and height of sprite dimensions
    {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },*/                        
    SpawnZombie,                /*spawn function*/
    {
      0,0                     /*offset coordinates to draw the legs at*/
    },    
    NULL           
  },
  {
    {8,12,36,26},             /*bounding box for wall detection*/
    "armor",                 /*the name of the entity*/
    "images/81x110armor.png",      /*the sprite for the main part of the entity*/
    81,110,                    /*width and height of sprite dimensions
    {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },*/                        
    SpawnArmor,                /*spawn function*/
    {
      0,0                     /*offset coordinates to draw the legs at*/
    },    
    NULL           
  },
   {
    {8,12,36,26},             /*bounding box for wall detection*/
    "health",                 /*the name of the entity*/
    "images/50x51bandaid.png",      /*the sprite for the main part of the entity*/
    50,51,                    /*width and height of sprite dimensions
    {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },*/                        
    SpawnHealth,                /*spawn function*/
    {
      0,0                     /*offset coordinates to draw the legs at*/
    },    
    NULL           
  },
  {
    {8,12,36,26},             /*bounding box for wall detection*/
    "ammo",                 /*the name of the entity*/
    "images/73x51ammo9mm.png",      /*the sprite for the main part of the entity*/
    73,51,                    /*width and height of sprite dimensions
    {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },*/                        
    SpawnAmmo,                /*spawn function*/
    {
      0,0                     /*offset coordinates to draw the legs at*/
    },    
    NULL           
  },/*
  {
    {8,12,36,26},             /*bounding box for wall detection
    "ninja",                 /*the name of the entity
    //"images/exploder.png",      /*the sprite for the main part of the entity
    32,32,                    /*width and height of sprite dimensions
   /* {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    },                        
    //SpawnNinja,                /*spawn function
    NULL
  },
  {
     {8,12,36,26},             /*bounding box for wall detection
     "robot",                 /*the name of the entity
     //"images/exploder.png",      /*the sprite for the main part of the entity
     32,32,                    /*width and height of sprite dimensions                     
  /* {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
     },
	// SpawnRobot,                /*spawn function
     NULL
   },
     {
     {8,12,36,26},             /*bounding box for wall detection
     "merc",                 /*the name of the entity
     //"images/exploder.png",      /*the sprite for the main part of the entity
     32,32,                    /*width and height of sprite dimension                      
  /* {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
     },
//	 SpawnMerc,                /*spawn function
     NULL
   }*/
  {
    {0,0,0,0},             /*bounding box for wall detection*/
    "\0",                 /*the name of the entity*/
    "\0",      /*the sprite for the main part of the entity*/
    0,0,                    /*width and height of sprite dimensions
    {                         /*a list of pointers to the wav files that this entity will produce
      "\0",
      "\0",
      "\0",
      "\0"
    }, */                       
    NULL,                /*spawn function*/
    {
      0,0                     /*offset coordinates to draw the legs at*/
    },
    NULL
  }
};

int GetSpawnIndexByName(char EntName[40])
{
  int i;
  for(i =0;i < MaxSpawns;i++)
  {
    if(strncmp(EntName,GameSpawns[i].EntName,40)== 0)return i;
  }
  return -1;/*not found*/
}

/*only after a map's info has been loaded*/
void PrecacheSpawns()                       
{
  int i,j;
  i =0;
  for(i = 0;i < level.spawncount;i++)
  {
    /*for(j = 0;j < SOUNDSPERENT;i++)
    {
      if(GameSpawns[GetSpawnIndexByName(level.spawnlist[i].name)].sound[j][0] != '\0')
      {
        LoadSound(GameSpawns[GetSpawnIndexByName(level.spawnlist[i].name)].sound[j],SDL_MIX_MAXVOLUME>>4);
      }
    }*/
  }
}

void LoadSpawnSprites()
{
  int index;
  int i = 0;
  while(strncmp(GameSpawns[i].EntName,"\0",40 )!=0)
  {
    i++;
  }
  MaxSpawns = i + zcount;
  for(index = 0;index < MaxSpawns;index++)
  {
    GameSpawns[index].mapsprite = LoadSprite(GameSpawns[index].sprite,GameSpawns[index].sw,GameSpawns[index].sh);
  }
}


void DrawSpawnPoints()
{
  int i;
  for(i = 0;i < level.spawncount;i++)
  {
    DrawSpawn(GetSpawnIndexByName(level.spawnlist[i].name),level.spawnlist[i].sx - Camera.x,level.spawnlist[i].sy - Camera.y);    
  }
}

/*draws the desired spawn candidate at the location*/
void DrawSpawn(int index,int sx, int sy)     
{
  if(GameSpawns[index].mapsprite != NULL)
  DrawSprite(GameSpawns[index].mapsprite,screen,sx,sy,0);
  /*by not freeing the sprite, I ensure that it only gets loaded from disk once.*/
}

void SpawnAll(int initial)                               /*after map is loaded, start all entities*/
{
  int aindex;
  int sindex;
  int i = 0;
  aindex=0;
  while(strncmp(GameSpawns[i].EntName,"\0",40 )!=0)
  {
    i++;
  }
  MaxSpawns = i + zcount;

  for(i = 0;i < level.spawncount;i++)
  {
    sindex = GetSpawnIndexByName(level.spawnlist[i].name);
    if(GameSpawns[sindex].spawn != NULL)
      GameSpawns[sindex].spawn(NULL,level.spawnlist[i].sx,level.spawnlist[i].sy,level.spawnlist[i].UnitInfo,level.spawnlist[i].UnitType);
    else
    {
      if(strncmp(level.spawnlist[i].name,"player_start",40) == 0)
      {
        if(initial == 1)
        {
          SpawnPlayer(level.spawnlist[i].sx,level.spawnlist[i].sy);
        }
        else
        {
          ThePlayer->s.x = level.spawnlist[i].sx;
          ThePlayer->s.y = level.spawnlist[i].sy;
          ThePlayer->v.x = 0;
          ThePlayer->v.y = 0;
          ThePlayer->a.x = 0;
          ThePlayer->a.y = 0;
          UpdateEntityPosition(ThePlayer,0);
        }
      }
      if(strncmp(level.spawnlist[i].name,"func_door",40) == 0)
      {
//        SetDoor(level.spawnlist[i].sx, level.spawnlist[i].sy, 0,0,0);
      }
    }
  }
}
