#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "particle.h"
#include "weapons.h"
#include "menu.h";

#define  JT_Button  0
#define  JT_Axis    1
#define  JT_Inverse 2
#define  Wiggle     6000
#define  AWiggle    -6000

extern SDL_Rect Camera;
extern SDL_Surface *screen;
extern SDL_Joystick *joy;
extern SDL_Surface *bg;
extern SDL_Surface *background;

enum PlayerInputs {PI_MovDown,PI_MovDownLeft,PI_MovLeft,PI_MovUpLeft,PI_MovUp,PI_MovUpRight,PI_MovRight,PI_MovDownRight,
                   PI_FireDown,PI_FireDownLeft,PI_FireLeft,PI_FireUpLeft,PI_FireUp,PI_FireUpRight,PI_FireRight,PI_FireDownRight,
                   PI_NextWeapon,PI_PreviousWeapon,PI_Pause,PI_Fire,PI_Reload,PI_NULL};

int   JoyButtons[PI_NULL][2];
Uint32 KeyButtons[PI_NULL];

int PlayerCommands[3]; 
Entity *ThePlayer = NULL;
SDL_Surface *titlebar = NULL;
int attackleft = SDLK_LEFT;
void UpdatePlayer(Entity *self);
void PlayerThink(Entity *self);
void FinalOutput();
void UpdateInput();

PlayerStats ThisPlayer_Stats;

void SpawnPlayer(int x,int y)
{
  Entity *newent = NULL;
  newent = NewEntity();
  if(newent == NULL)
  {
    fprintf(stderr,"Unable to generate player entity; %s",SDL_GetError());
    exit(0);
  }
  strcpy(newent->EntName,"Player\0");
  newent->sprite = LoadSwappedSprite("images/191x237girl.png",191,237);
  newent->weapon = LoadSwappedSprite("images/191x237fists.png",191,237);
  /*ThisPlayer_Stats.color[PC_Core1] = Blue;
  ThisPlayer_Stats.color[PC_Core2] = DarkRed;
  ThisPlayer_Stats.color[PC_Leg1] = Blue;
  ThisPlayer_Stats.color[PC_Leg2] = DarkRed;
  ThisPlayer_Stats.color[PC_Leg3] = Silver;
  ThisPlayer_Stats.color[PC_Gun] = DarkGrey;
  ThisPlayer_Stats.color[PC_Trail] = LightBlue;*/
  //SDL_SetColorKey(newent->sprite->image, SDL_SRCCOLORKEY , SDL_MapRGB(newent->sprite->image->format, 0,0,0));
  //SDL_SetColorKey(newent->legs->image, SDL_SRCCOLORKEY , SDL_MapRGB(newent->legs->image->format, 0,0,0));
  newent->size.x = newent->sprite->w;
  newent->size.y = newent->sprite->h;
  newent->update = UpdatePlayer;
  newent->think = PlayerThink;
  newent->UpdateRate = 35;
  newent->ThinkRate = 35;
  newent->weaponstate = 1;
  newent->takedamage = 1;
  newent->XP = 0;
  newent->Unit_Type = ET_Player;
  newent->switchdelay = 0;
  newent->healthmax = 100;
  newent->health = 100;
  newent->frame = 0;
  newent->Color = LightBlue;
//  newent->fcount = B_Tiny;
  newent->frate = 1;
  newent->weaponframe = 0;
  newent->state = ST_IDLE;
  newent->aimdir = F_East;
  //newent->thick = 1; WHAT IS THIS
  //newent->trailhead = -1;
  newent->s.x = x;
  newent->s.y = y;
  newent->v.x = 0;
  newent->v.y = 0;
  newent->fs.x = x;//FISTS offset
  newent->fs.y = y;//FISTS offset
  newent->maxspeed = 25;
  newent->movespeed = 0;
  newent->accel = 4;
  newent->totaloffset = 30;
  newent->Boundingbox.x = 10;
  newent->Boundingbox.y = 10;
  newent->Boundingbox.w = 191;
  newent->Boundingbox.h = 237;  
  newent->origin.x = 24;
  newent->origin.y = 32;
  newent->currentweapon = 0;
  UpdatePlayer(newent);
  ThePlayer = newent;
  newent->jump=1;
  newent->right=1;
  newent->gs.x=x;
  newent->gs.y=y;
  newent->reload=1;
  newent->ss.x=x;
  newent->ss.y=y;
  newent->swordlvl=1;
  newent->gclip=1;
  newent->dclip=1;
  newent->ds.y=y;
  newent->ds.x=x;
  newent->aks.x=x;
  newent->aks.y=y;
  newent->akmag=1;
  newent->acrs.x=x;
  newent->acrs.y=y;
  newent->acrmag=1;
  newent->shotys.x=x;
  newent->shotys.y=y;
  newent->shells=1;
  newent->maxgclip=15;
  newent->maxacrclip=30;
  newent->maxakclip=30;
  newent->maxdclip=8;
  newent->maxshells=8;
  newent->total12ga=0;
  newent->total50ae=0;
  newent->total556=0;
  newent->total762=0;
  newent->total9mm=0;
  newent->armor=0;
  newent->armormax=100;
  atexit(FinalOutput);
}

/*
 * Think Function handles giving the entity new orders and changing states
 */
