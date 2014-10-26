#include <stdlib.h>
#include <string.h>
#include "space.h"
#include "entity.h"
//#include "audio.h"

int NumTiles;

extern ScreenData  S_Data;
extern Uint32 rmask,gmask,bmask,amask;
extern SDL_Rect Camera;
extern SDL_Surface *bgimage;
extern SDL_Surface *background;
SDL_Surface *clipmask;
Level level;
int zcount;
int ncount;
//Mix_Music *BgMusic = NULL;

void CloseLevel()
{
  /*if(BgMusic != NULL)
  {
    Mix_FreeMusic(BgMusic);
  }*/
  if(bgimage != NULL)
  {
    SDL_FreeSurface(bgimage);
  }
  if(clipmask != NULL)
  {
    SDL_FreeSurface(clipmask);
  }
}

void GenerateLevel(char *filename,int x, int y)
{
  int i,j,k,l;
  FILE *file;
  char buf[512];
  if(x > SPACE_W)x = SPACE_W;
  if(y > SPACE_H)y = SPACE_H;
  level.width = x;
  level.height = y;
  level.tileset = 1;
  level.infocount = 0;
  level.spawncount = 0;
  strcpy(level.levelname,"Level 1");
  strcpy(level.bgimage,"images/lvltest.png");
  file = fopen(filename, "r");
  if(file==NULL)
  {
	fprintf(stderr,"Unable to open file for writing: %s",SDL_GetError());
	exit(0);
  }
  memset(level.tilemap,0,sizeof(level.tilemap));
  NumTiles=0;
  i=0;
  j=0;
  k=0;
  l=0;
  zcount=0;
  ncount=0;
  while(fscanf(file,"%s",buf)!=EOF)
  {
	  if(strcmp(buf,"#")==0)
	  {
		  fgets(buf,sizeof(buf),file);
		  continue;
	  }
	  else if(strcmp(buf,"<level>")==0)
	  {
		  fgets(buf,sizeof(buf),file);
		  continue;
	  }
	  else if(buf[0]=='1')
	  {
		  for(l=0;l<sizeof(buf)/sizeof(buf[0]);l++)
		  {
			  if(buf[l]=='1')
			  {
				  level.tilemap[j][i]=1;
				  NumTiles++;
				  i++;
			  }
			  else if(buf[l]=='p')
			  {
				  strcpy(level.spawnlist[k].name,"player_start");
				  level.spawnlist[k].sx=128; //i*50
				  level.spawnlist[k].sy=128; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='a')
			  {
				  strcpy(level.spawnlist[k].name,"armor");
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='h')
			  {
				  strcpy(level.spawnlist[k].name,"health");
				  level.spawnlist[k].UnitInfo=0;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='j')
			  {
				  strcpy(level.spawnlist[k].name,"health");
				  level.spawnlist[k].UnitInfo=1;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='k')
			  {
				  strcpy(level.spawnlist[k].name,"health");
				  level.spawnlist[k].UnitInfo=2;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='q')
			  {
				  strcpy(level.spawnlist[k].name,"ammo");
				  level.spawnlist[k].UnitInfo=0;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='w')
			  {
				  strcpy(level.spawnlist[k].name,"ammo");
				  level.spawnlist[k].UnitInfo=1;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='e')
			  {
				  strcpy(level.spawnlist[k].name,"ammo");
				  level.spawnlist[k].UnitInfo=2;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='r')
			  {
				  strcpy(level.spawnlist[k].name,"ammo");
				  level.spawnlist[k].UnitInfo=3;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='t')
			  {
				  strcpy(level.spawnlist[k].name,"ammo");
				  level.spawnlist[k].UnitInfo=4;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='z')
			  {
				  //char text[40];
				  //sprintf(text,"zombie%i",(zcount));
				  strcpy(level.spawnlist[k].name,"zombie");
				  //level.spawnlist[k].name;
				  level.spawnlist[k].UnitInfo=0;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  level.spawnlist[k].zcount=zcount;
				  zcount++;
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  /*else if(buf[l]=='y')FIX ME LATER PLS
			  {
				  //char text[40];
				  //sprintf(text,"zombie%i",(zcount));
				  strcpy(level.spawnlist[k].name,"zombie");
				  //level.spawnlist[k].name;
				  level.spawnlist[k].UnitInfo=6;
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  level.spawnlist[k].ncount=ncount;
				  ncount++;
				  k++;
				  level.spawncount++;
				  i++;
			  }*/
			   else if(buf[l]=='=')
			  {
				  strcpy(level.spawnlist[k].name,"wolf");
				  level.spawnlist[k].sx= i*50;
				  level.spawnlist[k].sy=j*50; //j*50
				  k++;
				  level.spawncount++;
				  i++;
			  }
			  else if(buf[l]=='0')
			  {
				  i++;
			  }
			  else if(buf[l]==';')
			  {
				j++;
				break;
			  }
		  }
		  i=0;
		  fgets(buf,sizeof(buf),file);
	  }

  }
  fclose(file);

  /*
  if(x > SPACE_W)x = SPACE_W;
  if(y > SPACE_H)y = SPACE_H;
  level.width = x;
  level.height = y;
  level.tileset = 1;
  level.infocount = 0;
  level.spawncount = 0;
  strcpy(level.levelname,"Test Level");
  //strcpy(level.bgimage,"images/dojo.png");
  //strcpy(level.bgmusic,"sounds/bgmusic1.ogg");
  memset(level.tilemap,0,sizeof(level.tilemap));
  for(j = 0;j < y;j++)
  {
    level.tilemap[j][0] = 1;
    level.tilemap[j][x - 1] = 1;    
  }
  for(i = 0;i < x;i++)
  {
    level.tilemap[0][i] = 1;
    level.tilemap[y - 1][i] = 1;    
  }*/
}
void GenerateLevel1(int x, int y)
{
  int i,j;
  SDL_Surface *temp, *temp2;
  if(x > SPACE_W)x = SPACE_W;
  if(y > SPACE_H)y = SPACE_H;
  level.width = x;
  level.height = y;
  level.tileset = 1;
  level.infocount = 0;
  level.spawncount = 0;
  strcpy(level.levelname,"Level 1");
  strcpy(level.bgimage,"images/lvltest.png");
  //strcpy(level.bgmusic,"sounds/bgmusic1.ogg");
  memset(level.tilemap,0,sizeof(level.tilemap));
  for(j = 0;j < y;j++)
  {
    level.tilemap[j][0] = 1;
    level.tilemap[j][x - 1] = 1;    
  }
  for(i = 0;i < x;i++)
  {
    level.tilemap[0][i] = 1;
    level.tilemap[y - 1][i] = 1;    
  }
}

/*load the graphics and draw the graphics to the background buffer*/
void DrawLevel()
{
  Sprite *tiles = NULL;
  SDL_Surface *temp;
  int i,j;
  /*first we load the sprite sheet for the background tiles*/
  switch(level.tileset)
  {
    case 1:
    default:
      tiles = LoadSprite("images/50x50tile.png",50,50);
      SDL_SetColorKey(tiles->image, SDL_SRCCOLORKEY , SDL_MapRGB(tiles->image->format, 255,255,255));
  }
  if(tiles == NULL)
  {
    fprintf(stderr,"Unable to load tile set for map: %s \n",SDL_GetError());
    exit(0);
  }
  /*if(BgMusic != NULL)
  {
    Mix_HaltMusic();
    Mix_FreeMusic(BgMusic);
  }*/
  /*then we load the background music*/
  /*BgMusic = Mix_LoadMUS(level.bgmusic);
  if(BgMusic == NULL)
  {
    fprintf(stderr,"Unable to load music: %s \n",SDL_GetError());
    exit(0);
  }
  
  /*And we load the background image*/
  if(bgimage != NULL)SDL_FreeSurface(bgimage);
  temp = IMG_Load("images/lvltest.png");
  if(temp == NULL)
  {
    fprintf(stderr,"Unable to load background image: %s \n",SDL_GetError());
    exit(0);
  }
  bgimage = SDL_DisplayFormat(temp);
  //SDL_SetColorKey(bgimage, SDL_RLEACCEL, 0);
  SDL_FreeSurface(temp);
  
  /*then we check to see if the background image is already in memory and if so, then we shall destroy it*/
  if(background != NULL)SDL_FreeSurface(background);
  temp = SDL_CreateRGBSurface(SDL_HWSURFACE, level.width * 50, level.height * 50, S_Data.depth,rmask, gmask,bmask,amask);
  if(temp == NULL)
  {
    fprintf(stderr,"Couldn't initialize background buffer: %s\n", SDL_GetError());
    exit(1);
  }
  /* Just to make sure that the surface we create is compatible with the screen*/
  background = SDL_DisplayFormatAlpha(temp);
  SDL_FreeSurface(temp);
  
  if(clipmask != NULL)SDL_FreeSurface(clipmask);
  temp = SDL_CreateRGBSurface(SDL_HWSURFACE, level.width * 50, level.height * 50, S_Data.depth,rmask, gmask,bmask,amask);
  if(temp == NULL)
  {
    fprintf(stderr,"Couldn't initialize background buffer: %s\n", SDL_GetError());
    exit(1);
  }
  /* Just to make sure that the surface we create is compatible with the screen*/
  clipmask = SDL_DisplayFormatAlpha(temp);
  SDL_FreeSurface(temp);
 // DrawFilledRect(0,0, background->w,background->h, SDL_MapRGB(background->format,0,0,0), background);
  for(j = 0;j < level.height;j++)
  {
    for(i = 0;i < level.width;i++)
    {
      DrawSprite(tiles,background,i * tiles->w,j * tiles->h,level.tilemap[j][i] - 1);
    }
  }
  SDL_BlitSurface(background,NULL,clipmask,NULL);
  SDL_SetColorKey(background,SDL_RLEACCEL, SDL_MapRGB(background->format,0,0,0));
  FreeSprite(tiles);
  //Mix_VolumeMusic(MIX_MAX_VOLUME>>3);
  //Mix_PlayMusic(BgMusic, -1);
}


void SaveLevel(char *filename)
{
  FILE *file;
  file = fopen(filename, "wb");
  if(file == NULL)
  {
    fprintf(stderr,"Unable to open file for writing: %s",SDL_GetError());
    exit(0);
  }
  if(fwrite(&level, sizeof(level), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to write to file: %s",SDL_GetError());
    exit(0);
  }
  fclose(file);
}

void LoadLevel(char *filename)
{
  FILE *file;
  file = fopen(filename, "rb");
  if(file == NULL)
  {
    fprintf(stderr,"Unable to open file for reading: %s",SDL_GetError());
    exit(0);
  }
  if(fread(&level, sizeof(level), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to read from file: %s",SDL_GetError());
    exit(0);    
  }
  fclose(file);
}

