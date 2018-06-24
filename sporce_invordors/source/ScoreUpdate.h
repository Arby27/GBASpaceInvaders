#ifndef _Score_Update
#define _Score_Update


#include <stdio.h>
extern int scoreIncrease(int* score, int* tens, int* hundreds);

void scoreUpdate(int* score,
 int* tens,
 int* hundreds,
 int *bulletIsDead,
 unsigned short* OAM, 
 unsigned short* scoremap1,
 unsigned short* scoremap2,
 unsigned short* scoremap3 )
{
	
	
	*OAM = 128;
	*bulletIsDead = 1;

	scoreIncrease(score, tens,hundreds);
	*scoremap1 = (*hundreds<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
	*scoremap2 = (*tens<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
	*scoremap3 = (*score<<0)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
	
	
}
#endif