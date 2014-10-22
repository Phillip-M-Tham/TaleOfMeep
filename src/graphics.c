#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

#define MaxSprites    666

SDL_Surface *screen; /*pointer to the draw buffer*/
SDL_Surface *background;/*pointer to the background image buffer*/
SDL_Surface *bgimage;
SDL_Surface *videobuffer;
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/
Sprite SpriteList[MaxSprites];
Uint32 NOW;  /*this represents the current time for the game loop.  Things move according to time*/

int NumSprites;

/*some data on the video settings that can be useful for a lot of functions*/
Uint32 rmask,gmask,bmask,amask;
ScreenData  S_Data;

void Init_Graphics(int windowed)
{
  Uint32 Vflags = SDL_ANYFORMAT | SDL_SRCALPHA;
    Uint32 HWflag = 0;
    SDL_Surface *temp;
	char *error = NULL;
    S_Data.xres = 1600;
    S_Data.yres = 900;//WHAT IS THE 600 FOR
    if(!windowed)Vflags |= SDL_FULLSCREEN;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
    #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
    #endif
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_DOUBLEBUF) < 0 )
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
    if(SDL_VideoModeOK(1600, 900, 32, Vflags | SDL_HWSURFACE))
    {
        S_Data.xres = 1600;
        S_Data.yres = 900;
        S_Data.depth = 32;
		    fprintf(stderr,"32 bits of depth\n");
        HWflag = SDL_HWSURFACE;
    }
    else if(SDL_VideoModeOK(1600, 900, 16, Vflags | SDL_HWSURFACE))
    {
        S_Data.xres = 1600;
        S_Data.yres = 900;
        S_Data.depth = 16;
     		fprintf(stderr,"16 bits of depth\n");
        HWflag = SDL_HWSURFACE;
    }
    else if(SDL_VideoModeOK(1600, 900, 16, Vflags))
    {
        S_Data.xres = 1600;
        S_Data.yres = 900;
        S_Data.depth = 16;
    		fprintf(stderr,"16 bits of depth\n");
        HWflag = SDL_SWSURFACE;
    }
    else                                                         
    {
        fprintf(stderr, "Unable to Use your crap: %s\n Upgrade \n", SDL_GetError());
		error = SDL_GetError();

        exit(1);
    }
    videobuffer = SDL_SetVideoMode(S_Data.xres, S_Data.yres,S_Data.depth, Vflags);
    if ( videobuffer == NULL )
    {
        fprintf(stderr, "Unable to set 1024x600 video: %s\n", SDL_GetError());
        exit(1);
    }
    temp = SDL_CreateRGBSurface(HWflag, S_Data.xres, S_Data.yres, S_Data.depth,rmask, gmask,bmask,amask);
    if(temp == NULL)
	  {
        fprintf(stderr,"Couldn't initialize background buffer: %s\n", SDL_GetError());
        exit(1);
	  }
    /* Just to make sure that the surface we create is compatible with the screen*/
    screen = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    temp = SDL_CreateRGBSurface(HWflag, 2048, 2048, S_Data.depth,rmask, gmask,bmask,amask);
    if(temp == NULL)
	  {
        fprintf(stderr,"Couldn't initialize background buffer: %s\n", SDL_GetError());
        exit(1);
	  }
    /* Just to make sure that the surface we create is compatible with the screen*/
    background = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_ShowCursor(SDL_DISABLE);
    Camera.x = 0;
    Camera.y = 0;
    Camera.w = screen->w;
    Camera.h = screen->h;
    srand(SDL_GetTicks());
}

void ResetBuffer()
{
	/*Blit BGimage to background and then blit the tile map to that. for paralax*/
	SDL_BlitSurface(bgimage,&Camera,screen,NULL);
	SDL_BlitSurface(background,&Camera,screen,NULL);
}

void NextFrame()
{
	Uint32 Then;
	SDL_BlitSurface(screen,NULL,videobuffer,NULL);
	SDL_Flip(videobuffer);
	FrameDelay(30);
	Then=NOW;
	NOW=SDL_GetTicks();
}

