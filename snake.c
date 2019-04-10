#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "lcd_lib.h"

#define MAX_LENGTH 100
#define MAX_WIDTH  84 
#define MAX_HEIGHT 48 
#define THICK      3

static int speed = 10;
enum direction_t {f, up , down ,left ,right};
static int direction = 0;
struct snake {
	int head ;
	int current_length;
	int head_x[4];
	int head_y[4];
	int arr_x[THICK][MAX_LENGTH];
	int arr_y[THICK][MAX_LENGTH];
	
};
void init(struct snake *s) {
	int i = 0,j = 0;
	int start_x_point =25;
	int start_y_point = 20;

	s->head = left;	
	s->current_length = 45;
	for (j = 0 ; j < THICK ; j ++) {	
		for( i = 0 ; i < s->current_length; i++) {
			s->arr_x[j][i]  = start_x_point + i ;

			if (j == 1) {
				s->arr_y[j][i]  = start_y_point;
			}
			else if (j == 0) {
				s->arr_y[j][i] = start_y_point - 1;
			}
			else if (j == 2 ) {
				s->arr_y[j][i] = start_y_point + 1;
			}
			else { 
				printf("init error\n");
				return ;
			}
			s->arr_y[j][i] = start_y_point ;
				
			
		}
	}

}
void draw_snake(struct snake s ){
	int i = 0,j = 0;
	
	for (j = 0; j < THICK; j++) {	
		for( i = 0 ; i < s.current_length; i++) {
			lcd_draw_pixel(s.arr_x[j][i], s.arr_y[j][i], Pixel_Set);
		}
	}
	
}

void draw_snake_move(struct snake *s,int lx[],int ly[], int dir ){
	int i = 0;
	
	//write new head'snake
	for (i = 0; i < THICK; i++) {
		lcd_draw_pixel(s->arr_x[i][0], s->arr_y[i][0], Pixel_Set);
	}

	//delete old tail'snake
	for (i = 0; i < THICK; i++) {
		lcd_draw_pixel(lx[i], ly[i], Pixel_Clear);
	}

		
}

void update_snake_arr_pos(struct snake *s) {
	int i  = 0,j=0;
	for (j = 0; j < THICK; j++) {
		for (i = s->current_length-1 ; i>0 ; i--){
			s->arr_x[j][i] = s->arr_x[j][i-1];
			s->arr_y[j][i] = s->arr_y[j][i-1];
		}
	}
	
}

