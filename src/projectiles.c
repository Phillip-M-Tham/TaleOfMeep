#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "particle.h"
#include "projectiles.h"
#include "space.h"

void UpdateSlash(Entity *self);
void UpdatePunch(Entity *self);
void UpdateBullet(Entity *self);
void UpdateGrenade(Entity *self);
void UpdateRocket(Entity *self);
//void UpdateExplode(Entity *self);
void UpdateMissile(Entity *self);
void MissileThink(Entity *self);
void UpdateLaser(Entity *self);
void UpdateFlame(Entity *self);
void UpdateSmoke(Entity *self);
void UpdateCBomb(Entity *self);
void FadingLaser(Entity *self);
void UpdateMine(Entity *self);
void UpdateBlade(Entity *self);

extern Uint32 NOW;
extern Entity EntityList[];
extern Entity *Enemy;
extern Entity *ThePlayer;
extern Level level;
Entity *Bullet = NULL;

/*this will be a generic spawn function that will set some basic info to save code*/
Entity *SpawnProjectile(int sx,int sy,float angle,float speed,float accel,int damage,int dtype,float kick)
{
  float cosine,sine;
  Entity *newent = NULL;
  newent = NewEntity();
  if(newent == NULL)return NULL;
  cosine = cos(angle);
  sine = sin(angle);
  newent->dtype = dtype;
  newent->kick = kick;
  newent->damage = damage;
  newent->gravityent = 0;
  newent->shown = 1;
  //newent->legstate = -1;    /*needed if we don't have separate legs*/
  newent->s.x = sx;
  newent->s.y = sy;
  newent->v.x = speed * cosine;
  newent->v.y = speed * sine;
  if(accel != 0)
  {
    newent->v.x = speed * cosine;
    newent->v.y = speed * sine;
    newent->accel = accel;
  } 
  newent->movespeed = (int)speed;
  /*binding the entity to the engine should happen here as well*/
  return newent;
}

/*Entity *SpawnExplosion(Entity *owner,int sx, int sy,float vx,float vy,int damage)
{
  Entity *newent;
  newent = NewEntity();
  if(newent == NULL)return NULL;
  strcpy(newent->EntName,"Explosion\0");
  newent->sprite = LoadSprite("images/explode1.png",32,32);
  SDL_SetColorKey(newent->sprite->image, SDL_SRCCOLORKEY , SDL_MapRGB(newent->sprite->image->format, 0,0,0));
  newent->frame = 0;
  newent->Unit_Type = 0;
  newent->owner = owner;
  newent->Color = IndexColor(LightYellow);
  //newent->legstate = -1;    /*needed if we don't have separate legs*/
  /*newent->s.x = sx - 16;
  newent->s.y = sy - 16;
  newent->v.x = vx;
  newent->v.y = vy;
  newent->update = UpdateExplode;
  newent->UpdateRate = 60;
  newent->shown = 1;
  //newent->lifespan = 12;
  return newent;
}*/

    
/*************************************************************
              
                       Any Size bullet

 *************************************************************/


