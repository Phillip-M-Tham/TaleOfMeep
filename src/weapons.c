#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "weapons.h"
#include "particle.h"
#include "projectiles.h"
                        /*2Pi*/
#define Sixteenth       0.392699081698724155
#define Twentyfifth     0.251327412287183459
#define Fiftieth        0.125663706143591729
#define Hundredth       0.062831853071795864
#define TwoHundredth    0.031415926535897932
#define ThreeHundredth  0.020943951023931955
#define FourHundredth   0.015707963267948966

//Sound *WeaponSounds[NumWeapons * 2];
extern SDL_Rect Camera;
extern SDL_Surface *screen;
extern Entity *ThePlayer;

void GetSpawnByDir(Entity *self,int *sx,int *sy,int offset,int dir);

void FireM3(Entity *self,Weapon *weapon,int direction);
//void UltimateShotgun(Entity *self,Weapon *weapon);
void FireAk74(Entity *self,Weapon *weapon,int direction);
//void UltimateMachinegun(Entity *self,Weapon *weapon);
void FireACR(Entity *self,Weapon *weapon,int direction);
void FireGlock(Entity *self, Weapon *weapon, int direction);
void FireDesertEagle(Entity *self, Weapon *weapon, int direction);
void Punch(Entity *self, Weapon *weapon, int direction);
void Slash(Entity *self, Weapon *weapon, int direction);
//void UltimateRifle(Entity *self,Weapon *weapon);
//void FireLaser(Entity *self,Weapon *weapon,int direction);
//void UltimateLaser(Entity *self,Weapon *weapon);
/*void FireRocket(Entity *self,Weapon *weapon,int direction);
void UltimateRocket(Entity *self,Weapon *weapon);
void FireMissile(Entity *self,Weapon *weapon,int direction);
void UltimateMissile(Entity *self,Weapon *weapon);
void FireGLauncher(Entity *self,Weapon *weapon,int direction);
void UltimateGLauncher(Entity *self,Weapon *weapon);
void FireCluster(Entity *self,Weapon *weapon,int direction);
void UltimateCluster(Entity *self,Weapon *weapon);
void FireFlameThrower(Entity *self,Weapon *weapon,int direction);
void UltimateFlameThrower(Entity *self,Weapon *weapon);
void FireSword(Entity *self,Weapon *weapon,int direction);*/
//void UltimateSword(Entity *self,Weapon *weapon);