void PlayerThink(Entity *self)
{
  float t;
  char text[40];
  Uint8 *keys = SDL_GetKeyState(NULL);
  UpdateInput();
 /*while(self->grounded==0){
	  self->v.y -=95;
  }*/
  if(self->switchdelay <= 0)
  {
	  if(PlayerCommands[1] == PI_Fire && self->reload==1)
	{
		printf("eggs");
		if(self->currentweapon==0)
		{
			if(self->aimdir==F_East && self->right==1)
			{
				self->weaponframe++;
				//if(self->weaponframe=1)self->state=ST_ATTACK;
				if(self->weaponframe>1)self->weaponframe=0;
			}
			if(self->aimdir==F_West && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>3 ||self->weaponframe<2)self->weaponframe=2;
			}
			if(self->aimdir==F_SW && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>11 ||self->weaponframe<10)self->weaponframe=10;
			}
			if(self->aimdir==F_NW && self->right==0)
			{
				self->weaponframe--;
				if(self->weaponframe<4 || self->weaponframe>5)self->weaponframe=5;
			}
			if(self->aimdir==F_SE && self->right==1)
			{
				self->weaponframe--;
				if(self->weaponframe<8 ||self->weaponframe>9)self->weaponframe=9;
			}
			if(self->aimdir==F_NE && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe<5 || self->weaponframe>7)self->weaponframe=6;
			}
			if(self->aimdir==F_North && self->right==1)
			{
				self->weaponframe--;
				if(self->weaponframe<12 || self->weaponframe>13)self->weaponframe=13;
			}
			if(self->aimdir==F_North && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe<15 || self->weaponframe>17)self->weaponframe=16;
			}
			if(self->aimdir==F_South && self->right==1)
			{
				self->weaponframe--;
				if(self->weaponframe<14 || self->weaponframe>15)self->weaponframe=15;
			}
			if(self->aimdir==F_South && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe<17 || self->weaponframe>19)self->weaponframe=18;
			}
		}
		if(self->currentweapon==1){
			if(self->aimdir==F_East && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>1)self->weaponframe=0;
			}
			if(self->aimdir==F_West && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>3 ||self->weaponframe<2)self->weaponframe=2;
			}
			if(self->aimdir==F_SW && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>11 ||self->weaponframe<10)self->weaponframe=10;
			}
			if(self->aimdir==F_NW && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>7 || self->weaponframe<6)self->weaponframe=6;
			}
			if(self->aimdir==F_SE && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>10 ||self->weaponframe<9)self->weaponframe=9;
			}
			if(self->aimdir==F_NE && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>5 || self->weaponframe<4)self->weaponframe=4;
			}
		}
		if(self->currentweapon==2)
		{
		  if(self->gclip>0){
			if(self->aimdir==F_East && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>1)self->weaponframe=0;
			}
			if(self->aimdir==F_West && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>23 ||self->weaponframe<22)self->weaponframe=22;
			}
			if(self->aimdir==F_SW && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>27 ||self->weaponframe<26)self->weaponframe=26;
			}
			if(self->aimdir==F_NW && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>7 || self->weaponframe<6)self->weaponframe=6;
			}
			if(self->aimdir==F_SE && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>3 ||self->weaponframe<2)self->weaponframe=2;
			}
			if(self->aimdir==F_NE && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>21 || self->weaponframe<20)self->weaponframe=20;
			}
			if(self->aimdir==F_North && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>25 || self->weaponframe<24)self->weaponframe=24;
			}
			if(self->aimdir==F_North && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>5 || self->weaponframe<4)self->weaponframe=4;
			}
			if(self->aimdir==F_South && self->right==1)
			{
				self->weaponframe++;
				if(self->weaponframe>9 || self->weaponframe<8)self->weaponframe=8;
			}
			if(self->aimdir==F_South && self->right==0)
			{
				self->weaponframe++;
				if(self->weaponframe>29 || self->weaponframe<28)self->weaponframe=28;
			}
		  }
		  if(self->gclip==0)
		  {
			if(self->aimdir==F_East && self->right==1)self->weaponframe=0;
			if(self->aimdir==F_West && self->right==0)self->weaponframe=22;
			if(self->aimdir==F_SW && self->right==0)self->weaponframe=26;
			if(self->aimdir==F_NW && self->right==0)self->weaponframe=6;
			if(self->aimdir==F_SE && self->right==1)self->weaponframe=2;
			if(self->aimdir==F_NE && self->right==1)self->weaponframe=20;
			if(self->aimdir==F_North && self->right==1)self->weaponframe=24;
			if(self->aimdir==F_North && self->right==0)self->weaponframe=4;
			if(self->aimdir==F_South && self->right==1)self->weaponframe=8;
			if(self->aimdir==F_South && self->right==0)self->weaponframe=28;
		  }
	  }
	  if(self->currentweapon==3)
		{
			if(self->dclip>0)
			{
				if(self->aimdir==F_East && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>1)self->weaponframe=0;
				}
				if(self->aimdir==F_West && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>5 ||self->weaponframe<4)self->weaponframe=4;
				}
				if(self->aimdir==F_SW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>11 ||self->weaponframe<10)self->weaponframe=10;
				}
				if(self->aimdir==F_NW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>7 || self->weaponframe<6)self->weaponframe=6;
				}
				if(self->aimdir==F_SE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>9 ||self->weaponframe<8)self->weaponframe=8;
				}
				if(self->aimdir==F_NE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>3 || self->weaponframe<2)self->weaponframe=2;
				}
				if(self->aimdir==F_North && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>13 || self->weaponframe<12)self->weaponframe=12;
				}
				if(self->aimdir==F_North && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>15 || self->weaponframe<14)self->weaponframe=14;
				}
				if(self->aimdir==F_South && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>17 || self->weaponframe<16)self->weaponframe=16;
				}
				if(self->aimdir==F_South && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>19 || self->weaponframe<18)self->weaponframe=18;
				}
				self->state=ST_ATTACK;
			}
			if(self->dclip==0)
			{
				if(self->aimdir==F_East && self->right==1)self->weaponframe=0;
				if(self->aimdir==F_West && self->right==0)self->weaponframe=4;
				if(self->aimdir==F_SW && self->right==0)self->weaponframe=10;
				if(self->aimdir==F_NW && self->right==0)self->weaponframe=6;
				if(self->aimdir==F_SE && self->right==1)self->weaponframe=8;
				if(self->aimdir==F_NE && self->right==1)self->weaponframe=2;
				if(self->aimdir==F_North && self->right==1)self->weaponframe=12;
				if(self->aimdir==F_North && self->right==0)self->weaponframe=14;
				if(self->aimdir==F_South && self->right==1)self->weaponframe=16;
				if(self->aimdir==F_South && self->right==0)self->weaponframe=18;
			}
		}
	  if(self->currentweapon==4)
		{
			if(self->akmag>0){
				if(self->aimdir==F_East && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>1)self->weaponframe=0;
				}
				if(self->aimdir==F_West && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>3 ||self->weaponframe<2)self->weaponframe=2;
				}
				if(self->aimdir==F_SW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>9 ||self->weaponframe<8)self->weaponframe=8;
				}
				if(self->aimdir==F_NW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>7 || self->weaponframe<6)self->weaponframe=6;
				}
				if(self->aimdir==F_SE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>11 ||self->weaponframe<10)self->weaponframe=10;
				}
				if(self->aimdir==F_NE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>5 || self->weaponframe<4)self->weaponframe=4;
				}
				if(self->aimdir==F_North && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>19 || self->weaponframe<18)self->weaponframe=18;
				}
				if(self->aimdir==F_North && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>17 || self->weaponframe<16)self->weaponframe=16;
				}
				if(self->aimdir==F_South && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>13 || self->weaponframe<12)self->weaponframe=12;
				}
				if(self->aimdir==F_South && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>15 || self->weaponframe<14)self->weaponframe=14;
				}
		  }
		  if(self->akmag==0)
		  {
			if(self->aimdir==F_East && self->right==1)self->weaponframe=0;
			if(self->aimdir==F_West && self->right==0)self->weaponframe=2;
			if(self->aimdir==F_SW && self->right==0)self->weaponframe=8;
			if(self->aimdir==F_NW && self->right==0)self->weaponframe=6;
			if(self->aimdir==F_SE && self->right==1)self->weaponframe=10;
			if(self->aimdir==F_NE && self->right==1)self->weaponframe=4;
			if(self->aimdir==F_North && self->right==1)self->weaponframe=18;
			if(self->aimdir==F_North && self->right==0)self->weaponframe=16;
			if(self->aimdir==F_South && self->right==1)self->weaponframe=12;
			if(self->aimdir==F_South && self->right==0)self->weaponframe=14;
		  }
	  }
	   if(self->currentweapon==5)
		{
			if(self->acrmag>0)
			{
				if(self->aimdir==F_East && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>1)self->weaponframe=0;
				}
				if(self->aimdir==F_West && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>3 ||self->weaponframe<2)self->weaponframe=2;
				}
				if(self->aimdir==F_SW && self->right==0)
				{
					self->weaponframe++;
				if(self->weaponframe>11 ||self->weaponframe<10)self->weaponframe=10;
				}
				if(self->aimdir==F_NW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>5 || self->weaponframe<4)self->weaponframe=4;
				}
				if(self->aimdir==F_SE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>9 ||self->weaponframe<8)self->weaponframe=8;
				}
				if(self->aimdir==F_NE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>7 || self->weaponframe<6)self->weaponframe=6;
				}
				if(self->aimdir==F_North && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>15 || self->weaponframe<14)self->weaponframe=14;
				}
				if(self->aimdir==F_North && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>13 || self->weaponframe<12)self->weaponframe=12;
				}
				if(self->aimdir==F_South && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>19 || self->weaponframe<18)self->weaponframe=18;
				}
				if(self->aimdir==F_South && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>17 || self->weaponframe<16)self->weaponframe=16;
				}
			}
			if(self->acrmag==0)
			{
				if(self->aimdir==F_East && self->right==1)self->weaponframe=0;
				if(self->aimdir==F_West && self->right==0)self->weaponframe=2;
				if(self->aimdir==F_SW && self->right==0)self->weaponframe=10;
				if(self->aimdir==F_NW && self->right==0)self->weaponframe=4;
				if(self->aimdir==F_SE && self->right==1)self->weaponframe=8;
				if(self->aimdir==F_NE && self->right==1)self->weaponframe=6;
				if(self->aimdir==F_North && self->right==1)self->weaponframe=14;
				if(self->aimdir==F_North && self->right==0)self->weaponframe=12;
				if(self->aimdir==F_South && self->right==1)self->weaponframe=18;
				if(self->aimdir==F_South && self->right==0)self->weaponframe=16;
			}
	   }
	   if(self->currentweapon==6)
	   {
			if(self->shells>0)
			{
				if(self->aimdir==F_East && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>1)self->weaponframe=0;
				}
				if(self->aimdir==F_West && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>3 ||self->weaponframe<2)self->weaponframe=2;
				}
				if(self->aimdir==F_SW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>11 ||self->weaponframe<10)self->weaponframe=10;
				}
				if(self->aimdir==F_NW && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>5 || self->weaponframe<4)self->weaponframe=4;
				}
				if(self->aimdir==F_SE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>9 ||self->weaponframe<8)self->weaponframe=8;
				}
				if(self->aimdir==F_NE && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>7 || self->weaponframe<6)self->weaponframe=6;
				}
				if(self->aimdir==F_North && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>13 || self->weaponframe<12)self->weaponframe=12;
				}
				if(self->aimdir==F_North && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>15 || self->weaponframe<14)self->weaponframe=14;
				}
				if(self->aimdir==F_South && self->right==1)
				{
					self->weaponframe++;
					if(self->weaponframe>17 || self->weaponframe<16)self->weaponframe=16;
				}
				if(self->aimdir==F_South && self->right==0)
				{
					self->weaponframe++;
					if(self->weaponframe>19 || self->weaponframe<18)self->weaponframe=18;
				}
			}
			if(self->shells==0)
			{
				if(self->aimdir==F_East && self->right==1)self->weaponframe=0;
				if(self->aimdir==F_West && self->right==0)self->weaponframe=2;
				if(self->aimdir==F_SW && self->right==0)self->weaponframe=10;
				if(self->aimdir==F_NW && self->right==0)self->weaponframe=4;
				if(self->aimdir==F_SE && self->right==1)self->weaponframe=8;
				if(self->aimdir==F_NE && self->right==1)self->weaponframe=6;
				if(self->aimdir==F_North && self->right==1)self->weaponframe=12;
				if(self->aimdir==F_North && self->right==0)self->weaponframe=14;
				if(self->aimdir==F_South && self->right==1)self->weaponframe=16;
				if(self->aimdir==F_South && self->right==0)self->weaponframe=18;
			}
	}
	  self->state=ST_ATTACK;
    }
	if(PlayerCommands[1] == PI_NextWeapon)
    {
	  self->currentweapon++;
      if(self->currentweapon >= NumWeapons)self->currentweapon = 0;
      self->switchdelay = 4;
      sprintf(text,"Next Weapon : %s",PrintWeaponName(self->currentweapon));
      NewMessage(text,IndexColor(LightBlue));
	  if(self->currentweapon==0)
		{
			if(self->right==1)
			{
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				self->weaponframe=3;
			}
			self->weapon=LoadSwappedSprite("images/191x237fists.png",191,237);
		}
	  if(self->currentweapon==1)
		{
			if(self->right==1)
			{
				if(self->swordlvl==2)self->ss.y+=150;
				if(self->swordlvl==0)self->ss.y-=150;
				self->swordlvl=1;
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				if(self->swordlvl==2)self->ss.y+=150;
				if(self->swordlvl==0)self->ss.y-=150;
				self->swordlvl=1;
				self->weaponframe=2;
			}
			self->weapon=LoadSwappedSprite("images/305x305SWORD2.png",305,305);
		}
	  if(self->currentweapon==2)
		{
			if(self->right==1)
			{
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				self->weaponframe=22;
			}
			self->weapon=LoadSwappedSprite("images/130x195glock.png",130,195);
		}
	  if(self->currentweapon==3)
		{
			if(self->right==1)
			{
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				self->weaponframe=4;
			}
			self->weapon=LoadSwappedSprite("images/300x300deagle.png",300,300);
		}
	   if(self->currentweapon==4)
		{
			if(self->right==1)
			{
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				self->weaponframe=2;
			}
			self->weapon=LoadSwappedSprite("images/400x336ak74.png",400,336);
		}
	    if(self->currentweapon==5)
		{
			if(self->right==1)
			{
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				self->weaponframe=2;
			}
			self->weapon=LoadSwappedSprite("images/400x400acr.png",400,400);
		}
		 if(self->currentweapon==6)
		{
			if(self->right==1)
			{
				self->weaponframe=0;
			}
			if(self->right==0)
			{
				self->weaponframe=2;
			}
			self->weapon=LoadSwappedSprite("images/375x375shotty.png",375,375);
		}
    }
	if(PlayerCommands[1]==PI_Reload)
	{
		int ammo,remainder;
		//printf("YOU RELOAD THAT SHIT");
		if(self->currentweapon==2)
		{	
			if(self->gclip < self->maxgclip && self->right==1 && self->total9mm>0){
				if(self->reload==1)self->weaponframe=10;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==15)
				{
					self->weaponframe=0;
					self->reload=1;
					ammo=self->total9mm%self->maxgclip;
					if(ammo==0)ammo=15;
					self->total9mm-=ammo;
					self->gclip+=ammo;
					if(self->gclip > self->maxgclip)
					{
						remainder=self->gclip-self->maxgclip;
						self->total9mm+=remainder;
						self->gclip=self->maxgclip;
					}
				}
			}
			if(self->gclip< self->maxgclip && self->right==0 && self->total9mm>0){
				if(self->reload==1)self->weaponframe=19;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe--;
				}
				if(self->weaponframe==16)
				{
					self->weaponframe=22;
					self->reload=1;
					ammo=self->total9mm%self->maxgclip;
					if(ammo==0)ammo=15;
					self->total9mm-=ammo;
					self->gclip+=ammo;
					if(self->gclip > self->maxgclip)
					{
						remainder=self->gclip-self->maxgclip;
						self->total9mm+=remainder;
						self->gclip=self->maxgclip;
					}
				}
			}
		}
		if(self->currentweapon==3)
		{	
			if(self->dclip < self->maxdclip && self->right==1 && self->total50ae>0)
			{
				if(self->reload==1)self->weaponframe=20;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==25)
				{
					self->weaponframe=0;
					self->reload=1;
					ammo=self->total50ae%self->maxdclip;
					if(ammo==0)ammo=8;
					self->total50ae-=ammo;
					self->dclip+=ammo;
					if(self->dclip > self->maxdclip)
					{
						remainder=self->dclip-self->maxdclip;
						self->total50ae+=remainder;
						self->dclip=self->maxdclip;
					}
				}
			}
			if(self->dclip < self->maxdclip && self->right==0 && self->total50ae>0)
			{
				if(self->reload==1)self->weaponframe=25;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==30)
				{
					self->weaponframe=4;
					self->reload=1;
					ammo=self->total50ae%self->maxdclip;
					if(ammo==0)ammo=8;
					self->total50ae-=ammo;
					self->dclip+=ammo;
					if(self->dclip > self->maxdclip)
					{
						remainder=self->dclip-self->maxdclip;
						self->total50ae+=remainder;
						self->dclip=self->maxdclip;
					}
				}
			}
		}
		if(self->currentweapon==4)
		{	
			if(self->akmag < self->maxakclip && self->right==1 && self->total762 > 0)
			{
				if(self->reload==1)self->weaponframe=20;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==25)
				{
					self->weaponframe=0;
					self->reload=1;
					ammo=self->total762%self->maxakclip;
					if(ammo==0)ammo=30;
					self->total762-=ammo;
					self->akmag+=ammo;
					if(self->akmag > self->maxakclip)
					{
						remainder=self->akmag-self->maxakclip;
						self->total762+=remainder;
						self->akmag=self->maxakclip;
					}
				}
			}
			if(self->akmag < self->maxakclip && self->right==0 && self->total762 > 0)
			{
				if(self->reload==1)self->weaponframe=25;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==30)
				{
					self->weaponframe=4;
					self->reload=1;
					ammo=self->total762%self->maxakclip;
					if(ammo==0)ammo=30;
					self->total762-=ammo;
					self->akmag+=ammo;
					if(self->akmag > self->maxakclip)
					{
						remainder=self->akmag-self->maxakclip;
						self->total762+=remainder;
						self->akmag=self->maxakclip;
					}
				}
			}
		}
		if(self->currentweapon==5)
		{	
			if(self->acrmag < self->maxacrclip && self->right==1 && self->total556 > 0)
			{
				if(self->reload==1)self->weaponframe=20;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==25)
				{
					self->weaponframe=0;
					self->reload=1;
					ammo=self->total556%self->maxacrclip;
					if(ammo==0)ammo=30;
					self->total556-=ammo;
					self->acrmag+=ammo;
					if(self->acrmag > self->maxacrclip)
					{
						remainder=self->acrmag-self->maxacrclip;
						self->total556+=remainder;
						self->acrmag=self->maxacrclip;
					}
				}
			}
			if(self->acrmag<self->maxacrclip && self->right==0 && self->total556 > 0)
			{
				if(self->reload==1)self->weaponframe=25;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==30)
				{
					self->weaponframe=2;
					self->reload=1;
					ammo=self->total556%self->maxacrclip;
					if(ammo==0)ammo=30;
					self->total556-=ammo;
					self->acrmag+=ammo;
					if(self->acrmag > self->maxacrclip)
					{
						remainder=self->acrmag-self->maxacrclip;
						self->total556+=remainder;
						self->acrmag=self->maxacrclip;
					}
				}
			}
		}
		if(self->currentweapon==6)
		{	
			if(self->shells < self->maxshells && self->right==1 && self->total12ga > 0)
			{
				if(self->reload==1)self->weaponframe=20;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==24)
				{
					self->weaponframe=20;
					self->reload=1;
					self->shells++;
					self->total12ga--;
				}
			}
			if(self->shells< self->maxshells && self->right==0 && self->total12ga > 0)
			{
				if(self->reload==1)self->weaponframe=25;
				self->reload=0;
				if(self->reload==0){
					self->weaponframe++;
				}
				if(self->weaponframe==29)
				{
					self->weaponframe=25;
					self->reload=1;
					self->shells++;
					self->total12ga--;
				}
			}
		}
	}
    /*if(PlayerCommands[1] == PI_PreviousWeapon)
    {
      self->currentweapon--;
      if(self->currentweapon < 0)self->currentweapon = NumWeapons - 1;
      self->switchdelay = 4;
      sprintf(text,"Last Weapon : %s",PrintWeaponName(self->currentweapon));
      NewMessage(text,IndexColor(LightBlue));
    } */   
  }
  else self->switchdelay--;
  if((self->state != ST_DEAD)&&(self->state != ST_DIE))
  {
    /*testing (cheating)
    if(keys[SDLK_1])self->PowerLevel = 0;
    else if(keys[SDLK_2])self->PowerLevel = 1;
    else if(keys[SDLK_3])self->PowerLevel = 2;
    else if(keys[SDLK_4])self->PowerLevel = 3;*/
    /*player movement follows*/
	  if(PlayerCommands[0] == PI_MovUpLeft)
    {     
	  if(self->right==1)
	  {
		  self->aimdir=F_West;
		  self->fs.x-=150;
		  self->gs.x-=150;
		  self->ss.x-=150;
		  self->ds.x-=50;
		  self->aks.x-=50;
		  self->acrs.x-=150;
		  self->shotys.x-=150;
		  if(self->currentweapon==0)self->weaponframe=3;
		  if(self->currentweapon==1)
		  {
			  if(self->swordlvl==0)self->weaponframe=10;
			  if(self->swordlvl==1)self->weaponframe=2;
			  if(self->swordlvl==2)self->weaponframe=6;
		  }
		  if(self->currentweapon==2)self->weaponframe=22;
		  if(self->currentweapon==3)self->weaponframe=4;
		  if(self->currentweapon==4)self->weaponframe=2;
		  if(self->currentweapon==5)self->weaponframe=2;
		  if(self->currentweapon==6)self->weaponframe=2;
		  self->right=0;
	  }
	  if(UpdateEntityPosition(self,0)==0)self->s.y+=20;
	  self->right=0;
	  if(self->jump==1){
			self->v.y=-30;
			self->jump=0;
			t = self->accel * 3;
			self->v.x-=t;
		}
		if(self->jump==0)
		{
			self->s.y-=4;
		}
		if((self->frame>=29)||(self->frame<=25))self->frame=29;
		self->frame--;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,1.4,1.4,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovUpRight)
    {     
		if(self->right==0)
	  {
		 self->aimdir=F_East;
		 self->fs.x+=150;
		 self->gs.x+=150;
		 self->ss.x+=150;
		 self->ds.x+=50;
		 self->aks.x+=50;
		 self->acrs.x+=150;
		 self->shotys.x+=150;
		 if(self->currentweapon==0)self->weaponframe=0;
		 if(self->currentweapon==1)
		  {
			  if(self->swordlvl==0)self->weaponframe=8;
			  if(self->swordlvl==1)self->weaponframe=0;
			  if(self->swordlvl==2)self->weaponframe=4;
		  }
		 if(self->currentweapon==2)self->weaponframe=0;
		 if(self->currentweapon==3)self->weaponframe=0;
		 if(self->currentweapon==4)self->weaponframe=0;
		 if(self->currentweapon==5)self->weaponframe=0;
		 if(self->currentweapon==6)self->weaponframe=0;
		 self->right=1;
	  }
		if(UpdateEntityPosition(self,0)!=3)self->s.y+=20;
		self->right=1;
		if(self->jump==1){
			self->v.y=-30;
			self->jump=0;
			t = self->accel * 3;
			self->v.x+=t;
		}
		if(self->jump==0)
		{
			self->s.y-=4;
		}
		if((self->frame>=24)||(self->frame<=20))self->frame=20;
		self->frame++;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,-1.4,1.4,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovDownRight)
    {     
		if(self->right==0)
	  {
		 self->aimdir=F_East;
		 self->fs.x+=150;
		 self->gs.x+=150;
		 self->ss.x+=150;
		 self->ds.x+=50;
		 self->aks.x+=50;
		 self->acrs.x+=150;
		 self->shotys.x+=150;
		 if(self->currentweapon==0)self->weaponframe=0;
		 if(self->currentweapon==1)
		  {
			  if(self->swordlvl==0)self->weaponframe=8;
			  if(self->swordlvl==1)self->weaponframe=0;
			  if(self->swordlvl==2)self->weaponframe=4;
		  }
		 if(self->currentweapon==2)self->weaponframe=0;
		 if(self->currentweapon==3)self->weaponframe=0;
		 if(self->currentweapon==4)self->weaponframe=0;
		 if(self->currentweapon==5)self->weaponframe=0;
		 if(self->currentweapon==6)self->weaponframe=0;
		 self->right=1;
	  }
	  if(UpdateEntityPosition(self,0)!=1)self->s.y+=20;
	  self->right=1;
	  self->v.x += self->accel;
      self->movespeed = VectorLength(self->v.x,self->v.y);
	  self->grounded = 0;
      if(self->movespeed > self->maxspeed)
      {
        self->v.y = 0;
        self->v.x = self->maxspeed;
        self->movespeed = self->maxspeed;
      }	
	  if((self->frame>=19)||(self->frame<=15))self->frame=15;
	  self->frame++;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,-1.4,-1.4,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovDownLeft)
    {     
	  if(self->right==1)
	  {
		  self->aimdir=F_West;
		  self->fs.x-=150;
		  self->gs.x-=150;
		  self->ss.x-=150;
		  self->ds.x-=50;
		  self->aks.x-=50;
		  self->acrs.x-=150;
		  self->shotys.x-=150;
		  if(self->currentweapon==0)self->weaponframe=3;
		  	  if(self->currentweapon==1)
		  {
			  if(self->swordlvl==0)self->weaponframe=10;
			  if(self->swordlvl==1)self->weaponframe=2;
			  if(self->swordlvl==2)self->weaponframe=6;
		  }
		  if(self->currentweapon==2)self->weaponframe=4;
		  if(self->currentweapon==3)self->weaponframe=4;
		  if(self->currentweapon==4)self->weaponframe=2;
		  if(self->currentweapon==5)self->weaponframe=2;
		  if(self->currentweapon==6)self->weaponframe=2;
		  self->right=0;
	  }
	  if(UpdateEntityPosition(self,0)!=1)self->s.y+=20;
	  self->right=0;
	  self->v.x -= self->accel;
      self->movespeed = VectorLength(self->v.x,self->v.y);
      self->grounded = 0;
      if(self->movespeed > self->maxspeed)
      {
        self->v.y = 0;
        self->v.x = self->maxspeed * -1;
        self->movespeed = self->maxspeed;
      } 
	  if((self->frame>=14) ||(self->frame<=10))self->frame=14; 
	  self->frame--;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,1.4,-1.4,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovUp)
    {    
		if(self->jump==1){
			self->v.y=-25;
			self->jump=0;
		}
		if(self->jump==0)
		{
			self->s.y-=4;
		}
		if(self->right==1 && self->frame!=1)self->frame=0;
		if(self->right==0 && self->frame!=9)self->frame=9;	
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,0,2,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovDown)
    {      
		if(UpdateEntityPosition(self,0)!=1)self->s.y+=40;
		self->grounded = 0;  
		if(self->right==1)self->frame=15;
		if(self->right==0)self->frame=14;
		self->v.x=0;
		self->v.y=0;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,0,-1,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovLeft)
    {      
      //int y;
	  if(self->right==1)
	  {
		  self->fs.x-=150;
		  self->gs.x-=150;
		  self->ss.x-=150;
		  self->ds.x-=50;
		  self->aks.x-=50;
		  self->acrs.x-=150;
		  self->shotys.x-=150;
		  self->aimdir=F_West;
		  if(self->currentweapon==0)self->weaponframe=3;
		  if(self->currentweapon==1)
		  {
			  if(self->swordlvl==0)self->weaponframe=10;
			  if(self->swordlvl==1)self->weaponframe=2;
			  if(self->swordlvl==2)self->weaponframe=6;
		  }
		  if(self->currentweapon==2)self->weaponframe=22;
		  if(self->currentweapon==3)self->weaponframe=4;
		  if(self->currentweapon==4)self->weaponframe=2;
		  if(self->currentweapon==5)self->weaponframe=2;
		  if(self->currentweapon==6)self->weaponframe=2;
		  self->right=0;
	  }
	  if(UpdateEntityPosition(self,0)!=1)self->s.y+=20;
	  self->right=0;
	  self->v.x -= self->accel;
      self->movespeed = VectorLength(self->v.x,self->v.y);
      self->grounded = 0;
      if(self->movespeed > self->maxspeed)
      {
        self->v.y = 0;
        self->v.x = self->maxspeed * -1;
        self->movespeed = self->maxspeed;
      }
	  if((self->frame >= 9)||(self->frame <= 5))self->frame = 9;
		self->frame--;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,2,0,self->movespeed,self->movespeed * 4);
    }
    else if(PlayerCommands[0] == PI_MovRight)
    {      
	  if(self->right==0)
	  {
		  self->aimdir=F_East;
		 self->fs.x+=150;
		 self->gs.x+=150;
		 self->ss.x+=150;
		 self->ds.x+=50;
		 self->aks.x+=50;
		 self->acrs.x+=150;
		 self->shotys.x+=150;
		 if(self->currentweapon==0)self->weaponframe=0;
		  if(self->currentweapon==1)
		  {
			  if(self->swordlvl==0)self->weaponframe=8;
			  if(self->swordlvl==1)self->weaponframe=0;
			  if(self->swordlvl==2)self->weaponframe=4;
		  }
		 if(self->currentweapon==2)self->weaponframe=0;
		 if(self->currentweapon==3)self->weaponframe=0;
		 if(self->currentweapon==4)self->weaponframe=0;
		 if(self->currentweapon==5)self->weaponframe=0;
		 if(self->currentweapon==6)self->weaponframe=0;
		 self->right=1;  
	  }
	  if(UpdateEntityPosition(self,0)!=1)self->s.y+=20;
	  self->right=1;
	  self->v.x += self->accel;
      self->movespeed = VectorLength(self->v.x,self->v.y);
      self->grounded = 0;
      if(self->movespeed > self->maxspeed)
      {
        self->v.y = 0;
        self->v.x = self->maxspeed;
        self->movespeed = self->maxspeed;
      }
	  if(self->frame >=4 )self->frame = 0;	
		self->frame++;
      //SpawnThrust(IndexColor(self->Color),self->s.x + 24,self->s.y + 24,-2,0,self->movespeed,self->movespeed * 4);
    }

    switch(self->state)
    {
      case ST_IDLE: 
        /*
        else if(keys[SDLK_r])
        {
          SpawnOrbitMine(self,self->s.x + self->origin.x,self->s.y - 5,20,10,50,0,self->Color,self->Unit_Type);
          self->frame = 12;
          self->state = ST_ATTACK1;
          self->aimdir = F_North;
          self->Ls.x = -5;
          self->Ls.y = -26;
          self->Cooldown = 3;                    
        }*/
		if(self->reload==1)
		{
			if(PlayerCommands[1] == PI_FireUpLeft)
			{
				if(self->right==0){
					printf("fire up left");
					if(self->swordlvl==1)self->ss.y-=150;
					if(self->swordlvl==0)self->ss.y-=300;
					self->swordlvl=2;
					if(self->currentweapon==0)self->weaponframe=4;
					if(self->currentweapon==1)self->weaponframe=6;
					if(self->currentweapon==2)self->weaponframe=6;
					if(self->currentweapon==3)self->weaponframe=6;
					if(self->currentweapon==4)self->weaponframe=6;
					if(self->currentweapon==5)self->weaponframe=4;
					if(self->currentweapon==6)self->weaponframe=4;
					self->aimdir = F_NW;
				}
			}
			else if(PlayerCommands[1] == PI_FireUp)
			{
				self->aimdir = F_North;
				printf("fire up");
				if(self->currentweapon==0)//fists aim direction control
				{
					if(self->right==0)self->weaponframe=17;
					if(self->right==1)self->weaponframe=12;
				}
				if(self->currentweapon==2)//fists aim direction control
				{
					if(self->right==0)self->weaponframe=4;
					if(self->right==1)self->weaponframe=24;
				}
				if(self->currentweapon==3)//fists aim direction control
				{
					if(self->right==0)self->weaponframe=14;
					if(self->right==1)self->weaponframe=12;
				}
				if(self->currentweapon==4)//fists aim direction control
				{	
					if(self->right==0)self->weaponframe=16;
					if(self->right==1)self->weaponframe=18;
				}
				if(self->currentweapon==5)//fists aim direction control
				{
					if(self->right==0)self->weaponframe=12;
					if(self->right==1)self->weaponframe=14;
				}
				if(self->currentweapon==6)//fists aim direction control
				{
					if(self->right==0)self->weaponframe=14;
					if(self->right==1)self->weaponframe=12;
				}
			}    
			else if(PlayerCommands[1] == PI_FireUpRight)
			{
				if(self->right==1)
				{
					printf("fire up right");
					if(self->swordlvl==1)self->ss.y-=150;
					if(self->swordlvl==0)self->ss.y-=300;
					self->swordlvl=2;
					if(self->currentweapon==0)self->weaponframe=7;
					if(self->currentweapon==1)self->weaponframe=4;
					if(self->currentweapon==2)self->weaponframe=20;
					if(self->currentweapon==3)self->weaponframe=2;
					if(self->currentweapon==4)self->weaponframe=4;
					if(self->currentweapon==5)self->weaponframe=6;
					if(self->currentweapon==6)self->weaponframe=6;
					self->aimdir = F_NE;
				}
			}    
			else if(PlayerCommands[1] == PI_FireLeft)
			{
				if(self->right==0)
				{
					if(self->swordlvl==2)self->ss.y+=150;
					if(self->swordlvl==0)self->ss.y-=150;
					self->swordlvl=1;
					if(self->currentweapon==0)self->weaponframe=3;
					if(self->currentweapon==1)self->weaponframe=2;
					if(self->currentweapon==2)self->weaponframe=22;
					if(self->currentweapon==3)self->weaponframe=4;
					if(self->currentweapon==4)self->weaponframe=2;
					if(self->currentweapon==5)self->weaponframe=2;
					if(self->currentweapon==6)self->weaponframe=2;
					printf("fire left");
					self->aimdir = F_West;
				}
			}    
			else if(PlayerCommands[1] == PI_FireRight)
			{
				if(self->right==1)
				{
					if(self->swordlvl==2)self->ss.y+=150;
					if(self->swordlvl==0)self->ss.y-=150;
					self->swordlvl=1;
					if(self->currentweapon==0)self->weaponframe=0;
					if(self->currentweapon==1)self->weaponframe=0;
					if(self->currentweapon==2)self->weaponframe=0;
					if(self->currentweapon==3)self->weaponframe=0;
					if(self->currentweapon==4)self->weaponframe=0;
					if(self->currentweapon==5)self->weaponframe=0;	
					if(self->currentweapon==6)self->weaponframe=0;
					printf("fire right");			
					self->aimdir = F_East;
				}
			}    
			else if(PlayerCommands[1] == PI_FireDownLeft)
			{
				if(self->right==0)
				{
					if(self->swordlvl==1)self->ss.y+=150;
					if(self->swordlvl==2)self->ss.y+=300;
					self->swordlvl=0;
					if(self->currentweapon==0)self->weaponframe=11;
					if(self->currentweapon==1)self->weaponframe=10;
					if(self->currentweapon==2)self->weaponframe=26;
					if(self->currentweapon==3)self->weaponframe=10;
					if(self->currentweapon==4)self->weaponframe=8;
					if(self->currentweapon==5)self->weaponframe=10;
					if(self->currentweapon==6)self->weaponframe=10;
					printf("fire down left");	
					self->aimdir = F_SW;
				}
			}    
			else if(PlayerCommands[1] == PI_FireDown)
			{
				if(self->currentweapon==0)
				{
					if(self->right==0)self->weaponframe=19;
					if(self->right==1)self->weaponframe=14;
				}
				if(self->currentweapon==2)
				{
					if(self->right==0)self->weaponframe=28;
					if(self->right==1)self->weaponframe=8;
				}
				if(self->currentweapon==3)
				{
					if(self->right==0)self->weaponframe=18;
					if(self->right==1)self->weaponframe=16;
				}
				if(self->currentweapon==4)
				{
					if(self->right==0)self->weaponframe=14;
					if(self->right==1)self->weaponframe=12;
				}
				if(self->currentweapon==5)
				{
					if(self->right==0)self->weaponframe=16;
					if(self->right==1)self->weaponframe=18;
				}
				if(self->currentweapon==6)
				{
					if(self->right==0)self->weaponframe=18;
					if(self->right==1)self->weaponframe=16;
				}
				printf("fire down ");	
				self->aimdir = F_South; 
			}    
			else if(PlayerCommands[1] == PI_FireDownRight)
			{
				if(self->right==1)
				{
					if(self->swordlvl==1)self->ss.y+=150;
					if(self->swordlvl==2)self->ss.y+=300;
					self->swordlvl=0;
					if(self->currentweapon==0)self->weaponframe=8;
					if(self->currentweapon==1)self->weaponframe=8;
					if(self->currentweapon==2)self->weaponframe=2;
					if(self->currentweapon==3)self->weaponframe=8;
					if(self->currentweapon==4)self->weaponframe=10;
					if(self->currentweapon==5)self->weaponframe=8;
					if(self->currentweapon==6)self->weaponframe=8;
					printf("fire down right");
					self->aimdir = F_SE;
				}
			}
		}
        
        break;
      case ST_ATTACK:
		  printf("shoot stuff");
		  FireWeapon(self,self->aimdir);
		  break;
    }
  }
}