Entity *SpawnBullet(Entity *owner,int sx,int sy,float angle,float speed,int damage,float kick,int size,int color,int UType,int bulletframe)
{
  Entity *newent = NULL;
  newent = SpawnProjectile(sx,sy,angle,speed,0,damage,DT_Physical,kick);
  if(newent == NULL)return NULL;
  newent->maxtraillen = 2;
  switch(size)
  {
    case nine:
      newent->sprite = LoadSwappedSprite("images/15x159mm.png",15,15);
	  //SDL_SetColorKey(newent->sprite->image, SDL_SRCCOLORKEY , SDL_MapRGB(newent->sprite->image->format, 255,255,255));
      newent->origin.x = 3;
      newent->origin.y = 3;
      newent->Boundingbox.x = 8;
      newent->Boundingbox.y = 12;
      newent->Boundingbox.w = 15;
      newent->Boundingbox.h = 15;
	  newent->lifespan=60;
	  newent->update = UpdateBullet;
     // newent->sound[SF_ALERT] = LoadSound("sounds/ric1.wav",SDL_MIX_MAXVOLUME>>4);
      break;
    case fivefivesix:
      newent->sprite = LoadSwappedSprite("images/25x25556.png",25,25);
      newent->origin.x = 4;
      newent->origin.y = 4;
      newent->Boundingbox.x = 1;
      newent->Boundingbox.y = 1;
      newent->Boundingbox.w = 25;
      newent->Boundingbox.h = 25;
	  newent->lifespan=60;
	  newent->update = UpdateBullet;
      //newent->sound[SF_ALERT] = LoadSound("sounds/ric2.wav",SDL_MIX_MAXVOLUME>>4);
      break;
    case sevensixtwo:
      newent->sprite = LoadSwappedSprite("images/25x25762.png",25,25);
      newent->origin.x = 6;
      newent->origin.y = 6;
      newent->Boundingbox.x = 2;
      newent->Boundingbox.y = 2;
      newent->Boundingbox.w = 25;
      newent->Boundingbox.h = 25;
	  newent->lifespan=60;
	  newent->update = UpdateBullet;
   //   newent->sound[SF_ALERT] = LoadSound("sounds/ric3.wav",SDL_MIX_MAXVOLUME>>3);
      break;
    case fifty:
      newent->maxtraillen = 4;
      newent->sprite = LoadSwappedSprite("images/13x1350ae.png",13,13);
      newent->origin.x = 8;
      newent->origin.y = 8;
      newent->Boundingbox.x = 3;
      newent->Boundingbox.y = 3;
      newent->Boundingbox.w = 13;
      newent->Boundingbox.h = 13;
	  newent->lifespan=60;
	  newent->update = UpdateBullet;
      //newent->sound[SF_ALERT] = LoadSound("sounds/xfire.wav",SDL_MIX_MAXVOLUME>>2);
      break;
	case pellets:
      newent->maxtraillen = 4;
      newent->sprite = LoadSwappedSprite("images/5x5pellet.png",5,5);
      newent->origin.x = 2;
      newent->origin.y = 2;
      newent->Boundingbox.x = 3;
      newent->Boundingbox.y = 3;
      newent->Boundingbox.w = 5;
      newent->Boundingbox.h = 5;
	  newent->lifespan=60;
	  newent->update = UpdateBullet;
      //newent->sound[SF_ALERT] = LoadSound("sounds/xfire.wav",SDL_MIX_MAXVOLUME>>2);
      break;
	case fist:
      newent->maxtraillen = 4;
      newent->sprite = LoadSwappedSprite("images/45x45fist.png",45,45);
      newent->origin.x = 2;
      newent->origin.y = 2;
      newent->Boundingbox.x = 3;
      newent->Boundingbox.y = 3;
      newent->Boundingbox.w = 45;
      newent->Boundingbox.h = 45;
	  //newent->lifespan=1;
	  newent->update = UpdatePunch;
      //newent->sound[SF_ALERT] = LoadSound("sounds/xfire.wav",SDL_MIX_MAXVOLUME>>2);
      break;
	case blade:
      newent->maxtraillen = 4;
      newent->sprite = LoadSwappedSprite("images/305x305slash.png",305,305);
      newent->origin.x = 2;
      newent->origin.y = 2;
      newent->Boundingbox.x = 3;
      newent->Boundingbox.y = 3;
      newent->Boundingbox.w = 305;
      newent->Boundingbox.h = 305;
	  newent->update=UpdateSlash;
	  //newent->lifespan=2;
      //newent->sound[SF_ALERT] = LoadSound("sounds/xfire.wav",SDL_MIX_MAXVOLUME>>2);
      break;

   
    //default:
     // newent->sound[SF_ALERT] = LoadSound("sounds/ric1.wav",SDL_MIX_MAXVOLUME>>4);
  }
  //SDL_SetColorKey(newent->sprite->image, SDL_SRCCOLORKEY , SDL_MapRGB(newent->sprite->image->format, 0,0,0));
  newent->frame = bulletframe;
  newent->owner = owner;
  newent->UpdateRate = 30;
  newent->Color = color;
  newent->Unit_Type = UType;
  //newent->lifespan = 60;
  newent->thick = 1;
  newent->trailhead = 0;
  newent->traillen = 0;
  newent->trail[0].x = sx;
  newent->trail[0].y = sy;
  newent->m.x = (sx + newent->origin.x) >> 6;
  newent->m.y = (sy + newent->origin.y) >> 6;
  AddEntToRegion(newent,newent->m.x,newent->m.y);
  return newent;
}