/*
  InitSpriteList is called when the program is first started.
  It just sets everything to zero and sets all pointers to NULL.
  It should never be called again.
*/

void InitSpriteList()
{
	int x;
	NumSprites=0;
	memset(SpriteList,0,sizeof(Sprite) *MaxSprites);
	for(x=0;x<MaxSprites;x++)SpriteList[x].image=NULL;
}

/*Create a sprite from a file, the most common use for it.*/
Sprite *LoadSprite(char *filename, int sizex, int sizey)
{
	int i;
	SDL_Surface *temp;
	/*first search to see if the requested sprite image is alreday loaded*/
	for(i=0;i<NumSprites;i++)
	{
		if(strncmp(filename,SpriteList[i].filename,40)==0){
			SpriteList[i].used++;
			return &SpriteList[i];
		}
	}
	/*makesure we have the room for a new sprite*/
	if(NumSprites +1 >= MaxSprites)
	{
		fprintf(stderr, "Maximum Sprites Reached. \n");
		exit(1);
	}
	 /*if its not already in memory, then load it.*/
	NumSprites++;
	for(i=0;i <=NumSprites;i++)
	{
		if(!SpriteList[i].used)
		{
			break;
		}
	}
	temp = IMG_Load(filename);
	if(temp==NULL)
	{
		fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
		exit(0);
	}
	SpriteList[i].image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/*sets a transparent color for blitting.*/
	SDL_SetColorKey(SpriteList[i].image, SDL_SRCCOLORKEY , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
	/*then copy the given information to the sprite*/
	strncpy(SpriteList[i].filename,filename,20);
    /*now sprites don't have to be 16 frames per line, but most will be.*/
	SpriteList[i].framesperline = 5;
	SpriteList[i].w = sizex;
	SpriteList[i].h = sizey;
	SpriteList[i].used++;
	return &SpriteList[i];
}

Sprite *LoadSwappedSprite(char *filename,int sizex, int sizey)
{
  int i;
  SDL_Surface *temp;
  /*first search to see if the requested sprite image is alreday loaded*/
  for(i = 0; i < NumSprites; i++)
  {
    if((strncmp(filename,SpriteList[i].filename,40)==0)&&(SpriteList[i].used >= 1))
    {
      SpriteList[i].used++;
      return &SpriteList[i];
    }
  }
  /*makesure we have the room for a new sprite*/
  if(NumSprites + 1 >= MaxSprites)
  {
        fprintf(stderr, "Maximum Sprites Reached.\n");
        exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].used)break;
  }
  temp = IMG_Load(filename);
  if(temp == NULL)
  {
        fprintf(stderr, "FAILED TO LOAD A VITAL SPRITE.\n");
        exit(1);
  }
  SpriteList[i].image = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  /*sets a transparent color for blitting.*/
  SDL_SetColorKey(SpriteList[i].image, SDL_SRCCOLORKEY , SDL_MapRGB(SpriteList[i].image->format, 255,255,255));
  //fprintf(stderr,"asked for colors: %d,%d,%d \n",c1,c2,c3);
  //SwapSprite(SpriteList[i].image);
   /*then copy the given information to the sprite*/
  strcpy(SpriteList[i].filename,filename);
      /*now sprites don't have to be 16 frames per line, but most will be.*/
  SpriteList[i].framesperline = 5;
  SpriteList[i].w = sizex;
  SpriteList[i].h = sizey;
  SpriteList[i].used++;
  return &SpriteList[i];
}

/*
 * When we are done with a sprite, lets give the resources back to the system...
 * so we can get them again later.
 */