/*updates the entity's postition and handles animation*/

void UpdatePlayer(Entity *self)
{
  int Goframe = 0; 
  char text[60];
  if(self->currentweapon==2){
	  sprintf(text,"%i / %i",self->gclip,self->total9mm);
	  DrawText(text,screen,50,20,IndexColor(self->Color),F_Large);
  }
  if(self->currentweapon==3){
	  sprintf(text,"%i / %i",self->dclip,self->total50ae);
	  DrawText(text,screen,50,20,IndexColor(self->Color),F_Large);
  }
  if(self->currentweapon==4){
	  sprintf(text,"%i / %i",self->akmag,self->total762);
	  DrawText(text,screen,50,20,IndexColor(self->Color),F_Large);
  }
  if(self->currentweapon==5){
	  sprintf(text,"%i / %i",self->acrmag,self->total556);
	  DrawText(text,screen,50,20,IndexColor(self->Color),F_Large);
  }
  if(self->currentweapon==6){
	  sprintf(text,"%i / %i",self->shells,self->total12ga);
	  DrawText(text,screen,50,20,IndexColor(self->Color),F_Large);
  }
  UpdateEntityPosition(self,0);
  if(self->jump==0)
  {
	  self->grounded=0;
  }
  if(UpdateEntityPosition(self,0)==1)
  {
	  self->jump=1;
	  self->v.x=0;
  }
  if(self->grounded == 0)
  {
    if(self->v.y < 20);
    self->v.y += 5;     /*gravity at work*/
  }
  //if(VectorLength(self->v.y,self->v.x)==0)self->jump=1;
  ApplyFriction(self,0.05);
  if(self->state == ST_DIE)
  {
    self->fcount = 10;
    self->state = ST_DEAD;
    return;
  }
  else if(self->state == ST_DEAD)
  {
    self->fcount--;
    if(self->fcount <= 0)
    {
      FreeEntity(self);
      exit(0);
    }
    //ExplodingParticle(self->s.x + self->origin.x,self->s.y + self->origin.y,crandom(),crandom());
    return;
  }
  //else if(self->Cooldown > 0)self->Cooldown--;/*weapons cooloff*/
  else
  {
    self->state = ST_IDLE;
  }
  if(self->KillCount > 0)
  {
    self->XP += (self->KillCount * 10) * ((self->KillCount>>1) + 1);
    self->KillCount = 0;
  }
  if(self->fcount <= 0)
  {
    Goframe = 1;
    self->fcount = self->frate;
  }
  else
  {
    self->fcount--;
  }
  if(!Goframe)return;
  else GetFace(self);/*check out movement vector to see what direction we are facing.*/
  /*only frame animations should take place next*/
  /*switch(self->face)
  {
    case F_NULL:
      self->frame = 0;
	  self->weaponframe=0;
    break;
    //case F_North:
      /*self->frame++;
      if((self->legframe > 9)||(self->legframe < 7))self->legframe = 7;
    break;
    case F_South:
      //self->frame++;
      //if((self->frame > 3)||(self->frame < 1))self->frame = 1;
    break;
    case F_SW:
		/*if((self->frame>=14) ||(self->frame<=10))self->frame=14; 
		self->frame--;
		break;
	case F_NW:*
    case F_West:
		//if((self->frame >= 9)||(self->frame <= 5))self->frame = 9;
		//self->frame--;
   //   if(self->frame >=4 )self->frame = 0;
		self->totaloffset=-50;
		break;
    case F_SE:
    case F_NE:
    case F_East:
		if(self->frame >=4 )self->frame = 0;	
		self->frame++;
		break; 
  }*/
  if(self->state == ST_ATTACK)
  {
	//self->weaponframe++;
    switch(self->aimdir)
    {
      case F_South:
		  
        if(self->frame > 2)self->frame = 0;
        break;
      case F_SW:
        if((self->frame > 5)||(self->frame < 3))self->frame = 3;
        break;
      case F_West:
		  printf("shoot west");
        if((self->weaponframe < 0)||(self->weaponframe > 1))self->weaponframe = 0;
        break;
      case F_NW:
        if((self->frame > 11)||(self->frame < 9))self->frame = 9;
        break;
      case F_North:
        if((self->frame > 14)||(self->frame < 12))self->frame = 12;
        break;
      case F_NE:
        if((self->frame > 17)||(self->frame < 15))self->frame = 15;
        break;
      case F_East:
		   printf("shoot east");
        if((self->frame > 20)||(self->frame < 18))self->frame = 18;
        break;
      case F_SE:
        if((self->frame > 23)||(self->frame < 21))self->frame = 21;
        break;
      default:
        self->frame = 0;
    }
  }
}