void UpdateBullet(Entity *self)
{
  int i;
  Entity *target = NULL;
  self->lifespan--;
  if(self->lifespan <= 0)
  {
    FreeEntity(self);
    return;  
  }
  if(UpdateEntityPosition(self,0))
  {
    //if(self->sound[SF_ALERT]->sound != NULL)Mix_PlayChannel(Mix_GroupAvailable(2),self->sound[SF_ALERT]->sound,0);
    FreeEntity(self);
    return;  
  }
  if(self->owner==ThePlayer)
  {
	  for(i = 0;i < MAXENTITIES; i++)
	{
		if(EntityList[i].Enemy==1 && EntityList[i].used==1)
		{
			SDL_Rect bbox,bbox2;
			bbox2.x=(int)EntityList[i].s.x;
			bbox2.y=(int)EntityList[i].s.y;
			bbox2.h=(int)EntityList[i].Boundingbox.w;
			bbox2.w=(int)EntityList[i].Boundingbox.h;
			bbox.x=(int)self->s.x;
			bbox.y=(int)self->s.y;
			bbox.w=(int)self->Boundingbox.w;
			bbox.h=(int)self->Boundingbox.h;
			if(Collide(bbox,bbox2)==1)
			{
				target=&EntityList[i];
				printf("YOU HIT ME :)");
				DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
				FreeEntity(self);
			}
			target=NULL;
		}
		
	}
  }
 if(self->owner!=ThePlayer){
		SDL_Rect bbox,bbox2;
		bbox2.x=(int)ThePlayer->s.x;
		bbox2.y=(int)ThePlayer->s.y;
		bbox2.h=(int)ThePlayer->Boundingbox.w;
		bbox2.w=(int)ThePlayer->Boundingbox.h;
		bbox.x=(int)self->s.x;
		bbox.y=(int)self->s.y;
		bbox.w=(int)self->Boundingbox.w;
		bbox.h=(int)self->Boundingbox.h;
		if(Collide(bbox,bbox2)==1)
		{
			printf("YOU HIT THE GIRL :(");
			target=ThePlayer;
			DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
			FreeEntity(self);
		}
		FreeEntity(self);
  }

}

void UpdatePunch(Entity *self)
{
  int i;
  Entity *target = NULL;
  if(self->owner==ThePlayer)
  {
	for(i = 0;i < MAXENTITIES; i++)
	{
		if(EntityList[i].Enemy==1 && EntityList[i].used==1)
		{
			SDL_Rect bbox,bbox2;
			bbox2.x=(int)EntityList[i].s.x;
			bbox2.y=(int)EntityList[i].s.y;
			bbox2.h=(int)EntityList[i].Boundingbox.w;
			bbox2.w=(int)EntityList[i].Boundingbox.h;
			bbox.x=(int)self->s.x;
			bbox.y=(int)self->s.y;
			bbox.w=(int)self->Boundingbox.w;
			bbox.h=(int)self->Boundingbox.h;
			if(Collide(bbox,bbox2)==1)
			{
				target=&EntityList[i];
				printf("YOU HIT ME :)");
				DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
				FreeEntity(self);
			}
			target=NULL;
		}
		
	}
	FreeEntity(self);
  }
  if(self->owner!=ThePlayer){
		SDL_Rect bbox,bbox2;
		bbox2.x=(int)ThePlayer->s.x;
		bbox2.y=(int)ThePlayer->s.y;
		bbox2.h=(int)ThePlayer->Boundingbox.w;
		bbox2.w=(int)ThePlayer->Boundingbox.h;
		bbox.x=(int)self->s.x;
		bbox.y=(int)self->s.y;
		bbox.w=(int)self->Boundingbox.w;
		bbox.h=(int)self->Boundingbox.h;
		if(Collide(bbox,bbox2)==1)
		{
			printf("YOU HIT THE GIRL :(");
			target=ThePlayer;
			DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
			FreeEntity(self);
		}
		FreeEntity(self);
  }
}