void FreeSprite(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  sprite->used--;
  if(sprite->used == 0)
  {
  strcpy(sprite->filename,"\0");
     /*just to be anal retentive, check to see if the image is already freed*/
  if(sprite->image != NULL)SDL_FreeSurface(sprite->image);
  sprite->image = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

void CloseSprites()
{ 
  int i;
  for(i = 0;i < MaxSprites;i++)
  {
	/*it shouldn't matter if the sprite is already freed, FreeSprite checks for that*/
    FreeSprite(&SpriteList[i]);
  }
}

void DrawGreySprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame)//What is this for
{
  int i,j;
  int offx,offy;
  Uint8 r,g,b;
  Uint32 pixel;
  Uint32 Key = sprite->image->format->colorkey;
  offx = frame%sprite->framesperline * sprite->w;
  offy = frame/sprite->framesperline * sprite->h;
  if ( SDL_LockSurface(sprite->image) < 0 )
  {
      fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
      exit(1);
  }
  for(j = 0;j < sprite->h;j++)
  {
    for(i = 0;i < sprite->w;i++)
    {
      pixel = getpixel(sprite->image, i + offx ,j + offy);
      if(Key != pixel)
      {
        SDL_GetRGB(pixel, sprite->image->format, &r, &g, &b);
        r = (r + g + b)/3;
        putpixel(surface, sx + i, sy + j, SDL_MapRGB(sprite->image->format, r, r, r));
      }
    }
  }
  SDL_UnlockSurface(sprite->image);
}

void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame)
{
    SDL_Rect src,dest;
    src.x = frame%sprite->framesperline * sprite->w;
    src.y = frame/sprite->framesperline * sprite->h;
    src.w = sprite->w;
    src.h = sprite->h;
    dest.x = sx;
    dest.y = sy;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite->image, &src, surface, &dest);
}

void DrawSpritePixel(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame)
{
    SDL_Rect src,dest;
    src.x = frame%sprite->framesperline * sprite->w + sprite->w/2;
    src.y = frame/sprite->framesperline * sprite->h + sprite->h/2;
    src.w = 1;
    src.h = 1;
    dest.x = sx;
    dest.y = sy;
    dest.w = 1;
    dest.h = 1;
    SDL_BlitSurface(sprite->image, &src, surface, &dest);
}

void FrameDelay(Uint32 delay)
{
    static Uint32 pass = 100;
    Uint32 dif;
    dif = SDL_GetTicks() - pass;
    if(dif < delay)SDL_Delay( delay - dif);
    pass = SDL_GetTicks();
}
/*draws an elipse at the location specified*/
void DrawElipse(int ox,int oy, int radius, Uint32 Color, SDL_Surface *surface)
{
  int r2 = radius * radius;
  double x,y;//was an int
  for(x = radius * -1;x <= radius;x++)
  {
    y = (int) (sqrt(r2 - x*x) * 0.6);
    putpixel(surface, x + ox, oy + y, Color);
    putpixel(surface, x + ox, oy - y, Color);
  }
}