Weapon WeaponList[] = 
{
  {
    "Hands",          /*weapon's name*/
    //8,                 /*how long it will take to cool down between shots*/
    //26,                 /*Ultimate cooldown*/
    8,                  /*how much damage each projectile will deal*/
    0.2,                  /*how much kick the weapon carries*/
    16,                 /*speed*/
    0.2,                /*how much variation can be expeted between each bullet.*/
    6,                  /*volume of attack*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    0,                  /*bounce*/
    fist,             /*some weapons have different sized projectiles*/
    Punch,        /*call this when you fire the weapon*/
    //UltimateShotgun,     /*call this when you fure the ultimate attack*/
    //"sounds/shotty.wav",
    //"sounds/bigshotgun.wav"
  },
  {
    "Sword",          /*weapon's name*/
    //1,                 /*how long it will take to cool down between shots*/
    //16,                 /*Ultimate cooldown*/
    20,                  /*how much damage each projectile will deal*/
    0.15,                  /*how much kick the weapon carries*/
    18,                 /*speed*/
    0.15,                /*how much variation can be expeted between each bullet.*/
    3,                  /*volume of attack*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    0,                  /*bounce*/
    blade,             /*some weapons have different sized projectiles*/
    Slash,        /*call this when you fire the weapon*/
    //UltimateMachinegun,     /*call this when you fure the ultimate attack*/
    //"sounds/machineg.wav",
    //"\0"
  },
  {
    "Glock",          /*weapon's name*/
   // 5,                  /*how long it will take to cool down between shots*/
   // 24,                 /*Ultimate cooldown*/
    18,                  /*how much damage each projectile will deal*/
    0.1,                  /*how much kick the weapon carries*/
    24,                   /*speed*/
    0.01,                /*how much variation can be expeted between each bullet.*/
    1,                  /*volume of attack*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    0,                  /*bounce*/
    nine,             /*some weapons have different sized projectiles*/
    FireGlock,        /*call this when you fire the weapon*/
    //UltimateRifle,     /*call this when you fure the ultimate attack*/
    //"sounds/rifleshot.wav",
    //"\0"
  },
  {
    "Desert Eagle",          /*weapon's name*/
    //4,                  /*how long it will take to cool down between shots*/
    //24,                 /*Ultimate cooldown*/
    12,                  /*how much damage each projectile will deal*/
    0,                  /*how much kick the weapon carries*/
    18,                   /*speed*/
    0,                /*how much variation can be expeted between each bullet.*/
    1,                  /*volume of attack*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    1,                  /*bounce*/
    fifty,             /*some weapons have different sized projectiles*/
    FireDesertEagle,        /*call this when you fire the weapon*/
    //UltimateLaser,     /*call this when you fure the ultimate attack*/
    //"sounds/medatck1.wav",
    //"\0"
  },
  {
    "Ak-74",          /*weapon's name*/
   // 12,                  /*how long it will take to cool down between shots*/
    //30,                 /*Ultimate cooldown*/
    50,                  /*how much damage each projectile will deal*/
    1,                  /*how much kick the weapon carries*/
    150,                   /*speed*/
    0,                /*how much variation can be expeted between each bullet.*/
    1,                  /*volume of attack*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    0,                  /*bounce*/
    sevensixtwo,             /*some weapons have different sized projectiles*/
    FireAk74,        /*call this when you fire the weapon*/
    //UltimateRocket,     /*call this when you fure the ultimate attack*/
    //"sounds/rocketfly.wav",
    //"\0"
  },
  {
    "ACR",          /*weapon's name*/
   // 8,                  /*how long it will take to cool down between shots*/
   // 30,                 /*Ultimate cooldown*/
    25,                  /*how much damage each projectile will deal*/
    1,                  /*how much kick the weapon carries*/
    20,                   /*speed*/
    0,                /*how much variation can be expeted between each bullet.*/
    3,                  /*depth of search fo a target, KEEP IT SMALL*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    0,                  /*bounce*/
    fivefivesix,             /*some weapons have different sized projectiles*/
    FireACR,        /*call this when you fire the weapon*/
    //UltimateMissile,     /*call this when you fure the ultimate attack*/
    //"sounds/rocketfly.wav",
    //"\0"
  },
  {
    "M3",          /*weapon's name*/
    //2,                  /*how long it will take to cool down between shots*/
    //24,                 /*Ultimate cooldown*/
    7,                  /*how much damage each projectile will deal*/
    0.2,                  /*how much kick the weapon carries*/
    12,                   /*speed*/
    0.15,                /*how much variation can be expeted between each bullet.*/
    9,                  /*volume of attack*/
    0,                  /*if the projectiles produced will be affected by gravity*/
    0,                  /*bounce*/
    pellets,             /*some weapons have different sized projectiles*/
    FireM3,        /*call this when you fire the weapon*/
   // UltimateFlameThrower,     /*call this when you fure the ultimate attack*/
   // "sounds/fthrower.wav",
    //"\0"
  }
};


void FireWeapon(Entity *self,int Direction)
{
  if((self->currentweapon >= 0)&&(self->currentweapon < NumWeapons))
  {
    if(WeaponList[self->currentweapon].fireweapon != NULL)
    {
      WeaponList[self->currentweapon].fireweapon(self,&WeaponList[self->currentweapon],Direction);
      //self->Cooldown = WeaponList[self->currentweapon].cooldown;
      //if(WeaponSounds[self->currentweapon *2] != NULL)
      {
       // Mix_PlayChannel(Mix_GroupAvailable(FX_Bullets),WeaponSounds[self->currentweapon *2]->sound,0);
      }
    }
  }
}    

    
/*************************************************************
              
                       M3

 *************************************************************/
