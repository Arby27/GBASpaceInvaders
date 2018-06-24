
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include <stdio.h>
#include <stdlib.h>
#include "collision.h"
#include "ScoreUpdate.h"




extern int loosealife(int* lives);

extern int collision(int bulletX, int bulletY, int targetX, int targetY, int boxWidth, int boxHeight);
extern void scoreUpdate(int *score, int *tens, int *hundreds, int *bulletIsDead, unsigned short* OAM,unsigned short* scoremap1,unsigned short* scoremap2,unsigned short* scoremap3 );

struct Player
{
int xPos;
int yPos;
int isFiring;
int isMoving;
int isBlocking;
};


struct Bullet
{
	short xPos;
	short yPos;
	short isDead;
};

struct Enemy
{
short xPos;
short yPos;
short moveRight;
short isFiring;
short isDead;
short isBlocking;
};




//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

extern const unsigned int charData[256* 8];
	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	irqInit();
	irqSet( IRQ_VBLANK,mmVBlank);
	irqEnable(IRQ_VBLANK);
	
	// generic setup function
	//consoleDemoInit();
	mmInitDefault((mm_addr)soundbank_bin,8); //initialise sound 
	mmStart(MOD_IMPERIALMARCH,MM_PLAY_LOOP); //sound file start and loop

int gameOver = 0;
int mainMenu = 1;
int seedCounter = 0;
int reset = 0;