void UpdateSlash(Entity *self)
{
  int i;
  Entity *target = NULL;
  if(self->owner==ThePlayer)
  {
	for(i = 0;i < MAXENTITIES; i++)
	{
		if(EntityList[i].Enemy==1 && EntityList[i].used==1)
		{
			SDL_Rect bbox,bbox2;
			bbox2.x=EntityList[i].s.x;
			bbox2.y=EntityList[i].s.y;
			bbox2.h=EntityList[i].Boundingbox.w;
			bbox2.w=EntityList[i].Boundingbox.h;
			bbox.x=self->s.x;
			bbox.y=self->s.y;
			bbox.w=self->Boundingbox.w;
			bbox.h=self->Boundingbox.h;
			if(Collide(bbox,bbox2)==1)
			{
				target=&EntityList[i];
				printf("YOU HIT ME :)");
				DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
				FreeEntity(self);
			}
			target=NULL;
		}
	}
	FreeEntity(self);
  }
}
/*************************************************************
              
                       Basic Laser Projectile

 *************************************************************/

/*Entity *SpawnLaser(Entity *owner,int sx,int sy,float angle,float speed,int damage,float kick,int size, int color,int UType,int gravity)
{
  Entity *newent = NULL;
  newent = SpawnProjectile(sx,sy,angle,speed,0,damage,DT_Energy,kick);
  if(newent == NULL)return NULL;
  strcpy(newent->EntName,"Laser\0");
  newent->sound[SF_ALERT] = LoadSound("sounds/bluehit.wav",SDL_MIX_MAXVOLUME>>3);
  newent->origin.x = 2;
  newent->origin.y = 2;
  newent->owner = owner;
  newent->frame = 0;
  newent->gravityent = gravity;
  newent->update = UpdateLaser;
  newent->UpdateRate = 30;
  newent->Color = color;
  newent->Unit_Type = UType;
  newent->lifespan = 90;
  newent->trailhead = 0;
  newent->traillen = 0;
  newent->trail[0].x = sx;
  newent->trail[0].y = sy;
  newent->maxtraillen = MAXTRAIL - 1;
  newent->Boundingbox.x = 1;
  newent->Boundingbox.y = 1;
  newent->thick = size;
  newent->Boundingbox.w = 2;
  newent->Boundingbox.h = 2;
  newent->m.x = (sx + newent->origin.x) >> 6;
  newent->m.y = (sy + newent->origin.y) >> 6;
  AddEntToRegion(newent,newent->m.x,newent->m.y);
  return newent;
}


void UpdateLaser(Entity *self)
{
  Entity *target = NULL;
  if(UpdateEntityPosition(self,1))Mix_PlayChannel(Mix_GroupAvailable(2),self->sound[SF_ALERT]->sound,0);
  
  if(self->gravityent)self->v.y += 1.5;
  self->lifespan--;
  if(self->lifespan <= 0)
  {
    self->update = FadingLaser;
    self->v.x = 0;
    self->v.y = 0;
    self->a.x = 0;
    self->a.y = 0;
    self->fcount = self->maxtraillen;
    return;
  }
  target = GetTouchingEnt(self);
  if(target != NULL)
  {
    DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
    self->update = FadingLaser;
    self->v.x = 0;
    self->v.y = 0;
    self->a.x = 0;
    self->a.y = 0;
    self->fcount = self->maxtraillen;
  }
}

void FadingLaser(Entity *self)
{
  UpdateEntityPosition(self,0);
  self->fcount--;
  if(self->fcount <= 0)
      FreeEntity(self);
}



/*************************************************************
              
                       Projectile Support Functions

 *************************************************************/