/*draws an rectangle outline at the coordinates of the width and height*/
void DrawRect(int sx,int sy, int sw, int sh, Uint32 Color, SDL_Surface *surface)
{
  SDL_Rect box;
    box.x = sx;
    box.y = sy;
    box.w = sw;
    box.h = 1;                                        
    SDL_FillRect(surface,&box,Color);
    box.y = sy + sh;
    SDL_FillRect(surface,&box,Color);
    box.y = sy;
    box.w = 1;
    box.h = sh;
    SDL_FillRect(surface,&box,Color);
    box.x = sx + sw;
    SDL_FillRect(surface,&box,Color);
}
//SKIPPED FROM 329 to 561
Uint32 IndexColor(int color)
{
    switch(color)
    {
    case Red:
        return SDL_MapRGB(screen->format,138,0,0);;
    case Green:
        return SDL_MapRGB(screen->format,0,138,0);;
    case Blue:
        return SDL_MapRGB(screen->format,0,0,138);;
    case Yellow:
        return SDL_MapRGB(screen->format,128,128,0);;
    case Orange:
        return SDL_MapRGB(screen->format,148,118,0);;
    case Violet:
        return SDL_MapRGB(screen->format,128,0,128);
    case Brown:
        return SDL_MapRGB(screen->format,100,64,4);
    case Grey:
        return SDL_MapRGB(screen->format,128,128,128);
    case DarkRed:
        return SDL_MapRGB(screen->format,64,0,0);
    case DarkGreen:
        return SDL_MapRGB(screen->format,0,64,0);
    case DarkBlue:
        return SDL_MapRGB(screen->format,0,0,64);
    case DarkYellow:
        return SDL_MapRGB(screen->format,60,60,0);
    case DarkOrange:
        return SDL_MapRGB(screen->format,64,56,0);
    case DarkViolet:
        return SDL_MapRGB(screen->format,60,0,60);
    case DarkBrown:
        return SDL_MapRGB(screen->format,56,32,2);
    case DarkGrey:
        return SDL_MapRGB(screen->format,64,64,64);
    case LightRed:
        return SDL_MapRGB(screen->format,255,32,32);
    case LightGreen:
        return SDL_MapRGB(screen->format,32,255,32);
    case LightBlue:
        return SDL_MapRGB(screen->format,32,32,255);
    case LightYellow:
        return SDL_MapRGB(screen->format,250,250,60);
    case LightOrange:
        return SDL_MapRGB(screen->format,255,234,30);
    case LightViolet:
        return SDL_MapRGB(screen->format,250,30,250);
    case LightBrown:
        return SDL_MapRGB(screen->format,200,100,32);
    case LightGrey:
        return SDL_MapRGB(screen->format,196,196,196);
    case Black:
        return SDL_MapRGB(screen->format,0,0,0);
    case White:
        return SDL_MapRGB(screen->format,255,255,255);
    case Tan:
        return SDL_MapRGB(screen->format,255,128,64);
    case Gold:
        return SDL_MapRGB(screen->format,255,245,30);
    case Silver:
        return SDL_MapRGB(screen->format,206,206,226);
    case YellowGreen:
        return SDL_MapRGB(screen->format,196,255,30);
    case Cyan:
        return SDL_MapRGB(screen->format,0,255,255);;
    case Magenta:
        return SDL_MapRGB(screen->format,255,0,255);
    }
    return SDL_MapRGB(screen->format,0,0,0);
}

/*
  Copied from SDL's website.  I use it for palette swapping
  Its not plagerism if you document it!
*/
void DrawSquareLine(SDL_Surface *screen,Uint32 color,double sx,double sy,double gx,double gy)
{ 
  SDL_Rect box;
  if(sx < gx)box.x = sx;
  else box.x = gx;
  if(sy < gy)box.y = sy;
  else box.y = gy;
  if(sy == gy)
  {
    box.w = fabs(sx - gx);
    box.h = 1;                                        
    SDL_FillRect(screen,&box,color);    
    return;
  }
  box.h = fabs(sy - gy);
  box.w = 1;                                        
  SDL_FillRect(screen,&box,color);    
}