int CheckCommand(int i)
{
  Uint8 *keys = SDL_GetKeyState(NULL);
  if(((JoyButtons[i][0] == JT_Axis)&&(SDL_JoystickGetAxis(joy, JoyButtons[i][1]) > Wiggle))
       ||((JoyButtons[i][0] == JT_Inverse)&&(SDL_JoystickGetAxis(joy, JoyButtons[i][1]) < AWiggle))
       ||((JoyButtons[i][0] == JT_Button)&&(SDL_JoystickGetButton(joy, JoyButtons[i][1])))
       ||(keys[KeyButtons[i]]))
    return 1;
  return 0;
}

void UpdateInput()
{
  int i;
  for(i = 0;i < 3;i++)PlayerCommands[i] = -1;
  
  i = PI_MovDown;
  if(CheckCommand(i))
    PlayerCommands[0] = i;
  
  i = PI_MovUp;
  if(CheckCommand(i))
    PlayerCommands[0] = i;
    
  i = PI_MovLeft;
  if(CheckCommand(i))
  {
    if(PlayerCommands[0] == PI_MovUp)
    {
      PlayerCommands[0] = PI_MovUpLeft; 
    }
    else if(PlayerCommands[0] == PI_MovDown)
    {
      PlayerCommands[0] = PI_MovDownLeft; 
    }
    else PlayerCommands[0] = i;
  }
    
  i = PI_MovRight;
  if(CheckCommand(i))
  {
    if(PlayerCommands[0] == PI_MovUp)
    {
      PlayerCommands[0] = PI_MovUpRight; 
    }
    else if(PlayerCommands[0] == PI_MovDown)
    {
      PlayerCommands[0] = PI_MovDownRight; 
    }
    else PlayerCommands[0] = i;
  }
  
  
  if(CheckCommand(PI_FireDown))
    PlayerCommands[1] = PI_FireDown;

  if(CheckCommand(PI_FireUp))
    PlayerCommands[1] = PI_FireUp;
  
  i = PI_FireLeft;
  if(CheckCommand(i))
  {
    if(PlayerCommands[1] == PI_FireUp)
    {
      PlayerCommands[1] = PI_FireUpLeft; 
    }
    else if(PlayerCommands[1] == PI_FireDown)
    {
      PlayerCommands[1] = PI_FireDownLeft; 
    }
    else PlayerCommands[1] = i;
  }

  i = PI_FireRight;
  if(CheckCommand(i))
  {
    if(PlayerCommands[1] == PI_FireUp)
    {
      PlayerCommands[1] = PI_FireUpRight; 
    }
    else if(PlayerCommands[1] == PI_FireDown)
    {
      PlayerCommands[1] = PI_FireDownRight; 
    }
    else PlayerCommands[1] = i;
  }
  
  if(CheckCommand(PI_Fire))
    PlayerCommands[1] = PI_Fire;  
  
  if(CheckCommand(PI_NextWeapon))
    PlayerCommands[1] = PI_NextWeapon;  
  
  if(CheckCommand(PI_Reload))
    PlayerCommands[1] = PI_Reload;

  if(CheckCommand(PI_PreviousWeapon))
    PlayerCommands[1] = PI_PreviousWeapon;
  
  if(CheckCommand(PI_Pause))
    PlayerMenu(ThePlayer);
  
}


