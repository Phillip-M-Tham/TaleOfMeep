#include <math.h>
#include <stdlib.h>
#include "armor.h"

extern Entity *ThePlayer;
Entity *Armor=NULL;

void UpdateArmor(Entity *self);

void SpawnArmor(Entity *owner,int x,int y,int subtype,int affiliation)
{
	Entity *newent=NULL;
	newent=	NewEntity();
	if(newent == NULL){
		fprintf(stderr,"Unable to generate armor entity; %s",SDL_GetError());
		return;
	}
	newent->owner=owner;
	newent->s.x=x;
	newent->s.y=y;
	newent->sprite=LoadSwappedSprite("images/81x110armor.png",81,110);
	newent->origin.x = 10;
	newent->origin.y = 10;
	newent->Boundingbox.x=15;
	newent->Boundingbox.y=15;
	newent->Boundingbox.w=81;
	newent->Boundingbox.h=110;
	newent->Unit_Type=affiliation;
	newent->update = UpdateArmor;
	newent->lifespan=900;
	newent->used=1;
	newent->v.x=0;
	newent->v.y=0;
	newent->trail[0].x = x;
	newent->trail[0].y = y;
	newent->m.x = (x + newent->origin.x) >> 6;
	newent->m.y = (y + newent->origin.y) >> 6;
	AddEntToRegion(newent,newent->m.x,newent->m.y);
	Armor=newent;
	//return newent;
}

void UpdateArmor(Entity *self)
{
  UpdateEntityPosition(self,0);
  if(UpdateEntityPosition(self,0)!=1)
  {
	  self->v.y+=5;
  }
  /*self->lifespan--;
  if(self->lifespan <= 0)
  {
    FreeEntity(self);
    return;  
  }*/
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
		printf("YOU GOT Armor");
		if(ThePlayer->armor < ThePlayer->armormax)
		{
			ThePlayer->armor+=100;
			if(ThePlayer->armor > ThePlayer->armormax)
			{
				ThePlayer->armor=ThePlayer->armormax;
			}
			FreeEntity(self);
		}				
	}
  }
}