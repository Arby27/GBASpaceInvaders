#ifndef _collision
#define _collision


#include <stdio.h>

int collision(int bulletX, 
int bulletY, 
int targetX, 
int targetY,
int boxWidth,
int boxHeight)
{


short newX = targetX + (bulletX - targetX);
short newY = targetY + (bulletY - targetY);

if((newX >= (targetX-(boxWidth/2))) && (newX <= (targetX+(boxWidth/2))) && newY >= (targetY -(boxHeight/2))&& newY <= (targetY + (boxHeight/2)))
{
	
	return 1;
}

else
{
	return 0;
}
}

#endif