void snake_move(struct snake *s , int d ) {
	static int last_state = 100;
	int i = 0;		
	int tx[THICK],ty[THICK]; 
	
	s->head = d;

	for (i = 0; i < THICK; i++) {
		tx[i] = s->arr_x[i][s->current_length-1];
		ty[i] = s->arr_y[i][s->current_length-1];
	}	

	update_snake_arr_pos(s);

	if( (d == left)  && (last_state != right)) {
		if (s->arr_y[2][2] == s->arr_y[1][2] ){
			s->arr_y[0][0] = s->arr_y[1][0]  ;
			s->arr_y[2][0] = s->arr_y[1][0] -1 ;
		}
		else if (s->arr_y[0][2] == s->arr_y[1][2]) {
			s->arr_y[2][0] = s->arr_y[1][0]  ;
			s->arr_y[0][0] = s->arr_y[1][0] + 1 ;
		}

		if (s->arr_x[1][0] -1 < 0) {
			for (i = 0; i < THICK; i++) 
				s->arr_x[i][0] = MAX_WIDTH;
		}
		else {
			for (i = 0; i < THICK; i++) 
				s->arr_x[i][0] -=1;
		}

		last_state = d;
	}
	else if( (d == right) && (last_state != left)) {
		if (s->arr_y[2][2] == s->arr_y[1][2] ){
			s->arr_y[2][0] = s->arr_y[1][0] - 1;
			s->arr_y[0][0] = s->arr_y[1][0]  ;
		}
		else if (s->arr_y[0][2] == s->arr_y[1][2]) {
			s->arr_y[2][0] = s->arr_y[1][0]  ;
			s->arr_y[0][0] = s->arr_y[1][0] +1;
		}

		if (s->arr_x[1][0] + 1 > MAX_WIDTH ) {
			for (i = 0; i < THICK; i++) 
				s->arr_x[i][0] = 0;
		}
		else {
			for (i = 0; i < THICK; i++) 
				s->arr_x[i][0] +=1;
		}

		last_state = d;
	}
	else if( (d == up)  && (last_state != down) ) {

		if (s->arr_x[2][2] == (s->arr_x[1][2])){
			s->arr_x[2][0] = s->arr_x[1][0] - 1  ;
			s->arr_x[0][0] = s->arr_x[1][0] ;
		}
		else if (s->arr_x[0][2] == s->arr_x[1][2] ) {
			s->arr_x[2][0] = s->arr_x[1][0] ;
			s->arr_x[0][0] = s->arr_x[1][0] +1;
		}

		if (s->arr_y[1][0] -1 < 0) {
			for (i = 0; i < THICK; i++) 
				s->arr_y[i][0] = MAX_HEIGHT;
		}
		else {
			for (i = 0; i < THICK; i++) 
				s->arr_y[i][0] -=1;
		}
		
		last_state = d;
	}
	else if( (d == down) && (last_state != up) ) {

		if (s->arr_x[2][2] == (s->arr_x[1][2])){
			s->arr_x[2][0] = s->arr_x[1][0] + 1  ;
			s->arr_x[0][0] = s->arr_x[1][0] ;
		}
		else if (s->arr_x[0][2] == s->arr_x[1][2] ) {
			s->arr_x[2][0] = s->arr_x[1][0] ;
			s->arr_x[0][0] = s->arr_x[1][0] -1;
		}


		if (s->arr_y[1][0] + 1 > MAX_HEIGHT) {
			for (i = 0; i < THICK; i++) 
				s->arr_y[i][0] = 0;
		}
		else {
			for (i = 0; i < THICK; i++) 
				s->arr_y[i][0] +=1;
		}
		
		last_state = d;
	}
	else {
		if( last_state == left) {
			if (s->arr_y[2][2] == s->arr_y[1][2] ){
				s->arr_y[0][0] = s->arr_y[1][0]  ;
				s->arr_y[2][0] = s->arr_y[1][0] -1 ;
			}
			else if (s->arr_y[0][2] == s->arr_y[1][2]) {
				s->arr_y[2][0] = s->arr_y[1][0]  ;
				s->arr_y[0][0] = s->arr_y[1][0] + 1 ;
			}
			if (s->arr_x[1][0] -1 < 0) {
				for (i = 0; i < THICK; i++) 
					s->arr_x[i][0] = MAX_WIDTH;
			}
			else {
				for (i = 0; i < THICK; i++) 
					s->arr_x[i][0] -=1;
			}
		
		}
		else if( last_state == right) {
			if (s->arr_y[2][2] == s->arr_y[1][2] ){
				s->arr_y[2][0] = s->arr_y[1][0] - 1;
				s->arr_y[0][0] = s->arr_y[1][0]  ;
			}
			else if (s->arr_y[0][2] == s->arr_y[1][2]) {
				s->arr_y[2][0] = s->arr_y[1][0]  ;
				s->arr_y[0][0] = s->arr_y[1][0] +1;
			}

			if (s->arr_x[1][0] + 1 > MAX_WIDTH ) {
				for (i = 0; i < THICK; i++) 
					s->arr_x[i][0] = 0;
			}
			else {
				for (i = 0; i < THICK; i++) 
					s->arr_x[i][0] +=1;
			}
			
		}
		else if(last_state == up ) {
			if (s->arr_x[2][2] == (s->arr_x[1][2])){
				s->arr_x[2][0] = s->arr_x[1][0] - 1  ;
				s->arr_x[0][0] = s->arr_x[1][0] ;
			}
			else if (s->arr_x[0][2] == s->arr_x[1][2] ) {
				s->arr_x[2][0] = s->arr_x[1][0] ;
				s->arr_x[0][0] = s->arr_x[1][0] +1;
			}
	
			if (s->arr_y[1][0] -1 < 0) {
				for (i = 0; i < THICK; i++) 
					s->arr_y[i][0] = MAX_HEIGHT;
			}
			else {
				for (i = 0; i < THICK; i++) 
					s->arr_y[i][0] -=1;
			}
		}
		else if(last_state == down) {
			if (s->arr_x[2][2] == (s->arr_x[1][2])){
				s->arr_x[2][0] = s->arr_x[1][0] + 1  ;
				s->arr_x[0][0] = s->arr_x[1][0] ;
			}
			else if (s->arr_x[0][2] == s->arr_x[1][2] ) {
				s->arr_x[2][0] = s->arr_x[1][0] ;
				s->arr_x[0][0] = s->arr_x[1][0] -1;
			}

			if (s->arr_y[1][0] + 1 > MAX_HEIGHT) {
				for (i = 0; i < THICK; i++) 
					s->arr_y[i][0] = 0;
			}
			else {
				for (i = 0; i < THICK; i++) 
					s->arr_y[i][0] +=1;
			}
		}
	}

	draw_snake_move(s,tx,ty,last_state);

}

void *thread_read_file(void *argv) {
	char buf[10];
	char c;

	while(1) {
		FILE *fp = fopen("/dev/buttons","r");
		fgets(buf,10,fp);
		direction = atoi(buf);
		fclose(fp);
		usleep(100);
	}
}

void *thread_control_snake(void *argv) {
	int i = 0;	
	struct snake my_snake;
	
	init(&my_snake);
	draw_snake(my_snake);
	lcd_send_buff();
	sleep(3);
	lcd_clear_screen();

	while (1) {
		snake_move(&my_snake, direction);			
		draw_snake(my_snake);
		lcd_send_buff();
		usleep(speed*1000);
	}	
}

int main(int argc , char *argv[]) {
	pthread_t thread_rd_file_id, thread_control_id;

	if( argc == 2 ){
		speed = atoi(argv[1]);
		printf("speed : %d\n" ,speed);
	}  

	pthread_create(&thread_rd_file_id, NULL, thread_read_file, NULL);
	pthread_create(&thread_control_id, NULL, thread_control_snake, NULL);

	while(1) {
	}
	return 0;
}
