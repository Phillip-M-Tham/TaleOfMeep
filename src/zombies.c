#include <string.h>
#include <stdlib.h>
#include "particle.h"
#include "weapons.h"
#include "zombies.h"

extern SDL_Surface *screen;
extern Entity *ThePlayer;
Entity *Enemy = NULL;
//int zcount;


void UpdateZombie(Entity *self);
void ZombieThink(Entity *self);

void SpawnZombie(Entity *owner,int x,int y,int subtype,int affiliation)
{
  Entity *Newent = NULL;
  Newent = NewEntity();
  if(Newent == NULL)
  {
    fprintf(stderr,"Unable to generate zombie entity; %s",SDL_GetError());
    return;
  }
  strcpy(Newent->EntName,"Zombie\0");
  Newent->sprite = LoadSprite("images/191x237zombie.png",191,237);
  Newent->owner = owner;
  Newent->update = UpdateZombie;
  Newent->think = ZombieThink;
  Newent->UpdateRate = 45;
  Newent->ThinkRate = 45;
  Newent->Unit_Type = affiliation;
  Newent->frame = 0;
  Newent->fcount = 0;
  Newent->Color = Cyan;
  Newent->frate = 1;
  Newent->state = ST_IDLE;
  Newent->aimdir = F_East;
  Newent->healthmax = 200;
  Newent->health = 200;
  Newent->takedamage = 1;
  Newent->s.x = x;
  Newent->s.y = y;
  Newent->fs.x=x;
  Newent->fs.y=y;
  Newent->zcount=subtype;
  Newent->currentweapon = 0;
  Newent->origin.x = -26;
  Newent->origin.y = -16;
  Newent->v.x = 0;
  Newent->v.y = 0;
  Newent->maxspeed = 10;
  Newent->movespeed = 0;
  Newent->accel = 3;
  Newent->totaloffset = 0;
  Newent->Boundingbox.x = 12;
  Newent->Boundingbox.y = 8;
  Newent->Boundingbox.w = 191;
  Newent->Boundingbox.h = 237;
  Newent->right=1;
  Newent->ordercount=5;
  Newent->m.x = (x + Newent->origin.x) >> 6;
  Newent->m.y = (y + Newent->origin.y) >> 6;
  Newent->Enemy=1;
  Enemy=Newent;
  AddEntToRegion(Newent,Newent->m.x,Newent->m.y);
}

void ZombieThink(Entity *self)
{
  switch(self->state)
  {
    case ST_WALK:
      self->ordercount--;
      if(self->ordercount <= 0)self->state = ST_IDLE;
      break;
    case ST_IDLE:
      if(DistanceBetween(self, ThePlayer) < 150)
      {
        self->state = ST_ALERT;
      }
      else
      {
        self->state = ST_WALK;
        if(self->fcount == 0)
        {
          if(self->right==1){
			self->v.x = 5;
			self->frame++;
			self->face=F_East;
			if(self->frame > 4)self->frame = 0;
			self->fcount=1;
		  }//self->fcount = 1;
		  if(self->right==0){
			self->v.x = -5;
			self->fcount = 0;
			self->face=F_West;
			self->frame--;
			if((self->frame < 5)||(self->frame > 9))self->frame = 9;
			self->fcount=1;
		  }
        }
        self->ordercount = 50;
      }
    break;
    case ST_ALERT:
      if(DistanceBetween(self, ThePlayer) < 90)
      {
		self->v.x++;
		self->aimdir = AimAtTarget(self,ThePlayer);
		self->state = ST_ATTACK;
        return;
      }
      if(self->s.x < ThePlayer->s.x - 32)
      {
        self->v.x = 50;
		self->face=F_East;
		self->right=1;
		self->frame++;
        if(self->frame > 4)self->frame = 0;  
      }
      else if(self->s.x > ThePlayer->s.x +32)
      {
        self->v.x = -50;
		self->face=F_West;
		self->right=0;
		self->frame--;
        if((self->frame < 5)||(self->frame > 9))self->frame = 9;
      }
      else self->v.x = 0;
      if(self->s.y < ThePlayer->s.y - 32)
      {
        
		  self->v.x = 20;
		 // self->v.y = 4;
		  self->frame++;
        if(self->frame > 4)self->frame = 0;
        //self->grounded = 0;
      }
	  else self->v.y=0;
    break;
  }
}

void UpdateZombie(Entity *self)
{
	//if(self->owner==NULL)self->owner=self;
	if(UpdateEntityPosition(self,0)!=1)self->v.y+=20;
	UpdateEntityPosition(self,0);
  if(!self->grounded)
  {
    if(self->v.y < 20)
    self->v.y += 5;
  }
  //if(self->Cooldown > 0)self->Cooldown--;
  switch(self->state)
  {
    case ST_IDLE:
      ApplyFriction(self,0.02);
      self->frame++;
        if(self->frame >= 1)
        {
          self->frame = 0;
        }
      break;
    case ST_ATTACK:
      //ApplyFriction(self,0.02);
	  if(self->right==1){
		  self->frame++;
		  if(self->frame>12 || self->frame<10)self->frame=10;
		  FireWeapon(self,self->aimdir);
	  }
	  if(self->right==0){
		  self->frame--;
		  if(self->frame>15 || self->frame<13)self->frame=15;
		  FireWeapon(self,self->aimdir);
	  }
      self->state = ST_ALERT;
      break;
    
    case ST_ALERT:
      ApplyFriction(self,0.02);
    case ST_WALK:
      GetFace(self);
      switch(self->face)
      {
        case F_North:
        case F_South:
          //self->frame++;
         // if((self->frame < 27)||(self->frame > 29))self->frame = 27;
        //break;
        case F_West:
          if(self->grounded)
          {
            self->frame--;
            if((self->frame < 5)||(self->frame > 9))self->frame = 9;            
            break;
          }
        case F_SW:
        case F_NW:
          //self->frame++;
          //if((self->frame < 24)||(self->frame > 26))self->frame = 24;
          //break;
        case F_East:
          if(self->grounded)
          {
            self->frame++;
            if(self->frame > 4)self->frame = 0;            
            break;
          }
        case F_SE:
        case F_NE:
          /*self->frame++;
          if((self->frame < 30)||(self->frame > 32))self->frame = 30;*/
          break;
      }
      break;
    case ST_DIE:
      self->fcount = 10;
      self->state = ST_DEAD;
      break;
    case ST_DEAD:
      self->fcount--;
      if(self->fcount <= 0)
      {
        FreeEntity(self);
        return;
      }
      //ExplodingParticle(self->s.x + self->origin.x,self->s.y + self->origin.y,crandom(),crandom());
	  self->sprite=LoadSprite("images/191x237deadzombie.png",191,237);
	  if(self->right==0){
		  self->frame=4;
	  }
	 if(self->right==1){
		  self->frame=0;
	  }
	  self->frame++;
	  if(self->frame==1 || self->frame==5){
		  self->frame++;
	  }
	  if(self->frame==2 || self->frame==6){
		  self->frame++;
	  }
    default:
      return;
  }
}