/*void PrecacheProjSounds()
{
  LoadSound("sounds/deadexplode.wav",SDL_MIX_MAXVOLUME>>2);
  LoadSound("sounds/explode.wav",SDL_MIX_MAXVOLUME>>3);
  LoadSound("sounds/bluehit.wav",SDL_MIX_MAXVOLUME>>3);
  LoadSound("sounds/machgf1b.wav",SDL_MIX_MAXVOLUME>>3);
  LoadSound("sounds/machinexplode.wav",SDL_MIX_MAXVOLUME);
  LoadSound("sounds/xfire.wav",SDL_MIX_MAXVOLUME>>2);
  LoadSound("sounds/ric1.wav",SDL_MIX_MAXVOLUME>>4);
  LoadSound("sounds/ric2.wav",SDL_MIX_MAXVOLUME>>3);
  LoadSound("sounds/ric3.wav",SDL_MIX_MAXVOLUME>>2);
  LoadSound("sounds/hgrenb1a.wav",SDL_MIX_MAXVOLUME>>4);
  LoadSound("sounds/grenlb1b.wav",SDL_MIX_MAXVOLUME>>4);
  LoadSound("sounds/bang.wav",SDL_MIX_MAXVOLUME>>3);
}

/*I am not going to calculate the radians based on the 8 basic directions each time,
  These are the numbers and I see no reason not use them.*/
  
Entity *GetTouchingEnt(Entity *self)
{
  Entity *target = NULL;
  /*int i=0;
  SDL_Rect bbox,bbox2;
  bbox.w = self->Boundingbox.w;
  bbox.h = self->Boundingbox.h;
  bbox.x = self->Boundingbox.x + (int)self->s.x;
  bbox.y = self->Boundingbox.y + (int)self->s.y;
  
  for(i = 0;i < MAXENTITIES;i++)
  {
    if(EntityList[i].used == 1)
    {
		if(EntityList[i].Enemy==1)
		{
			printf("You found an enemy");
			bbox2.x=EntityList[i].s.x;
			bbox2.y=EntityList[i].s.y;
			bbox2.h=EntityList[i].Boundingbox.h;
			bbox2.w=EntityList[i].Boundingbox.w;
			if(Collide(bbox,bbox2)==1)
			{
				printf("YOU HIT ME :(");
				//target=EntityList[i].;
			}
		}
    }
  }
  /*target = GetEntByBox(bbox,self->m.x,self->m.y,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x + 1,self->m.y - 1,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x + 1,self->m.y + 1,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x + 1,self->m.y,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x - 1,self->m.y - 1,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x - 1,self->m.y + 1,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x - 1,self->m.y,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x,self->m.y - 1,self,self->Unit_Type);
  if(target != NULL)return target;
  target = GetEntByBox(bbox,self->m.x,self->m.y + 1,self,self->Unit_Type);*/
  if(target != NULL)return target;
  return NULL;
}


float GetRadFromDir(int dir)
{
  switch(dir)
  {
    case F_South:
      return 1.57;
    case F_SW:
      return 2.3561;
    case F_West:
      return 3.14159;
    case F_NW:
      return 3.926;
    case F_North:
      return 4.712;
    case F_NE:
      return 5.4977;
    case F_East:
      return 0;
    case F_SE:
      return  0.7853;
  }
  return 0;
}