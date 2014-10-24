#ifndef _ENTITY_
#define _ENTITY_

#include "graphics.h"
//INSERT audio.h later

#define MAXPATH		20
#define MAXTRAIL	16
#define QMAX		10
#define MAXENTITIES	8192
#define MAXSTATES	10
#define PERSISTENCE	8

#define random()  ((rand () & 0x7fff) / ((float)0x7fff))
#define crandom() (2.0 * (random() - 0.5))

enum ENTTYPES {ET_Temp,ET_WorldBullet,ET_WorldEnt,ET_PlayerBullet,ET_Player};
enum SOUNDTYPE {SF_ALERT,SF_IMPACT,SF_PAIN,SF_DYING};
enum ENTCLASS {EC_NULL,EC_AI,EC_PLAYER,EC_BULLET};
enum DAMAGE_T {DT_Physical, DT_Bullet,DT_Sword,DT_Laser};
enum STATES {ST_IDLE,ST_WALK,ST_DIE,ST_DEAD,ST_WSWITCH, ST_ALERT, ST_ATTACK};
enum FACE {F_South, F_SW, F_West, F_NW, F_North, F_NE,F_East, F_SE,F_NULL};

typedef struct POINT_T
{
	int x,y;
}Point;

typedef struct COORD_T
{
	float x,y;
}Coord;

typedef struct ENTITY_T
{
	int used; //keep track of what entities are free and should NEVER be touched
	Uint32 LastDrawn; //DO NOT TOUCH
	int Player; //references the player controlling the unit, NUMPLAYERS implies it is owned by the world
	int EntClass;
	int Unit_Type; //For hit detection masking
	char EntName[40];//name of the entity
	Sprite *sprite; //the sprite
	Sprite *weapon;//weapon sprite
	struct ENTITY_T *owner; //for bullets spawned by other entites
	struct ENTITY_T *target;//used for many uits, attack target
	void (*think) (struct ENTITY_T *self);//called by the engine to handle input and make decisions
	void(*update) (struct ENTITY_T *self);//update the position and check for collision
	Uint32 NextThink; //used for how oftern the entity thinks
	Uint16 ThinkRate; //incrementing the above
	Uint32 NextUpdate; //used for how often the entity is updated(animations)
	Uint16 UpdateRate; //increment the above
	int totaloffset; //
	int gravityent; //if we are affected by gravity
	int bounce; //if we can bounce off walls
	int grounded; //set to 1 when standing on the ground
	int Color;//index color for bullets and trails
	int shown; //if 1 then it will render on the screen
	int frame;//current frame to render
	int fcount;//used for animation, the loop variable
	int frate;//how often we update frames
	Uint32 framedelay;//amount of delay between frames
	//int legframe;
	int weaponstate;
	int face; //direction we are moving
	int aimdir; //the direction we are shooting at
	int state; //makes each entity a finite state machine
	SDL_Rect Boundingbox; //bounding box for collision detection
	//SDL_Rect Boundingbox2;//bounding box for fist
	SDL_Rect ClipBox; //the bounding box for hit detection
	Coord ws;                 /*offset coordinates to draw weapon at*/
	Coord s; //screen coordinates
	Coord a; //acceleration
	Coord v; //vector values
	Point m; //map coordinate
	Point size; //mirrors the sprite width and height
	Point origin; 
	int movespeed;
	int maxspeed;
	int ordercount;

	/*for drawing trails*/
	Point trail[MAXTRAIL];    /*the last 8 places that we've been: -1 means unused.*/
	int trailhead;            /*the point in the trail list we are acessing next*/
	int traillen;             /*how long our trail is currently*/
	int maxtraillen;          /*the most we will make our trail*/
	int thick;                /*how thick our trail is*/
	int trailtype;            /*enumerated*/

	int sightrange;
	int accel;
	int kick;
	int damage;
	int dtype;

	int switchdelay;
	int KillCount;
	Uint32 XP;
	int currentweapon;
	
	int weaponframe;
	int armormax;
	int armor;
	int health,healthmax;
	int takedamage;
	int lifespan;             /*some entities will expire after a time...hmm drone helpers that expire....*/

	int right;
	int jump;

	int fired;
	//FISTS
	Coord fs;                 //offset for fists
	//Glock
	Coord gs;               //offset for glock
	int reload;
	Coord ss; //offset for sword
	int swordlvl;//how to raise and lower sword properly
	int gclip;//
	int maxgclip;
	int maxacrclip;
	int maxakclip;
	int maxdclip;
	int maxshells;
	Coord ds;//offset for deagle
	Coord aks;//offset for ak
	int dclip;
	int akmag;
	Coord acrs;
	int acrmag;
	Coord shotys;
	int shells;
	int Enemy;
	int total9mm;
	int total12ga;
	int total50ae;
	int total556;
	int total762;
	int ammotype;
	int healthtype;
}Entity;

//startup and clean up
void InitEntityList();
void ClearEntities();

//creation and destruction function
Entity *NewEntity();
void FreeEntity(Entity *ent);

//update functions
void DrawEntity(Entity *ent);
void DrawEntities();
void UpdateEntities();
void ThinkEntities();

void DrawBBoxEntities();//draw only the bounding box of the entity

void DrawEntityTrail(Entity *ent);

//generic entity stuff
int OnScreen(Entity *self);
void DamageRadius(Entity *attacker,Entity *inflictor,int damage,int radius,int dtype,float kick);//dont know if i need
void DamageTarget(Entity *attacker,Entity *inflictor, Entity *defender, int damage, int dtype, float kick, float kickx, float kicky);
Entity *GetClosestEntity(Entity *self, int rx, int ry, int depth);
Entity *GetNextEntByRad(Entity *self, int radius, int rx, int ry, Entity *start, int ETMask);
int MoveUnit(Entity *self);
int GetNextCount(Entity *self);
void VectorScaleTo(int magnitude, float *xdir, float *ydir);
void ScaleVectors(float *vx, float *vy);
int VectorLength(float vx, float vy);
int Collide(SDL_Rect box1, SDL_Rect box2);
int WhatFace(Entity *self);
void GetFace(Entity *self);
void Get16Face(Entity *self);
void ApplyFriction(Entity *self, float friction);
Entity *GetNextRegionEnt(int x, int y, int count);
int UpdateEntityPosition(Entity *self, int bounce);
int DistanceBetween(Entity *self, Entity *target);
int AimAtTarget(Entity *self,Entity *target);
Coord AddVectors(Coord v1, Coord v2);
Coord FastAddVectors(Coord v1, Coord v2);

//collision detection stuff
int TraceHit(float sx, float sy, float vx, float vy, float *fx, float *fy, int *rx, int *ry);
void InitRegionMask(int sizex, int sizey);
void ClearRegionMask();
Entity *GetEntByBox(SDL_Rect bbox,int rx,int ry,Entity *ignore,int ETMask);
Entity *GetNextEntByBox(SDL_Rect bbox, int rx, int ry, Entity *start, int ETMask);
int AddEntToRegion(Entity *ent, int rx, int ry);
void RemoveEntFromRegion(Entity *ent, int rx, int ry);
void DrawBuckets();
void AdjustOrbit(Entity *self);

#endif