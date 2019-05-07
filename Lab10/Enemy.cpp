#include "Enemy.h"




void Enemy::initialize_enemies(){
	int xstart=0;
	int ystart=16;
	rightmost=3;
	remaining=12;
	score=0;
	leftmost=0;
	upmost=0;
	downmost=2;
	attack_count=10;
	flag=0;
	for(int i=0;i<4;i++)
	{ 
		for(int j=0;j<3;j++){
			enemyblock[j][i].x=xstart;
			enemyblock[j][i].y=ystart;
			enemyblock[j][i].image=SmallEnemy10pointA;
			enemyblock[j][i].life=alive;
		
	   ystart=ystart+16;
		
		}
		xstart=xstart+25;
		ystart=16;
	
	}
  for(int i=0;i<4;i++)
	{ enemy_missiles[i].image=enemyfire;
		enemy_missiles[i].life=dead;
		enemy_missiles[i].x=0;
		enemy_missiles[i].y=0;
	
	}

}
void Enemy::displayenemies(){

for(int i=0;i<4;i++)
	{
		
	for(int j=0;j<3;j++)
	{	if(enemyblock[j][i].life==alive)
			{ST7735_DrawBitmap(enemyblock[j][i].x, enemyblock[j][i].y, enemyblock[j][i].image, 16, 10);}
	}

}
}
void Enemy::clear_enemy(){
	for(int j=0;j<3;j++)
	{
 for(int i=0;i<4;i++)
	{ if(enemyblock[j][i].life==alive)
			ST7735_DrawBitmap(enemyblock[j][i].x, enemyblock[j][i].y, clearenemy, 16, 10);
	}
}
}
void Enemy::move_down_helper(){
clear_enemy();
for(int i=0;i<4;i++){
for(int j=0;j<3;j++){

	enemyblock[j][i].y+=16;
	
}
}

}
void Enemy::motion(){
	
for(int i=0;i<4;i++){
for(int j=0;j<3;j++){
	

			if(flag==0)
					{
					if(enemyblock[j][rightmost].x<110)
					{ enemyblock[j][i].x++;
					}
					else
					{ move_down_helper();
						flag=1;
						return;
					}
				}
					else
					{	if(enemyblock[j][leftmost].x>1)
						{enemyblock[j][i].x--;
						}
						else
						{move_down_helper();
							flag=0;
							return;
						}
				}
		
			}

}



}
int Enemy::leftmostupdate(){
	
for(int j=leftmost;j<=rightmost;j++)
{	
for(int i=0;i<3;i++)
	{
		if(enemyblock[i][j].life==alive)
			return j;
	
	}

}
}
int Enemy::rightmostupdate(){

	for(int j=rightmost;j>=leftmost;j--)
	{
	for(int i=0;i<3;i++)
	{
		if(enemyblock[i][j].life==alive)
			return j;
	
	}
}
	
}

void Enemy::lowestupdate(void){
for(int i=0;i<4;i++)
	{
	  if(enemyblock[downmost][i].life==alive)
		 return;
		
	
	}
downmost--;
}

void Enemy::moveattack(void){
 ST7735_DrawBitmap(enemy_missiles[0].x, enemy_missiles[0].y, enemyfire, 10, 16);
	enemy_missiles[0].y++;
	if(enemy_missiles[0].y>159)
	{enemy_missiles[0].life=dead;
	  ST7735_DrawBitmap(enemy_missiles[0].x, enemy_missiles[0].y, clear, 10, 16);
	
	}
}

