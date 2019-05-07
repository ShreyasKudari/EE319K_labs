#include <stdint.h>
#include "Images.h"
#include "ST7735.h"
class ground_fire{
public:
typedef enum {dead,alive} status_t;
struct sprite{
  uint32_t x;      // x coordinate
  uint32_t y;      // y coordinate
  const unsigned short *image; // ptr->image
  status_t life;            // dead/alive
};  
typedef struct sprite sprite_t;
  sprite_t bunker1;
	sprite_t bunker2;

  int bunkerdmg1[13][18];
	int bunkerdmg2[13][18];

	sprite_t ground_fire;
	void missile_propagate();
	void defense();

};