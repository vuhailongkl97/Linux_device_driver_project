#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "lcd_lib.h"

#define MAX_LENGTH 100
#define MAX_WIDTH  84 
#define MAX_HEIGHT 48 

struct snake {
	int head ;
	int current_length;
	int head_x[4];
	int head_y[4];

};
struct food {
	int pos_x[10];
	int pos_y[10];
};
void create_new_food(struct snake s, struct food *f){
	int fx,fy,i, j, t =0;	
	srand(time(NULL));
	fx = rand() % (MAX_WIDTH +1);
	srand(time(NULL));
	fy = rand() % (MAX_HEIGHT +1);

	for (i =0; i< 9; i++) {
		lcd_draw_pixel(f->pos_x[i],f->pos_y[i], Pixel_Clear);	
	}
	
	for (i = 0;i < 3; i++){
		for (j = 0; j< 3;j ++){
			f->pos_x[t] = fx + i ;
			f->pos_y[t] = fy + j ;
			t ++;
		}
	}	
	
}
void draw_food(struct food f) {
	int i = 0;
	for (i =0; i< 9; i++) {
		lcd_draw_pixel(f.pos_x[i],f.pos_y[i], Pixel_Set);	
	}
}
struct snake my_snake;
struct food my_food;

int main(int argc , char *argv[]) {
	
	lcd_clear_screen();
	sleep(2);
	while (1) {
		create_new_food(my_snake, &my_food);
		draw_food(my_food);
		lcd_send_buff();
		usleep(10000);
	}	
	return 0;
}