void FireM3(Entity *self,Weapon *weapon,int direction)
{
	if(self->shells!=0){
		if(self->aimdir==F_East && self->weaponframe==1)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			//angle = dir + (weapon->spread * crandom());

			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx+=250,sy+=80,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sx-=250;
				sy-=80;
			}
			self->shells--;
		}
		if(self->aimdir==F_West && self->weaponframe==3)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx-=150,sy+=80,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sx+=150;
				sy-=80;
			}
			self->shells--;
		}
		if(self->aimdir==F_NW && self->weaponframe==5){
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx-=90,sy-=50,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy+=50;
				sx+=90;
			}
			self->shells--;
		}
		if(self->aimdir==F_NE && self->weaponframe==7)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx+=200,sy-=50,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy+=50;
				sx-=200;
			}
			self->shells--;
		}
		if(self->aimdir==F_SE && self->weaponframe==9)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx+=180,sy+=200,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy-=200;
				sx-=180;
			}
			self->shells--;
		}
		if(self->aimdir==F_SW && self->weaponframe==11)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx-=80,sy+=200,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy-=200;
				sx+=80;
			}
			self->shells--;
		}
		if(self->aimdir==F_North && self->right==1 && self->weaponframe==13)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx+=155,sy-=60,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy+=60;
				sx-=155;
			}
			self->shells--;
		}
		if(self->aimdir==F_North && self->right==0 && self->weaponframe==15)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx-=35,sy-=60,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy+=60;
				sx+=35;
			}
			self->shells--;
		}
		if(self->aimdir==F_South && self->right==0 && self->weaponframe==19)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx+=10,sy+=220,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy-=220;
				sx-=10;
			}
			self->shells--;
		}
		if(self->aimdir==F_South && self->right==1 && self->weaponframe==17)
		{
			float angle;
			float dir = GetRadFromDir(direction);
			int i;
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			for(i = 0;i < weapon->volume;i++)
			{
				angle = dir + (weapon->spread * crandom());
				SpawnBullet(self,sx+=105,sy+=220,angle,weapon->speed * (fabs(crandom()) + 1) ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
				sy-=220;
				sx-=105;
			}
			self->shells--;
		}
	}
}



/*************************************************************
              
                       Ak-74

 *************************************************************/
void FireAk74(Entity *self,Weapon *weapon,int direction)
{
  if(self->akmag!=0){
		if(self->aimdir==F_East && self->weaponframe==1)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=300,sy+=80,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
			self->akmag--;
		}
		if(self->aimdir==F_West && self->weaponframe==3)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=100,sy+=80,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
			self->akmag--;
		}
		if(self->aimdir==F_NW && self->weaponframe==7){
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=120,sy-=60,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
			self->akmag--;
		}
		if(self->aimdir==F_NE && self->weaponframe==5)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=335,sy-=60,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
			self->akmag--;
		}
		if(self->aimdir==F_SE && self->weaponframe==11)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=335,sy+=275,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
			self->akmag--;
		}
		if(self->aimdir==F_SW && self->weaponframe==9)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=120,sy+=275,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,3);
			self->akmag--;
		}
		if(self->aimdir==F_North && self->right==1 && self->weaponframe==19)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy-=60,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->akmag--;
		}
		if(self->aimdir==F_North && self->right==0 && self->weaponframe==17)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=-10,sy-=60,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->akmag--;
		}
		if(self->aimdir==F_South && self->right==0 && self->weaponframe==15)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=40,sy+=270,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->akmag--;
		}
		if(self->aimdir==F_South && self->right==1 && self->weaponframe==13)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=250,sy+=270,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->akmag--;
		}
	}
}



/*************************************************************
              
                       ACR

 *************************************************************/
void FireACR(Entity *self,Weapon *weapon,int direction)
{
 if(self->acrmag!=0){
		if(self->aimdir==F_East && self->weaponframe==1)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=300,sy+=87,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
			self->acrmag--;
		}
		if(self->aimdir==F_West && self->weaponframe==3)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=200,sy+=87,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
			self->acrmag--;
		}
		if(self->aimdir==F_NW && self->weaponframe==5){
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=210,sy-=80,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
			self->acrmag--;
		}
		if(self->aimdir==F_NE && self->weaponframe==7)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=310,sy-=80,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
			self->acrmag--;
		}
		if(self->aimdir==F_SE && self->weaponframe==9)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=340,sy+=310,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
			self->acrmag--;
		}
		if(self->aimdir==F_SW && self->weaponframe==11)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=220,sy+=310,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,3);
			self->acrmag--;
		}
		if(self->aimdir==F_North && self->right==1 && self->weaponframe==15)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=170,sy-=80,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->acrmag--;
		}
		if(self->aimdir==F_North && self->right==0 && self->weaponframe==13)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=50,sy-=80,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->acrmag--;
		}
		if(self->aimdir==F_South && self->right==0 && self->weaponframe==17)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=30,sy+=300,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->acrmag--;
		}
		if(self->aimdir==F_South && self->right==1 && self->weaponframe==19)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=90,sy+=300,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->acrmag--;
		}
	}
}