void DefaultConfig()
{
  
  /*PI_MovDown,PI_MovDownLeft,PI_MovLeft,PI_MovUpLeft,PI_MovUp,PI_MovUpRight,PI_MovRight,PI_MovDownRight,
  PI_FireDown,PI_FireDownLeft,PI_FireLeft,PI_FireUpLeft,PI_FireUp,PI_FireUpRight,PI_FireRight,PI_FireDownRight,
  PI_FireUltimate,PI_NextWeapon,PI_PreviousWeapon,PI_Pause*/
	//0 is a, 1 is b, 2 is x, 3 is y, 4 is left bumper,5 is right bumper, 6is back button,7 is start,8 is L3, 9 is R3
  JoyButtons[PI_MovDown][0] = JT_Axis;
  JoyButtons[PI_MovDown][1] = 1;
  JoyButtons[PI_MovLeft][0] = JT_Inverse;
  JoyButtons[PI_MovLeft][1] = 0;
  JoyButtons[PI_MovUp][0] = JT_Inverse;
  JoyButtons[PI_MovUp][1] = 1;
  JoyButtons[PI_MovRight][0] = JT_Axis;
  JoyButtons[PI_MovRight][1] = 0;
  JoyButtons[PI_FireDown][0] = JT_Axis;
  JoyButtons[PI_FireDown][1] = 3;  
  JoyButtons[PI_FireLeft][0] = JT_Inverse;
  JoyButtons[PI_FireLeft][1] = 4;  
  JoyButtons[PI_FireUp][0] = JT_Inverse;
  JoyButtons[PI_FireUp][1] = 3;  
  JoyButtons[PI_FireRight][0] = JT_Axis;
  JoyButtons[PI_FireRight][1] = 4;  
  JoyButtons[PI_Fire][0] = JT_Button;
  JoyButtons[PI_Fire][1] = 5;
  JoyButtons[PI_Reload][0] = JT_Button;
  JoyButtons[PI_Reload][1] = 2;
  JoyButtons[PI_NextWeapon][0] = JT_Button;
  JoyButtons[PI_NextWeapon][1] = 3;  
  JoyButtons[PI_PreviousWeapon][0] = JT_Button;
  JoyButtons[PI_PreviousWeapon][1] = 4;  
  JoyButtons[PI_Pause][0] = JT_Button;
  JoyButtons[PI_Pause][1] = 9;  
  KeyButtons[PI_MovDown] = SDLK_s;
  KeyButtons[PI_MovLeft] = SDLK_a;
  KeyButtons[PI_MovUp] = SDLK_w;
  KeyButtons[PI_MovRight] = SDLK_d;
  KeyButtons[PI_FireDown] = SDLK_KP2;
  KeyButtons[PI_FireDownLeft] = SDLK_KP1;
  KeyButtons[PI_FireLeft] = SDLK_KP4;
  KeyButtons[PI_FireUpLeft] = SDLK_KP7;
  KeyButtons[PI_FireUp] = SDLK_KP8;
  KeyButtons[PI_FireUpRight] = SDLK_KP9;
  KeyButtons[PI_FireRight] = SDLK_KP6;
  KeyButtons[PI_FireDownRight] = SDLK_KP3;
  KeyButtons[PI_NextWeapon] = SDLK_f;
  KeyButtons[PI_PreviousWeapon] = SDLK_g;
  KeyButtons[PI_Pause] = SDLK_ESCAPE;
  //KeyButtons[PI_FireUltimate] = SDLK_KP5;
}

