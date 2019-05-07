#include <stdint.h>

#include "Images.h"
#include "ST7735.h"
class Enemy{
	public:
typedef enum {dead,alive} status_t;
struct sprite{
  uint32_t x;      // x coordinate
  uint32_t y;      // y coordinate
  const unsigned short *image; // ptr->image
  status_t life;            // dead/alive
};  
typedef struct sprite sprite_t;

	public:
		int flag;
	sprite_t enemyblock[3][4];
	sprite_t enemy_missiles[5];
  int remaining;
	int score;
	int rightmost;
	int leftmost;
	int upmost;
	int downmost;
	int attack_count;
	void clear_enemy(void);
	void initialize_enemies(void);
	void displayenemies(void);
	void motion(void);
	void move_down_helper(void);
	int leftmostupdate(void);
	int rightmostupdate(void);
	void lowestupdate(void);
  void moveattack(void);
	
};
