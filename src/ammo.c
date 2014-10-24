#include <math.h>
#include <stdlib.h>
#include "ammo.h"

extern Entity *ThePlayer;
Entity *Ammo=NULL;

void UpdateAmmoBox(Entity *self);

Entity *SpawnAmmo(int x, int y, int type)
{
	Entity *newent=NULL;
	newent=	NewEntity();
	if(newent == NULL)return NULL;
	newent->s.x=x;
	newent->s.y=y;
	switch(type)
	{
		case 0:
			newent->sprite=LoadSwappedSprite("images/73x51ammo9mm.png",73,51);
			newent->origin.x = 3;
			newent->origin.y = 3;
			newent->Boundingbox.x=20;
			newent->Boundingbox.y=20;
			newent->Boundingbox.w=73;
			newent->Boundingbox.h=51;
			newent->update = UpdateAmmoBox;
			newent->ammotype=0;
			break;
		case 1:
			newent->sprite=LoadSwappedSprite("images/73x51ammo556.png",73,51);
			newent->origin.x = 5;
			newent->origin.y = 5;
			newent->Boundingbox.x=25;
			newent->Boundingbox.y=25;
			newent->Boundingbox.w=73;
			newent->Boundingbox.h=51;
			newent->update = UpdateAmmoBox;
			newent->ammotype=1;
			break;
		case 2:
			newent->sprite=LoadSwappedSprite("images/73x51ammo762.png",73,51);
			newent->origin.x = 10;
			newent->origin.y = 10;
			newent->Boundingbox.x=15;
			newent->Boundingbox.y=15;
			newent->Boundingbox.w=73;
			newent->Boundingbox.h=51;
			newent->update = UpdateAmmoBox;
			newent->ammotype=2;
			break;
		case 3:
			newent->sprite=LoadSwappedSprite("images/73x51ammo50ae.png",73,51);
			newent->origin.x = 20;
			newent->origin.y = 20;
			newent->Boundingbox.x=5;
			newent->Boundingbox.y=5;
			newent->Boundingbox.w=73;
			newent->Boundingbox.h=51;
			newent->update = UpdateAmmoBox;
			newent->ammotype=3;
			break;
		case 4:
			newent->sprite=LoadSwappedSprite("images/73x51ammo12Ga.png",73,51);
			newent->origin.x = 8;
			newent->origin.y = 8;
			newent->Boundingbox.x=20;
			newent->Boundingbox.y=20;
			newent->Boundingbox.w=73;
			newent->Boundingbox.h=51;
			newent->update = UpdateAmmoBox;
			newent->ammotype=4;
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
	Ammo=newent;
	return newent;
}

void UpdateAmmoBox(Entity *self)
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
		printf("YOU GOT AMMO");
		if(self->ammotype==0)
		{
			ThePlayer->total9mm+=150;
		}
		if(self->ammotype==1)
		{
			ThePlayer->total556+=300;
		}
		if(self->ammotype==2)
		{
			ThePlayer->total762+=300;
		}
		if(self->ammotype==3)
		{
			ThePlayer->total50ae+=80;
		}
		if(self->ammotype==4)
		{
			ThePlayer->total12ga+=80;
		}
		//target=Enemy;
		//DamageTarget(self->owner,self,target,self->damage,self->dtype,self->kick,self->v.x,self->v.y);
		FreeEntity(self);
	}
  }
}