void LoadKeyConfig()
{
  FILE *file;
  file = fopen("saves/config.cfg","r");
  if(file == NULL)
  {
    fprintf(stderr,"unable to load key configuration, falling back on default\n");
    DefaultConfig();
    SaveKeyConfig();
    return;
  }
  if(fread(KeyButtons, sizeof(KeyButtons), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to write config data to file: %s",SDL_GetError());
    return;
  }
  if(fread(JoyButtons, sizeof(JoyButtons), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to write config data to file: %s",SDL_GetError());
    return;
  }
  fclose(file);
}


void SaveKeyConfig()
{
  FILE *file;
  file = fopen("saves/config.cfg","w");
  if(file == NULL)
  {
    fprintf(stderr,"unable to save key configuration, error!\n");
    return;
  }
  if(fwrite(KeyButtons, sizeof(KeyButtons), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to write config data to file: %s",SDL_GetError());
    return;
  }
  if(fwrite(JoyButtons, sizeof(JoyButtons), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to write config data to file: %s",SDL_GetError());
    return;
  }
  fclose(file);
}

void CloseHUD()
{
  if(titlebar != NULL)
  {
    SDL_FreeSurface(titlebar);
    titlebar = NULL;
  }
}

void LoadHUD()
{
  SDL_Surface *temp;
  temp = IMG_Load("images/titlebar.png");
  if(temp == NULL)
  {
    fprintf(stderr,"Unable to load title bar: %s\n",SDL_GetError());
    exit(0);
  }
  titlebar = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  SDL_SetColorKey(titlebar, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(titlebar->format,0,0,0));
  atexit(CloseHUD);
}

void DrawHUD(Entity *self)
{
  SDL_Rect window;
  char text[60];
  window.x = 0;
  window.y = 0;
  if(titlebar == NULL)
  {
    window.w = screen->w;
    window.h = 32;
    SDL_FillRect(screen, &window, IndexColor(DarkBlue));
  }
  else
  {
    SDL_BlitSurface(titlebar,NULL,screen,NULL);
  }
  sprintf(text,"XP: %i",self->XP);
  DrawText(text,screen,5,5,IndexColor(self->Color),F_Medium);
  window.x = screen->w - 120;
  window.w = 100;
  window.y = 5;
  window.h = 16;
  SDL_FillRect(screen, &window, IndexColor(Red));
  if(self->health >= 0)
    window.w = ((float)self->health / (float)self->healthmax) * 100;
  else window.w = 0;
  SDL_FillRect(screen, &window, IndexColor(Green));
}

void FinalOutput()
{
  fprintf(stdout,"Final XP : %i\n",ThePlayer->XP);
  SDL_SaveBMP(screen,"finale.bmp");
}

/*try to open a saved game and if we fail, then */
int GetUnusedSaveIndex()
{
  FILE *file;
  int i = 0;
  char filename[50];
  sprintf(filename,"saves/SAVE%04i.vwg",i);
  file = fopen(filename,"r");
  while(file != NULL)
  {
    i++;
    fclose(file);    
    sprintf(filename,"saves/SAVE%04i.vwg",i);
    file = fopen(filename,"r");    
  }
  fclose(file);    
  return i;
}

void SavePlayer(PlayerStats *player)
{
  FILE *file;
  char filename[50];
  if(player->previoussave != -1)sprintf(filename,"saves/SAVE%04i.vwg",GetUnusedSaveIndex());
  else sprintf(filename,"saves/SAVE%04i.vwg",player->previoussave);
  file = fopen(filename,"wb");
  if(file == NULL)
  {
    fprintf(stderr,"unable to save game!\n");
    return;
  }
  if(fwrite(player, sizeof(PlayerStats), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to write game data to file: %s",SDL_GetError());
    return;
  }
  fclose(file);
}

void LoadPlayer(char filename[40],PlayerStats *player)
{
  FILE *file;
  file = fopen(filename,"rb");
  if(file == NULL)
  {
    fprintf(stderr,"unable to open saved game!\n");
    return;
  }
  if(fread(player, sizeof(PlayerStats), 1, file)!= 1)
  {
    fprintf(stderr,"Unable to read game data from file: %s",SDL_GetError());
    return;
  }
  fclose(file);
}