/*************************************************************
              
                     Glock

 *************************************************************/
void FireGlock(Entity *self,Weapon *weapon,int direction)
{
	if(self->gclip!=0){
		if(self->aimdir==F_East && self->weaponframe==1)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy+=75,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
			self->gclip--;
		}
		if(self->aimdir==F_West && self->weaponframe==23)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=100,sy+=75,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
			self->gclip--;
		}
		if(self->aimdir==F_NW && self->weaponframe==7){
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
			self->gclip--;
		}
		if(self->aimdir==F_NE && self->weaponframe==21)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
			self->gclip--;
		}
		if(self->aimdir==F_SE && self->weaponframe==3)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
			self->gclip--;
		}
		if(self->aimdir==F_SW && self->weaponframe==27)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
			self->gclip--;
		}
		if(self->aimdir==F_North && self->right==1 && self->weaponframe==25)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=140,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->gclip--;
		}
		if(self->aimdir==F_North && self->right==0 && self->weaponframe==5)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=20,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->gclip--;
		}
		if(self->aimdir==F_South && self->right==0 && self->weaponframe==29)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=60,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->gclip--;
		}
		if(self->aimdir==F_South && self->right==1 && self->weaponframe==9)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=177,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->gclip--;
		}
	}
}



/*************************************************************
              
                       Desert Eagle

 *************************************************************/
void FireDesertEagle(Entity *self,Weapon *weapon,int direction)
{
 if(self->dclip!=0){
		if(self->aimdir==F_East && self->weaponframe==1)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy+=75,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
			self->dclip--;
		}
		if(self->aimdir==F_West && self->weaponframe==5)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=100,sy+=75,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
			self->dclip--;
		}
		if(self->aimdir==F_NW && self->weaponframe==7){
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy-=20,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
			self->dclip--;
		}
		if(self->aimdir==F_NE && self->weaponframe==3)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy-=20,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
			self->dclip--;
		}
		if(self->aimdir==F_SE && self->weaponframe==9)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=200,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
			self->dclip--;
		}
		if(self->aimdir==F_SW && self->weaponframe==11)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,3);
			self->dclip--;
		}
		if(self->aimdir==F_North && self->right==1 && self->weaponframe==13)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=105,sy-=20,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->dclip--;
		}
		if(self->aimdir==F_North && self->right==0 && self->weaponframe==15)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=20,sy-=20,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			self->dclip--;
		}
		if(self->aimdir==F_South && self->right==0 && self->weaponframe==19)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=5,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->dclip--;
		}
		if(self->aimdir==F_South && self->right==1 && self->weaponframe==17)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=125,sy+=200,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			self->dclip--;
		}
	}
}




/*************************************************************
              
                       Basic Sword

 *************************************************************/
void Slash(Entity *self,Weapon *weapon,int direction)
{
  if(self->aimdir==F_East && self->weaponframe==1)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=30,sy-=150,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
		}
		if(self->aimdir==F_West && self->weaponframe==3)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=130,sy-=150,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
		}
		if(self->aimdir==F_NW && self->weaponframe==7){
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=120,sy-=305,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,3);
		}
		if(self->aimdir==F_NE && self->weaponframe==5)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=20,sy-=305,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
		}
		if(self->aimdir==F_SE && self->weaponframe==9)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=30,sy-=10,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
		}
		if(self->aimdir==F_SW && self->weaponframe==11)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=130,sy-=10,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
		}
}


/*************************************************************
              
                       Fists

 *************************************************************/
