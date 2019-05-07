#include "ground_fire.h"

void ground_fire::defense(void){

bunker1.x=32;
bunker1.y=130;
bunker1.image=bunker;
bunker1.life=alive;
bunker2.x=75;
bunker2.y=130;
bunker2.image=bunker;
bunker2.life=alive;
int dataar[13*17]={
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
  int k=13*17-1;
	for(int i=0;i<13;i++)
	{for(int j=0;j<17;j++)
		{ bunkerdmg1[i][j]=dataar[k];
			bunkerdmg2[i][j]=dataar[k];

			k--;
		}
	}
	
}

void ground_fire::missile_propagate(void){

	ST7735_DrawBitmap(ground_fire.x, ground_fire.y, shortgreen, 6, 14);
	ground_fire.y=ground_fire.y-1; 
	
	if(ground_fire.y==0)
	{ground_fire.life=dead;
	}
}