void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
    switch (screen->format->BytesPerPixel)
    {
        case 1:
        { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2:
        { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3:
        { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4:
        { /* Probably 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, x, y, 1, 1);
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    /* Here p is the address to the pixel we want to retrieve*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
    if((x < 0)||(x >= surface->w)||(y < 0)||(y >= surface->h))return -1;
    switch(surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /*shouldn't happen, but avoids warnings*/
    }
}



/*
 * the putpixel function ont he SDL website doesn't always wrk right.  Here is a REAL simple alternative.
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  SDL_Rect point = {0,0,1,1};
  point.x = x;
  point.y = y;
  SDL_FillRect(surface,&point,pixel);
}

void DrawThickLine(int sx,int sy,int dx, int dy,int width,Uint32 Color,SDL_Surface *surface)
{
  SDL_Rect box;
  int deltax,deltay;
  int x,y,curpixel;
  int den,num,numadd,numpixels;
  int xinc1,xinc2,yinc1,yinc2;
  box.w = width;
  box.h = width;
  deltax = fabs(dx - sx);        // The difference between the x's
  deltay = fabs(dy - sy);        // The difference between the y's
  x = sx;                       // Start x off at the first pixel
  y = sy;                       // Start y off at the first pixel

  if (dx >= sx)                 // The x-values are increasing
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          // The x-values are decreasing
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (dy >= sy)                 // The y-values are increasing
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          // The y-values are decreasing
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         // There is at least one x-value for every y-value
  {
    xinc1 = 0;                  // Don't change the x when numerator >= denominator
    yinc2 = 0;                  // Don't change the y for every iteration
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         // There are more x-values than y-values
  }
  else                          // There is at least one y-value for every x-value
  {
    xinc2 = 0;                  // Don't change the x for every iteration
    yinc1 = 0;                  // Don't change the y when numerator >= denominator
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         // There are more y-values than x-values
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    box.x = x;
    box.y = y;
    SDL_FillRect(surface,&box,Color);    
    num += numadd;              // Increase the numerator by the top of the fraction
    if (num >= den)             // Check if numerator >= denominator
    {
      num -= den;               // Calculate the new numerator value
      x += xinc1;               // Change the x as appropriate
      y += yinc1;               // Change the y as appropriate
    }
    x += xinc2;                 // Change the x as appropriate
    y += yinc2;                 // Change the y as appropriate
  }
}

void DrawAnyLine(int sx,int sy,int dx, int dy,Uint32 Color,SDL_Surface *surface)
{
  int deltax,deltay;
  int x,y,curpixel;
  int den,num,numadd,numpixels;
  int xinc1,xinc2,yinc1,yinc2;
  deltax = fabs(dx - sx);        // The difference between the x's
  deltay = fabs(dy - sy);        // The difference between the y's
  x = sx;                       // Start x off at the first pixel
  y = sy;                       // Start y off at the first pixel

  if (dx >= sx)                 // The x-values are increasing
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          // The x-values are decreasing
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (dy >= sy)                 // The y-values are increasing
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          // The y-values are decreasing
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         // There is at least one x-value for every y-value
  {
    xinc1 = 0;                  // Don't change the x when numerator >= denominator
    yinc2 = 0;                  // Don't change the y for every iteration
    den = deltax;
    num = deltax >> 1;
    numadd = deltay;
    numpixels = deltax;         // There are more x-values than y-values
  }
  else                          // There is at least one y-value for every x-value
  {
    xinc2 = 0;                  // Don't change the x for every iteration
    yinc1 = 0;                  // Don't change the y when numerator >= denominator
    den = deltay;
    num = deltay >> 1;
    numadd = deltax;
    numpixels = deltay;         // There are more y-values than x-values
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    putpixel(surface,x, y,Color);             // Draw the current pixel
    num += numadd;              // Increase the numerator by the top of the fraction
    if (num >= den)             // Check if numerator >= denominator
    {
      num -= den;               // Calculate the new numerator value
      x += xinc1;               // Change the x as appropriate
      y += yinc1;               // Change the y as appropriate
    }
    x += xinc2;                 // Change the x as appropriate
    y += yinc2;                 // Change the y as appropriate
  }
}

/*
  copied and pasted and then significantly modified from the sdl website.  
  I kept ShowBMP to test my program as I wrote it, and I rewrote it to use any file type supported by SDL_image
*/

void ShowBMP(SDL_Surface *image, SDL_Surface *screen, int x, int y)
{
    SDL_Rect dest;

    /* Blit onto the screen surface.
       The surfaces should not be locked at this point.
     */
    dest.x = x;
    dest.y = y;
    dest.w = image->w;
    dest.h = image->h;
    SDL_BlitSurface(image, NULL, screen, &dest);

    /* Update the changed portion of the screen */
    SDL_UpdateRects(screen, 1, &dest);
}
/*draws a filled rect at the coordinates, in the color, on the surface specified*/
void DrawFilledRect(int sx,int sy, int sw, int sh, Uint32 Color, SDL_Surface *surface)
{
  SDL_Rect dst;
  dst.x = sx;
  dst.y = sy;
  dst.w = sw;
  dst.h = sh;
  SDL_FillRect(surface,&dst,Color);
}

/*sets an sdl surface to all color.*/

void BlankScreen(SDL_Surface *buf,Uint32 color)
{
    SDL_LockSurface(buf);
    memset(buf->pixels, (Uint8)color,buf->format->BytesPerPixel * buf->w *buf->h);
    SDL_UnlockSurface(buf);
}