void Punch(Entity *self,Weapon *weapon,int direction)
{
		if(self->aimdir==F_East && self->weaponframe==1)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=225,sy+=100,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
		}
		if(self->aimdir==F_West && self->weaponframe==2)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy+=100,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
		}
		if(self->aimdir==F_NW && self->weaponframe==5){
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
		}
		if(self->aimdir==F_NE && self->weaponframe==6)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=220,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
		}
		if(self->aimdir==F_SE && self->weaponframe==9)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=220,sy+=190,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
		}
		if(self->aimdir==F_SW && self->weaponframe==10)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx-=80,sy+=190,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,3);
		}
		if(self->aimdir==F_North && self->right==1 && self->weaponframe==13)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=140,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
		}
		if(self->aimdir==F_North && self->right==0 && self->weaponframe==16)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=20,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
		}
		if(self->aimdir==F_South && self->right==0 && self->weaponframe==18)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=5,sy+=195,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
		}
		if(self->aimdir==F_South && self->right==1 && self->weaponframe==15)
		{
			float dir = GetRadFromDir(direction);
			int sx,sy;
			GetSpawnByDir(self,&sx,&sy,0,direction);
			SpawnBullet(self,sx+=145,sy+=195,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
		}
		if(self->Enemy==1)
		{
			if(self->aimdir==F_East)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=225,sy+=100,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,0);
			}
			if(self->aimdir==F_West)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx-=80,sy+=100,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,5);
			}
			if(self->aimdir==F_NW)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx-=80,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,2);
			}
			if(self->aimdir==F_NE)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=220,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,1);
			}
			if(self->aimdir==F_SE)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=220,sy+=190,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,4);
			}
			if(self->aimdir==F_SW)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx-=80,sy+=190,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,3);
			}
			if(self->aimdir==F_North && self->right==1)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=140,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			}
			if(self->aimdir==F_North)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=20,sy,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,6);
			}
			if(self->aimdir==F_South && self->right==0)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=5,sy+=195,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			}
			if(self->aimdir==F_South && self->right==1)
			{
				float dir = GetRadFromDir(direction);
				int sx,sy;
				GetSpawnByDir(self,&sx,&sy,0,direction);
				SpawnBullet(self,sx+=145,sy+=195,dir + crandom() * weapon->spread,weapon->speed ,weapon->damage,weapon->kick, weapon->size, self->Color, self->Unit_Type,7);
			}
		}
}


/*************************************************************
              
                       Other Supported Functions

 *************************************************************/
char *PrintWeaponName(int index)
{
  return WeaponList[index].Name;
}

/*void PrecacheWeaponSounds()                    /*load sounds that the weapons use*/
/*{
  int i;
  for(i = 0;i < NumWeapons;i++)
  {
    if(WeaponList[i].firesound[0] != '\0')
    {
      WeaponSounds[i*2] = LoadSound(WeaponList[i].firesound,MIX_MAX_VOLUME);
    }  
    else WeaponSounds[i*2] = NULL;
    if(WeaponList[i].ultimatesound[0] != '\0')
    {
      WeaponSounds[i*2 + 1] = LoadSound(WeaponList[i].ultimatesound,MIX_MAX_VOLUME);
    }
    else WeaponSounds[i*2 + 1] = NULL;
  }
}

           /*return the index of the weapon in the list by the weapon's name*/
int GetWeaponByName(char name[40])
{
  int i;
  for(i = 0;i < NumWeapons;i++)
  {
    if(strncmp(name,WeaponList[i].Name,40)==0)return i;
  }
  return 0;
}

void GetSpawnByDir(Entity *self,int *sx,int *sy,int offset,int dir)
{
  int angle;
  *sx = (int)self->s.x+ self->origin.x;
  *sy = (int)self->s.y+ self->origin.y;
  angle = (int)(offset * 1.4);
  switch(dir)
  {
    case F_South:
      *sy += offset;
      return;
    case F_SW:
      *sx -= angle;
      *sy += angle;      
      return;
    case F_West:
      *sx -= offset;
      return;
    case F_NW:
      *sx -= angle;
      *sy -= angle;      
      return;
    case F_North:
      *sy -= offset;
      return;
    case F_NE:
      *sx += angle;
      *sy -= angle;      
      return;
    case F_East:
      *sx += offset;
      return;
    case F_SE:
      *sx += angle;
      *sy += angle;      
      return;
  }
}