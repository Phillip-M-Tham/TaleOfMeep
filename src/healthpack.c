#include <math.h>
#include <stdlib.h>
#include "healthpack.h"

extern Entity *ThePlayer;
Entity *Health=NULL;

void UpdateHealthPack(Entity *self);

Entity *SpawnHealth(int x, int y, int type)
{
	Entity *newent=NULL;
	newent=	NewEntity();
	if(newent == NULL)return NULL;
	newent->s.x=x;
	newent->s.y=y;
	switch(type)
	{
		case 0:
			newent->sprite=LoadSwappedSprite("images/50x51bandaid.png",50,51);
			newent->origin.x = 3;
			newent->origin.y = 3;
			newent->Boundingbox.x=20;
			newent->Boundingbox.y=20;
			newent->Boundingbox.w=50;
			newent->Boundingbox.h=51;
			newent->update = UpdateHealthPack;
			newent->healthtype=0;
			break;
		case 1:
			newent->sprite=LoadSwappedSprite("images/76x68medbox.png",76,68);
			newent->origin.x = 5;
			newent->origin.y = 5;
			newent->Boundingbox.x=25;
			newent->Boundingbox.y=25;
			newent->Boundingbox.w=76;
			newent->Boundingbox.h=68;
			newent->update = UpdateHealthPack;
			newent->healthtype=1;
			break;
		case 2:
			newent->sprite=LoadSwappedSprite("images/36x101dew.png",36,101);
			newent->origin.x = 10;
			newent->origin.y = 10;
			newent->Boundingbox.x=15;
			newent->Boundingbox.y=15;
			newent->Boundingbox.w=36;
			newent->Boundingbox.h=101;
			newent->update = UpdateHealthPack;
			newent->healthtype=2;
			break;
		
	}
	newent->lifespan=900;
	newent->used=1;
	newent->v.x=0;
	newent->v.y=0;
	newent->trail[0].x = x;
	newent->trail[0].y = y;
	newent->m.x = (x + newent->origin.x) >> 6;
	newent->m.y = (y + newent->origin.y) >> 6;
	AddEntToRegion(newent,newent->m.x,newent->m.y);
	Health=newent;
	return newent;
}

void UpdateHealthPack(Entity *self)
{
  UpdateEntityPosition(self,0);
  if(UpdateEntityPosition(self,0)!=1)
  {
	  self->v.y+=5;
  }
  self->lifespan--;
  if(self->lifespan <= 0)
  {
    FreeEntity(self);
    return;  
  }
  if(ThePlayer!=NULL)
  {
	SDL_Rect bbox,bbox2;
	bbox2.x=ThePlayer->s.x;
	bbox2.y=ThePlayer->s.y;
	bbox2.h=ThePlayer->Boundingbox.w;
	bbox2.w=ThePlayer->Boundingbox.h;
	bbox.x=self->s.x;
	bbox.y=self->s.y;
	bbox.w=self->Boundingbox.w;
	bbox.h=self->Boundingbox.h;
	if(Collide(bbox,bbox2)==1){
		printf("YOU GOT Health");
		if(self->health < self->healthmax)
		{
			if(self->healthtype==0)ThePlayer->health+=10;
			if(self->healthtype==1)ThePlayer->health+=25;
			if(self->healthtype==2)ThePlayer->health+=50;
			if(self->health > self->healthmax)
			{
				self->health=self->healthmax;
			}
			FreeEntity(self);
		}				
	}
  }
}