while(1)
{

if(mainMenu == 1)
{
	unsigned short* dispnct =(unsigned short*) 0x04000000;

dispnct[0] = (0<<0)|(0<<1)|(0<<2)|/*graphics mode 0-5*/(1<<6)/*OBJ data mode*/|(1<<8)|(1<<9)|(1<<10)|(1<<11)/*BG layers 0-3*/|(1<<12)/*OBJs on or off*/;

unsigned short* BG0CNT =(unsigned short*) 0x04000008; 
unsigned short* BG1CNT = (unsigned short*) 0x0400000A; 
unsigned short* BG2CNT = (unsigned short*) 0x0400000C; 
unsigned short* BG3CNT = (unsigned short*) 0x0400000E; 

BG0CNT[0] = (0<<0)/*layer priority 0-3(0 closest)*/|(0<<2)|(0<<3)/*character base block number*/|(0<<7)/*colour mode*/|(0<<8)|(0<<9)|(0<<10)|(1<<11)|(0<<12)/*screen base block layer*/|(0<<14)|(0<<15)/*screen size setting*/;
BG1CNT[0] = (1<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(0<<8)|(0<<9)|(1<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);
BG2CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(0<<8)|(1<<9)|(0<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);
BG3CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(1<<8)|(0<<9)|(0<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);



unsigned int* character = (unsigned int*) 0x6000000;

for(int i = 0; i<1024;i++)
{
character[i] = charData[i];
}

unsigned short* tagMap = (unsigned short*) 0x060040C6;

//New
tagMap[0] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[1] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[2] = (23<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
//And
tagMap[4] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[5] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[6] = (4<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
//Totally 
tagMap[8] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[9] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[10] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[11] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[12] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[13] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[14] = (25<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
//Original
tagMap[16] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[17] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[18] = (9<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[19] = (7<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[20] = (9<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[21] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[22] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
tagMap[23] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);



unsigned short* titleMap = (unsigned short*) 0x06004190;

//Sporce
titleMap[0] = (19<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[1] = (16<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[2] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[3] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[4] = (3<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[5] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

titleMap[6] = (0<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//Invordors
titleMap[7] = (9<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[8] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[9] = (22<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[10] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[11] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[12] = (4<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[13] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[14] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
titleMap[15] = (19<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);


//Text displaying play
unsigned short* playMap = (unsigned short*) 0x06004296;
playMap[0] = (16<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
playMap[1] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
playMap[2] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
playMap[3] = (25<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);


//Text displaying the controls for the game
unsigned short* controlsMap = (unsigned short*) 0x06004304;
//controls
controlsMap[0] = (3<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[1] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[2] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[3] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[4] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[5] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[6] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[7] = (19<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[8] = (42<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//A
controlsMap[10] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//To
controlsMap[12] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[13] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//Shoot
controlsMap[15] = (19<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[16] = (8<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[17] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[18] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
controlsMap[19] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//Second line of controls
unsigned short* Controls2 = (unsigned short*) 0x06004398;
//B
Controls2[0] = (2<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//To
Controls2[2] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls2[3] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//Block
Controls2[5] = (2<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls2[6] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls2[7] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls2[8] = (3<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls2[9] = (11<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

//Third line of controls
unsigned short* Controls3 = (unsigned short*) 0x06004418;

//D-Pad
Controls3[0] = (4<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[1] = (16<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[2] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[3] = (4<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
//To 
Controls3[5] = (20<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[6] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
//Move
Controls3[8] = (13<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[9] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[10] = (22<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Controls3[11] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);


unsigned short* palette = (unsigned short*) 0x05000000;
//BG palette 1
palette[1]= (31<<0);
palette[2]= (31<<5);
palette[3]= (31<<10);
palette[4]= (31<<0)|(31<<5)|(31<<10);


unsigned short* OBJPalette = (unsigned short*) 0x05000200;
//OBJ palette 1
OBJPalette[0] =(31<<0)|(31<<5)|(31<<10);
OBJPalette[1] =(31<<5)|(31<<10);
OBJPalette[2] =(31<<0)|(31<<10);
OBJPalette[3] =(31<<0)|(31<<5);
OBJPalette[4] =(31<<0)|(0<<5)|(0<<10);
OBJPalette[5] =(31<<5);
OBJPalette[6] =(31<<10);	
OBJPalette[7] =(31<<0)|(15<<5);
OBJPalette[8] =(31<<0)|(15<<10);
OBJPalette[9] =(31<<5)|(15<<10);
OBJPalette[11] =(31<<0)|(31<<5)|(31<<10);



unsigned int* sprite = (unsigned int*) 0x06010000;
for(int i = 0; i<8;i++)
{
	sprite[i] = 0;
}

sprite[10] = (1<<12);
sprite[11] = (4<<8)|(11<<12)|(4<<16);
sprite[12] = (11<<4)|(4<<8)|(11<<12)|(4<<16)|(11<<20);
sprite[13] = (11<<4)|(11<<20);


unsigned short* OAM = (unsigned short*) 0x07000000;

OAM[0] = (81<<0)/*y coord*/|(0<<8)/*rotation/scaling*/|(0<<9)/*OBJ disable*/|(0<<14)/*shape lookup*/;
OAM[1] = (79<<0)/*x coord*/|(0<<12)/*horizontal flip*/|(0<<13)/*vertical flip*/|(0<<14)/*size lookup*/;
OAM[2] = (1<<0)/*raw sprite image id*/|(1<<10)/*priority*/|(0<<12)/*palette to use*/;
OAM[3] = 0;

while(mainMenu ==1)
{
	scanKeys();
	unsigned short keys = keysHeld();
	seedCounter++;
	if((KEY_A & keys))
	{
		
			for(int i = 0; i<1024;i++)
			{
				character[i] = 0;
				playMap[i] = 0;
				controlsMap[i] = 0;
				Controls2[i] = 0;
				Controls3[i] = 0;
				titleMap[i] = 0;
				tagMap[i] = 0;
			}
			mainMenu = 0;
		
		
	}
	
	VBlankIntrWait();
}





	
}
////////////////////////////////////////////////////////////GAME OVER SCREEN/////////////////////////////////////
if(gameOver == 1)
{
	
short yPos = 81;	
	
unsigned short* dispnct =(unsigned short*) 0x04000000;

dispnct[0] = (0<<0)|(0<<1)|(0<<2)|/*graphics mode 0-5*/(1<<6)/*OBJ data mode*/|(1<<8)|(1<<9)|(1<<10)|(1<<11)/*BG layers 0-3*/|(1<<12)/*OBJs on or off*/;

unsigned short* BG0CNT =(unsigned short*) 0x04000008; 
unsigned short* BG1CNT = (unsigned short*) 0x0400000A; 
unsigned short* BG2CNT = (unsigned short*) 0x0400000C; 
unsigned short* BG3CNT = (unsigned short*) 0x0400000E; 

BG0CNT[0] = (0<<0)/*layer priority 0-3(0 closest)*/|(0<<2)|(0<<3)/*character base block number*/|(0<<7)/*colour mode*/|(0<<8)|(0<<9)|(0<<10)|(1<<11)|(0<<12)/*screen base block layer*/|(0<<14)|(0<<15)/*screen size setting*/;
BG1CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(0<<8)|(0<<9)|(1<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);
BG2CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(0<<8)|(1<<9)|(0<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);
BG3CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(1<<8)|(0<<9)|(0<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);



unsigned int* character = (unsigned int*) 0x6000000;

for(int i = 0; i<1024;i++)
{
character[i] = charData[i];
}

unsigned short* GameOverMap = (unsigned short*) 0x60041D6;

GameOverMap[0] = (7<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
GameOverMap[1] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
GameOverMap[2] = (13<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
GameOverMap[3] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

GameOverMap[4] = (0<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

GameOverMap[5] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
GameOverMap[6] = (22<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
GameOverMap[7] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
GameOverMap[8] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);



unsigned short* PlayAgainMap = (unsigned short*) 0x6004296;

PlayAgainMap[0] = (16<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[1] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[2] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[3] = (25<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

PlayAgainMap[4] = (0<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

PlayAgainMap[5] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[6] = (7<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[7] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[8] = (9<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
PlayAgainMap[9] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);



unsigned short* MainMenuMap = (unsigned short*) 0x6004316;
MainMenuMap[0] = (13<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
MainMenuMap[1] = (1<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
MainMenuMap[2] = (9<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
MainMenuMap[3] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

MainMenuMap[4] = (0<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

MainMenuMap[5] = (13<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
MainMenuMap[6] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
MainMenuMap[7] = (14<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
MainMenuMap[8] = (21<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);



unsigned short* palette = (unsigned short*) 0x05000000;
//BG palette 1
palette[1]= (31<<0);
palette[2]= (31<<5);
palette[3]= (31<<10);
palette[4]= (31<<0)|(31<<5)|(31<<10);


unsigned short* OBJPalette = (unsigned short*) 0x05000200;
//OBJ palette 1
OBJPalette[0] =(31<<0)|(31<<5)|(31<<10);
OBJPalette[1] =(31<<5)|(31<<10);
OBJPalette[2] =(31<<0)|(31<<10);
OBJPalette[3] =(31<<0)|(31<<5);
OBJPalette[4] =(31<<0)|(0<<5)|(0<<10);
OBJPalette[5] =(31<<5);
OBJPalette[6] =(31<<10);	
OBJPalette[7] =(31<<0)|(15<<5);
OBJPalette[8] =(31<<0)|(15<<10);
OBJPalette[9] =(31<<5)|(15<<10);
OBJPalette[11] =(31<<0)|(31<<5)|(31<<10);



unsigned int* sprite = (unsigned int*) 0x06010000;
for(int i = 0; i<8;i++)
{
	sprite[i] = 0;
}

sprite[10] = (1<<12);
sprite[11] = (4<<8)|(11<<12)|(4<<16);
sprite[12] = (11<<4)|(4<<8)|(11<<12)|(4<<16)|(11<<20);
sprite[13] = (11<<4)|(11<<20);


unsigned short* OAM = (unsigned short*) 0x07000000;

OAM[0] = (81<<0)/*y coord*/|(0<<8)/*rotation/scaling*/|(0<<9)/*OBJ disable*/|(0<<14)/*shape lookup*/;
OAM[1] = (79<<0)/*x coord*/|(0<<12)/*horizontal flip*/|(0<<13)/*vertical flip*/|(0<<14)/*size lookup*/;
OAM[2] = (1<<0)/*raw sprite image id*/|(1<<10)/*priority*/|(0<<12)/*palette to use*/;
OAM[3] = 0;


while(gameOver == 1)
{
	
	scanKeys();
	unsigned short keys = keysHeld();
	if((KEY_DOWN & keys) != 0 )
	{
		OAM[0] = (97<<0);
		yPos = 97;
	}
	
	if((KEY_UP & keys) != 0 )
	{
		OAM[0] = (81<<0);
		yPos = 81;
	}
	
	if((KEY_A & keys))
	{
		if(yPos == 81)
		{
			for(int i = 0; i<1024;i++)
			{
				character[i] = 0;
				PlayAgainMap[i] = 0;
				GameOverMap[i] = 0;
			}
			gameOver = 0;
		}
		if(yPos == 97)
		{
			for(int i = 0; i<1024;i++)
			{
			character[i] = 0;
				PlayAgainMap[i] = 0;
				GameOverMap[i] = 0;
			}
			mainMenu = 1;
			gameOver = 0;
			
		}
		
	}
	
	VBlankIntrWait();
}

}




//////////////////////////////////////GAME STATE///////////////////////////////////////////
if(gameOver == 0 && mainMenu == 0){	

int lives = 30;	
int *score = 27;
int *tens = 27;
int *hundreds = 27;

	

	struct Player player;
	player.xPos = 115;
	player.yPos = 150;
	player.isMoving = 0; //false = 0, true = 1
	player.isFiring = 0;
	player.isBlocking = 0;
	
	struct Bullet bullet[2];
	bullet[0].yPos = 0;
	bullet[0].xPos = 0;
	bullet[1].yPos = 0;
	bullet[1].xPos = 0;
	bullet[0].isDead = 0;
	bullet[1].isDead = 0;
	
	
	struct Enemy enemy[21];
	enemy[0].yPos = 25;
	enemy[0].xPos = 16;
	enemy[0].moveRight = 1;
	enemy[0].isFiring =0;
	
	
	for(int i = 1; i < 20; i++)
	{
		enemy[i].yPos = 25;
		enemy[i].xPos = (enemy[i-1].xPos + 8);
		enemy[i].isDead = 0;
	}
	
	enemy[20].yPos = 20; 
	enemy[20].xPos = 250;
	enemy[20].isDead = 0;


unsigned short* dispnct =(unsigned short*) 0x04000000;

dispnct[0] = (0<<0)|(0<<1)|(0<<2)|/*graphics mode 0-5*/(1<<6)/*OBJ data mode*/|(1<<8)|(1<<9)|(1<<10)|(1<<11)/*BG layers 0-3*/|(1<<12)/*OBJs on or off*/;

unsigned short* BG0CNT =(unsigned short*) 0x04000008; 
unsigned short* BG1CNT = (unsigned short*) 0x0400000A; 
unsigned short* BG2CNT = (unsigned short*) 0x0400000C; 
unsigned short* BG3CNT = (unsigned short*) 0x0400000E; 

BG0CNT[0] = (0<<0)/*layer priority 0-3(0 closest)*/|(0<<2)|(0<<3)/*character base block number*/|(0<<7)/*colour mode*/|(0<<8)|(0<<9)|(0<<10)|(1<<11)|(0<<12)/*screen base block layer*/|(0<<14)|(0<<15)/*screen size setting*/;
BG1CNT[0] = (1<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(0<<8)|(0<<9)|(1<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);
BG2CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(0<<8)|(1<<9)|(0<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);
BG3CNT[0] = (0<<0)|(0<<1)|(0<<2)|(0<<3)|(0<<7)|(1<<8)|(0<<9)|(0<<10)|(0<<11)|(0<<12)|(0<<14)|(0<<15);

unsigned int* character = (unsigned int*) 0x6000000;
//BG sprite

for(int i = 0; i<1024; i++)
{
	character[i] = charData[i];
}

// screen coords = ((y*32)+x) then convert ans to hex and add to baseblock mem adr
unsigned short* Scoremap = (unsigned short*) 0x6004044;

Scoremap[0] = (19<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[1] = (3<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[2] = (15<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[3] = (18<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[4] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15); 

Scoremap[5] = (0<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);

Scoremap[6] = (27<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[7] = (27<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[8] = (27<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
Scoremap[9] = (27<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);


unsigned short* LivesMap = (unsigned short*) 0x6004064;
 LivesMap[0] = (12<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 LivesMap[1] = (9<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 LivesMap[2] = (22<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 LivesMap[3] = (5<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 LivesMap[4] = (19<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 
 LivesMap[6] = (0<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 
 LivesMap[7] = ((lives)<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 

 
 unsigned short* backGroundMap = (unsigned short*) 0x06002080;
 for (int i = 0 ; i < 640;i++)
 {
	 
	
	 backGroundMap[i] = (( rand() % 37 + 42)<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
 }



unsigned short* palette = (unsigned short*) 0x05000000;
//BG palette 1
palette[1]= (31<<0);
palette[2]= (31<<5);
palette[3]= (31<<10);
palette[4]= (31<<0)|(31<<5)|(31<<10);


unsigned short* OBJPalette = (unsigned short*) 0x05000200;
//OBJ palette 1
OBJPalette[0] =(31<<0)|(31<<5)|(31<<10);
OBJPalette[1] =(31<<5)|(31<<10);
OBJPalette[2] =(31<<0)|(31<<10);
OBJPalette[3] =(31<<0)|(31<<5);
OBJPalette[4] =(31<<0)|(0<<5)|(0<<10);
OBJPalette[5] =(31<<5);
OBJPalette[6] =(31<<10);	
OBJPalette[7] =(31<<0)|(15<<5);
OBJPalette[8] =(31<<0)|(15<<10);
OBJPalette[9] =(31<<5)|(15<<10);
OBJPalette[11] =(31<<0)|(31<<5)|(31<<10);

///////////////////////////////////////////////OBJ SPRITES////////////////////////////////////////////////////////
	
unsigned int* sprite = (unsigned int*) 0x06010000;
for(int i = 0; i<8;i++)
{
	sprite[i] = 0;
}

sprite[10] = (1<<12);
sprite[11] = (4<<8)|(11<<12)|(4<<16);
sprite[12] = (11<<4)|(4<<8)|(11<<12)|(4<<16)|(11<<20);
sprite[13] = (11<<4)|(11<<20);


sprite[16]= 0;
sprite[17]= (4<<12)|(4<<16);
sprite[18]= (4<<12)|(4<<16);
sprite[19]= (4<<12)|(4<<16);
sprite[20]= (4<<12)|(4<<16);
sprite[21]= (4<<12)|(4<<16);
sprite[21]= (4<<12)|(4<<16);
sprite[22]= (4<<12)|(4<<16);
sprite[23] = 0;

sprite[24] = (1<<12)|(1<<16);
sprite[25] = (1<<8)|(1<<12)|(1<<16)|(1<<20);
sprite[26] = (1<<4)|(1<<12)|(1<<20)|(1<<24);
sprite[27] = (1<<4)|(1<<8)|(1<<12)|(1<<16)|(1<<20)|(1<<24);
sprite[28] = (1<<8)|(1<<20);
sprite[29] = (1<<8)|(1<<12)|(1<<16)|(1<<20);
sprite[30] = (1<<8)|(1<<20);

sprite[32] = (6<<12)|(6<<16);
sprite[33] = (6<<8)|(6<<20);
sprite[34] = (6<<4)|(1<<12)|(6<<24);
sprite[35] = (6<<0)|(4<<8)|(11<<12)|(4<<16)|(6<<28);
sprite[36] = (11<<4)|(4<<8)|(11<<12)|(4<<16)|(11<<20);
sprite[37] = (11<<4)|(11<<20);


sprite[41] = (7<<20)|(7<<24)|(7<<28);
sprite[42] = (7<<16)|(7<<20)|(7<<24)|(7<<28);
sprite[43] = (7<<12)|(7<<16)|(7<<28);
sprite[44] = (7<<8)|(7<<12)|(7<<16)|(7<<28);
sprite[45] = (7<<4)|(7<<8)|(7<<12)|(7<<16)|(7<<20)|(7<<24)|(7<<28);
sprite[46] = (7<<4)|(7<<8)|(7<<12)|(7<<16)|(7<<20)|(7<<24)|(7<<28);
sprite[47] = (7<<0)|(7<<12)|(7<<20)|(7<<28);

sprite[49] = (7<<0)|(7<<4)|(7<<8);
sprite[50] = (7<<0)|(7<<4)|(7<<8)|(7<<12);
sprite[51] = (7<<0)|(7<<12)|(7<<16);
sprite[52] = (7<<0)|(7<<12)|(7<<16)|(7<<20);
sprite[53] = (7<<0)|(7<<4)|(7<<8)|(7<<12)|(7<<16)|(7<<20)|(7<<24);
sprite[54] = (7<<0)|(7<<4)|(7<<8)|(7<<12)|(7<<16)|(7<<20)|(7<<24);
sprite[55] = (7<<0)|(7<<12)|(7<<20)|(7<<28);
////////////////////////////////////////////////OAMs///////////////////////////////////////////////////////////////


unsigned short* OAM = (unsigned short*) 0x07000000;

	OAM[0] = (player.yPos<<0)/*y coord*/|(0<<8)/*rotation/scaling*/|(0<<9)/*OBJ disable*/|(0<<14)/*shape lookup*/;
	OAM[1] = (player.xPos<<0)/*x coord*/|(0<<12)/*horizontal flip*/|(0<<13)/*vertical flip*/|(0<<14)/*size lookup*/;
	OAM[2] = (1<<0)/*raw sprite image id*/|(2<<10)/*priority*/|(0<<12)/*palette to use*/;
	OAM[3] = 0;



	
	
	
unsigned short* bulletOAM = (unsigned short*) 0x07000010;
bulletOAM[0] = (255<<0)/*y coord*/|(0<<8)/*rotation/scaling*/|(0<<9)/*OBJ disable*/|(0<<14)/*shape lookup*/;
bulletOAM[1] = (255<<0)/*x coord*/|(0<<12)/*horizontal flip*/|(0<<13)/*vertical flip*/|(0<<14)/*size lookup*/;
bulletOAM[2] = (2<<0)/*raw sprite image id*/|(0<<10)/*priority*/|(0<<12)/*palette to use*/;
bulletOAM[3] = 0;

int j,k,l,m;
j = 0;
k = 1;
l = 2;
m = 3;
unsigned short* enemyOAM = (unsigned short*)0x07000020;
for(int i = 0; i<20;i++)
{

enemyOAM[j] = (enemy[i].yPos<<0)|(0<<8)|(0<<9)|(0<<14);
enemyOAM[k] = ((enemy[i].xPos)<<0)|(0<<12)|(0<<13)|(0<<14);
enemyOAM[l] = (3<<0)|(0<<10)|(0<<12);
enemyOAM[m] = 0;

j = j + 4;
k = k+4;
l = l+4;
m = m+4;

}

unsigned short* enemyBulletOAM = (unsigned short*) 0x07000D20;
enemyBulletOAM[0] = (250<<0)|(0<<8)|(0<<9)|(0<<14);
enemyBulletOAM[1] = (250<<0)|(0<<12)|(0<<13)|(0<<14);
enemyBulletOAM[2] = (2<<0)|(0<<10)|(0<<12);
enemyBulletOAM[3] = 0;

unsigned short* largeEnemyOAM = (unsigned short*) 0x07000D30;
largeEnemyOAM[0] = (enemy[20].yPos<<0)|(0<<8)|(0<<9)|(1<<14);
largeEnemyOAM[1] = (enemy[20].xPos<<0)|(0<<12)|(0<<13)|(0<<14);
largeEnemyOAM[2] = (5<<0)|(0<<10)|(0<<12);
largeEnemyOAM[3] = 0;

short largeTimer = 0;
short passCount = 0;
srand(seedCounter);
	//////////////////////////////////////////// MAIN LOOP/////////////////////////////////////////////////////////////
	while (gameOver == 0) 
	
	{
	
	largeTimer++;
	
	
	
	
	///////////////////////////////////////////Player Movement//////////////////////////////////////////////////////
	scanKeys();
	unsigned short keys = keysHeld();
	if((KEY_DOWN & keys) != 0 && (KEY_LEFT& keys)== 0 && (KEY_RIGHT & keys)== 0)
	{
		player.yPos ++;
		if(player.yPos < 255)
		{
			OAM[0] = ((player.yPos<<0));
			
		}
	}
	if((KEY_UP & keys) !=0 && (KEY_LEFT& keys)== 0 && (KEY_RIGHT & keys)== 0) 
	{
		player.yPos --;
		if(player.yPos > 0)
		{
			OAM[0] = ((player.yPos<<0));
			OAM[2] = (5<<0)|(1<<10)|(0<<12);
			
		}
	}
	
	if(KEY_LEFT & keys) 
	{
		player.xPos --;
		if(player.xPos > 0)
		{
			OAM[1] = ((player.xPos<<0));
			
		}
	}
	if(KEY_RIGHT & keys)
	{
		
		player.xPos ++;
		if(player.xPos < 255)
		{
			OAM[1] = ((player.xPos<<0));
			
		}
	}
	/////////////////////////////////////////////////Screen Boundaries//////////////////////////////////////////////////
	if(player.xPos < 1)
	{
		player.xPos = 1;
	}
	
	if(player.xPos > 230)
	{
		player.xPos = 230;
	}
	
	if(player.yPos < 1)
	{
		player.yPos = 1;
	}
	
	if(player.yPos > 150)
	{
		player.yPos = 150;
	}
	
	/////////////////////////////////PLAYER FIRING////////////////////////////////////////////////////////////////////////
	if(player.isBlocking == 0)
	{
	if(player.isFiring == 0)
	{
	if(KEY_A & keys)
	{
		
		mmStart(MOD_SOUNDEFFECTS,MM_PLAY_ONCE); //sound file start and loop
		bullet[0].yPos = player.yPos;
		bullet[0].xPos = player.xPos;
		bulletOAM[0] = (bullet[0].yPos<<0);
		bulletOAM[1] = (bullet[0].xPos<<0);
		bullet[0].xPos = bullet[0].xPos;
		player.isFiring = 1;
	
	}

	}
}	
		if(player.isFiring == 1)
		{
			bullet[0].xPos = bullet[0].xPos;
			bullet[0].yPos--;
			bullet[0].yPos--;
			bulletOAM[0] = (bullet[0].yPos<<0);
		}
		
		if(bullet[0].yPos < 0)
		{
			bullet[0].isDead = 0;
			player.isFiring = 0;
			bullet[0].yPos = 0;
			mmStart(MOD_IMPERIALMARCH,MM_PLAY_LOOP);
			
	
		}
		

//Enemy Collisoin Detection
enemy[0].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[0].xPos,enemy[0].yPos,8,8);
enemy[1].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[1].xPos,enemy[1].yPos,8,8);
enemy[2].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[2].xPos,enemy[2].yPos,8,8);
enemy[3].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[3].xPos,enemy[3].yPos,8,8);
enemy[4].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[4].xPos,enemy[4].yPos,8,8);
enemy[5].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[5].xPos,enemy[5].yPos,8,8);
enemy[6].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[6].xPos,enemy[6].yPos,8,8);
enemy[7].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[7].xPos,enemy[7].yPos,8,8);
enemy[8].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[8].xPos,enemy[8].yPos,8,8);
enemy[9].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[9].xPos,enemy[9].yPos,8,8);
enemy[10].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[10].xPos,enemy[10].yPos,8,8);
enemy[11].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[11].xPos,enemy[11].yPos,8,8);
enemy[12].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[12].xPos,enemy[12].yPos,8,8);
enemy[13].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[13].xPos,enemy[13].yPos,8,8);
enemy[14].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[14].xPos,enemy[14].yPos,8,8);
enemy[15].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[15].xPos,enemy[15].yPos,8,8);
enemy[16].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[16].xPos,enemy[16].yPos,8,8);
enemy[17].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[17].xPos,enemy[17].yPos,8,8);
enemy[18].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[18].xPos,enemy[18].yPos,8,8);
enemy[19].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[19].xPos,enemy[19].yPos,8,8);

enemy[20].isDead = collision(bullet[0].xPos,bullet[0].yPos,enemy[20].xPos,enemy[20].yPos,16,8);


//If collisoin is true update the score and move the target
if(bullet[0].isDead == 0)
{
if(enemy[0].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[2],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[1].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[6],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[2].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[10],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[3].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[14],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[4].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[18],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[5].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[22],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[6].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[26],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[7].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[30],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[8].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[34],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[9].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[38],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[10].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[42],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[11].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead,&enemyOAM[46],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[12].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[50],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[13].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[54],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[14].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[58],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[15].isDead == 1){scoreUpdate(&score, &tens, &hundreds,&bullet[0].isDead, &enemyOAM[62],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[16].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[66],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[17].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[70],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[18].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[74],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
if(enemy[19].isDead == 1){scoreUpdate(&score, &tens, &hundreds, &bullet[0].isDead,&enemyOAM[78],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}

if(enemy[20].isDead == 1){scoreUpdate(&score,&tens,&hundreds,&bullet[0].isDead,&largeEnemyOAM[2],&Scoremap[7],&Scoremap[8],&Scoremap[9]);}
}


/*	for(int i =0; i<20; i++)
	{
		if(enemy[i].isDead == 1)
		{
			reset++;
		}
		if(reset >= 20)
		{
			enemy[i].isDead = 0;
		}
		if(enemy[i].isDead == 0)
		{
			int j = 2;
			enemyOAM[j] = (3<<0);
			j=j+4;
		}
	}
*/


	
	if(KEY_B & keys)
	{
		player.isBlocking = 1;
		OAM[2] = (4<<0);
	}
	else
	{
		OAM[2] = (1<<0);
		player.isBlocking = 0;
	}
	
	////////////////////////////////////////enemy[0] MOVEMENT///////////////////////////////////////////////////////
	

	
	if(enemy[0].moveRight== 1)
	{
	for(int i = 0; i < 20; i++)
	{
		
		enemy[i].xPos++;
		
	}
		enemyOAM[1] = (enemy[0].xPos <<0);
		enemyOAM[5] = ((enemy[1].xPos ) <<0);
		enemyOAM[9] = ((enemy[2].xPos ) <<0);
		enemyOAM[13] = ((enemy[3].xPos ) <<0);
		enemyOAM[17] = ((enemy[4].xPos ) <<0);
		enemyOAM[21] = ((enemy[5].xPos ) <<0);
		enemyOAM[25] = ((enemy[6].xPos ) <<0);
		enemyOAM[29] = ((enemy[7].xPos ) <<0);
		enemyOAM[33] = ((enemy[8].xPos ) <<0);
		enemyOAM[37] = ((enemy[9].xPos ) <<0);
		enemyOAM[41] = ((enemy[10].xPos ) <<0);
		enemyOAM[45] = ((enemy[11].xPos ) <<0);
		enemyOAM[49] = ((enemy[12].xPos ) <<0);
		enemyOAM[53] = ((enemy[13].xPos ) <<0);
		enemyOAM[57] = ((enemy[14].xPos ) <<0);
		enemyOAM[61] = ((enemy[15].xPos ) <<0);
		enemyOAM[65] = ((enemy[16].xPos ) <<0);
		enemyOAM[69] = ((enemy[17].xPos ) <<0);
		enemyOAM[73] = ((enemy[18].xPos ) <<0);
		enemyOAM[77] = ((enemy[19].xPos ) <<0);
	}

	if(enemy[0].xPos >= 88)
	{
		for(int i = 0 ; i < 20; i++)
		{
		enemy[i].yPos = enemy[0].yPos + 3;
		}
		enemyOAM[0] = (enemy[0].yPos<<0);
		enemyOAM[4] = (enemy[0].yPos<<0);
		enemyOAM[8] = (enemy[0].yPos<<0);
		enemyOAM[12] = (enemy[0].yPos<<0);
		enemyOAM[16] = (enemy[0].yPos<<0);
		enemyOAM[20] = (enemy[0].yPos<<0);
		enemyOAM[24] = (enemy[0].yPos<<0);
		enemyOAM[28] = (enemy[0].yPos<<0);
		enemyOAM[32] = (enemy[0].yPos<<0);
		enemyOAM[36] = (enemy[0].yPos<<0);
		enemyOAM[40] = (enemy[0].yPos<<0);
		enemyOAM[44] = (enemy[0].yPos<<0);
		enemyOAM[48] = (enemy[0].yPos<<0);
		enemyOAM[52] = (enemy[0].yPos<<0);
		enemyOAM[56] = (enemy[0].yPos<<0);
		enemyOAM[60] = (enemy[0].yPos<<0);
		enemyOAM[64] = (enemy[0].yPos<<0);
		enemyOAM[68] = (enemy[0].yPos<<0);
		enemyOAM[72] = (enemy[0].yPos<<0);
		enemyOAM[76] = (enemy[0].yPos<<0);
		enemy[0].moveRight = 0;
	}
		
	if(enemy[0].moveRight==0)
	{
for(int i = 0; i < 20; i++)
	{
		if(enemy[i].isDead == 0)
		{
		enemy[i].xPos--;
		}
	}
		enemyOAM[1] = (enemy[0].xPos <<0);
		enemyOAM[5] = (enemy[1].xPos <<0);
		enemyOAM[9] = (enemy[2].xPos <<0);
		enemyOAM[13] = (enemy[3].xPos  <<0);
		enemyOAM[17] = (enemy[4].xPos  <<0);
		enemyOAM[21] = (enemy[5].xPos  <<0);
		enemyOAM[25] = (enemy[6].xPos  <<0);
		enemyOAM[29] = (enemy[7].xPos  <<0);
		enemyOAM[33] = (enemy[8].xPos  <<0);
		enemyOAM[37] = (enemy[9].xPos  <<0);
		enemyOAM[41] = (enemy[10].xPos <<0);
		enemyOAM[45] = (enemy[11].xPos <<0);
		enemyOAM[49] = (enemy[12].xPos <<0);
		enemyOAM[53] = (enemy[13].xPos <<0);
		enemyOAM[57] = (enemy[14].xPos <<0);
		enemyOAM[61] = (enemy[15].xPos <<0);
		enemyOAM[65] = (enemy[16].xPos <<0);
		enemyOAM[69] = (enemy[17].xPos <<0);
		enemyOAM[73] = (enemy[18].xPos <<0);
		enemyOAM[77] = (enemy[19].xPos <<0);
		
	}
	
	if(enemy[0].xPos < 1)
	{
		for(int i = 0; i < 20; i++)
		{
		enemy[i].yPos = enemy[0].yPos + 3;
		}
		enemyOAM[0] = (enemy[0].yPos<<0);
		enemyOAM[4] = (enemy[0].yPos<<0);
		enemyOAM[8] = (enemy[0].yPos<<0);
		enemyOAM[12] = (enemy[0].yPos<<0);
		enemyOAM[16] = (enemy[0].yPos<<0);
		enemyOAM[20] = (enemy[0].yPos<<0);
		enemyOAM[24] = (enemy[0].yPos<<0);
		enemyOAM[28] = (enemy[0].yPos<<0);
		enemyOAM[32] = (enemy[0].yPos<<0);
		enemyOAM[36] = (enemy[0].yPos<<0);
		enemyOAM[40] = (enemy[0].yPos<<0);
		enemyOAM[44] = (enemy[0].yPos<<0);
		enemyOAM[48] = (enemy[0].yPos<<0);
		enemyOAM[52] = (enemy[0].yPos<<0);
		enemyOAM[56] = (enemy[0].yPos<<0);
		enemyOAM[60] = (enemy[0].yPos<<0);
		enemyOAM[64] = (enemy[0].yPos<<0);
		enemyOAM[68] = (enemy[0].yPos<<0);
		enemyOAM[72] = (enemy[0].yPos<<0);
		enemyOAM[76] = (enemy[0].yPos<<0);
		enemy[0].moveRight = 1;
	}
	
	
	
	if(largeTimer >= (200 + (passCount * 10) ))
	{
		enemy[20].xPos--;
		largeEnemyOAM[1] = (enemy[20].xPos<<0);
		
	}
	
	if(enemy[20].xPos < 1)
	{
		passCount = passCount + 1;
		largeTimer = 0;
		enemy[20].xPos = 250;
		largeEnemyOAM[1] = (enemy[20].xPos<<0);
	}
	
	
	
	if(enemy[0].isFiring == 0)
	{
	
		bullet[1].xPos = enemy[0].xPos;
		bullet[1].yPos = enemy[0].yPos;
		
		enemyBulletOAM[0]=(bullet[1].yPos<<0);
		enemyBulletOAM[1]=(bullet[1].xPos<<0);
		enemy[0].isFiring = 1;

		
	}
	if(bullet[1].yPos <= 150)
	{		
		bullet[1].yPos++;
		enemyBulletOAM[0]=(bullet[1].yPos<<0);
		
	}

	
	if(bullet[1].yPos > 150)
	{
		enemy[0].isFiring = 0;
		bullet[1].yPos = 0;
		bullet[1].isDead = 0;
	}
	
	
short boxWidth = 8;
short boxHeight = 8;

short newX1 = player.xPos + (bullet[1].xPos - player.xPos);
short newY1 = player.yPos + (bullet[1].yPos - player.yPos);

if((newX1 >= (player.xPos-(boxWidth/2))) && (newX1 <= (player.xPos+(boxWidth/2))) && newY1 >= (player.yPos -(boxHeight/2))&& newY1 <= (player.yPos + (boxHeight/2)) && player.isBlocking == 0 && bullet[1].isDead == 0)
{
	bullet[1].isDead = 1 ;
	enemyBulletOAM[0] = (0<<0);
	loosealife(&lives);
	LivesMap[7] = ((lives)<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
	if(lives <= 27)
	{
	for(int i = 0; i <1024; i++)
	{
	sprite[i] = 0;	
	character[i] = 0;
	Scoremap[i] = 0;
	LivesMap[i] = 0;
	}
	gameOver = 1;
	}
}

if(enemy[0].yPos > 150 && enemy[0].isDead == 0)
{
	for(int i = 0; i <1024; i++)
	{
	sprite[i] = 0;	
	character[i] = 0;
	Scoremap[i] = 0;
	LivesMap[i] = 0;
	}
	gameOver = 1;
}
	
	 
		VBlankIntrWait();
		mmFrame(); //sound function call every frame
	}
}
}
}


