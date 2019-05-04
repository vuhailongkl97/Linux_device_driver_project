#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>

#include "lcd_lib.h"

#define MY_SIGNAL		40

#define MAX_LENGTH 100
#define MAX_WIDTH  84 
#define MAX_HEIGHT 48 
#define THICK      3
#define WIDTH_FOOD  3
#define HEIGHT_FOOD  3
#define MODE_CONTROL_BUTTON  1
#define MODE_CONTROL_MPU  2

#define mpu_file "/dev/mpu6050"
#define buzzer_file "/dev/buzzer"

struct mpu_xyz {
	int x;
	int y;
	int z;
};

static int speed = 10;
//enum direction_t {zeros, up , down ,left ,right};
enum direction_t {zeros, down, up, left, right};
static int direction = 0;

struct snake {
	int head ;
	int current_length;
	int head_x[4];
	int head_y[4];
	int arr_x[THICK][MAX_LENGTH];
	int arr_y[THICK][MAX_LENGTH];
	
};
struct food {
	int pos_x[10];
	int pos_y[10];
};
void draw_score(struct snake s);
void init(struct snake *s);
void *thread_read_file(void *ar) {
	struct mpu_xyz mpu;
	int x ,y, z, pitch, roll;
	FILE *fp = NULL;
	float range_per_digit = .000061f;
	while(1) { 
			fp = fopen(mpu_file,"r");
			if (fp == NULL) {
				perror("fopen");
				//return NULL;
			}
			else {
				fread(&mpu, sizeof(struct mpu_xyz), 1, fp);		
				x = mpu.x * range_per_digit * 9.80665f ;
				y = mpu.y * range_per_digit * 9.80665f;
				z = mpu.z * range_per_digit * 9.80665f;
				pitch = -(atan2(x, sqrt(y*y + z*z))*180.0)/M_PI;
				roll = (atan2(y, z)*180.0)/M_PI;	
				//printf("x :%8d y :%8d z: %8d\n", x, y, z);
				if ( pitch < -70 && roll > 15  && roll < 60) {
					//printf("right\n");
					direction = right;
				}
				else if( pitch  < -40 && roll > 70) {
				//	printf("left\n");
					direction= left;
				}
				else if( pitch  < -70 && roll < 20) {
					//printf("up\n");
					direction = up;
				}
				else if( pitch   > -45 && roll < 10) {
					//printf("down\n");
					direction = down;
				}
				else {
				//	printf("pitch :%5d roll :%5d\n", pitch, roll);
				}
			}
			fclose(fp);
			usleep(20000);
		}	
	return NULL;
}
void create_new_food(struct snake s, struct food *f){
	int fx,fy,i, j, t =0;	

	while(1) {
		srand(time(NULL));
		fx = (rand() % (MAX_WIDTH -5)) +2 ;
		srand(time(NULL));
		fy = (rand() % (MAX_HEIGHT -5)) +2 ;
		for (i = 0;i < THICK; i++) {
			for (j =1; j < s.current_length; j++) {
				if ((fx == s.arr_x[i][j]) && (fy == s.arr_y[i][j])){
					goto next;					
				}
			}
		} 
		break;	
		next: ;
	
	}
	for (i =0; i< (WIDTH_FOOD * HEIGHT_FOOD); i++) {

		lcd_draw_pixel(f->pos_x[i],f->pos_y[i], Pixel_Clear);	
	}
	
	for (i = 0;i < WIDTH_FOOD; i++){
		for (j = 0; j< HEIGHT_FOOD;j ++){
			f->pos_x[t] = fx + i ;
			f->pos_y[t] = fy + j ;
			t ++;
		}
	}	
	
}
void draw_food(struct food f) {
	int i = 0;
	for (i =0; i< (WIDTH_FOOD * HEIGHT_FOOD); i++) {
		lcd_draw_pixel(f.pos_x[i],f.pos_y[i], Pixel_Set);	
	}
}
void signal_handle(int n, siginfo_t *info, void* data)
{
	direction = info->si_int;
}
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

void draw_snake_move(struct snake *s,int lx[],int ly[], int dir,int clear_tail ){
	int i = 0;
	
	//write new head'snake
	for (i = 0; i < THICK; i++) {
		lcd_draw_pixel(s->arr_x[i][0], s->arr_y[i][0], Pixel_Set);
	}

	//delete old tail'snake
	if( clear_tail ) {
		for (i = 0; i < THICK; i++) {
			lcd_draw_pixel(lx[i], ly[i], Pixel_Clear);
		}
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

void snake_move(struct snake *s , int d ,struct food *f) {
	static int last_state = 100;
	int pid; //new_1
	int del_tail = 1 ;
	int i = 0,j;		
	FILE *fp ;
	int tx[THICK],ty[THICK]; 
	
	s->head = d;
	for (i = 0; i < THICK; i++) 
		for (j = 5; j < s->current_length; j++) {
			if( (s->arr_x[i][0] == s->arr_x[i][j]) && (s->arr_y[i][0] == s->arr_y[i][j])) {
				lcd_clear_screen();
				init(s);			
				create_new_food(*s, f);
				draw_food(*f);
				lcd_send_buff();
				printf("collison:%d %d\n",i, s->arr_x[i][0]);
				return;
			}
		}
	/*check food*/	
	for (i = 0; i < THICK; i++) {
		for (j =0; j< (WIDTH_FOOD * HEIGHT_FOOD); j++) {
			if( (f->pos_x[j] == s->arr_x[i][0]) && (f->pos_y[j] == s->arr_y[i][0])) {
				//need fork() update
				pid = fork();
				if (pid < 0) {
					perror("failt fork\n");
					exit(-1) ;
				} 
				else if (pid == 0 ){
					pid = fork();	
					if (pid == 0) {
						fp = fopen(buzzer_file, "r+");
						fwrite("1",1,1,fp);
						fclose(fp);
						usleep(100000);
						fp = fopen(buzzer_file, "r+");
						fwrite("0",1,1,fp);
						fclose(fp);
						exit(0);
					}
					else if (pid > 0) {
						exit(0) ;
					}
					else {
						perror("failt fork 2\n");
						exit(0) ;
					}
				}
				wait(NULL);
				del_tail = 0;	
				s->current_length ++;
				s->arr_x[i][0] = f->pos_x[j];
				s->arr_y[i][0] = f->pos_y[j];
				draw_score(*s);
				create_new_food(*s, f);
				draw_food(*f);
				break;
			}	
		}
		
	}	

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

	draw_snake_move(s,tx,ty,last_state,del_tail);

}

void draw_score(struct snake s){
	char buf[10];
	lcd_gotoxy(65,40);		
	sprintf(buf,"%d",s.current_length);
	draw_string(buf,Pixel_Set ,FontSize_5x7);
	
}
void *thread_control(void *ar) {
	struct snake my_snake;
	struct food my_food;

	lcd_clear_screen();
	create_new_food(my_snake, &my_food);
	draw_food(my_food);
	init(&my_snake);
	draw_snake(my_snake);
	lcd_send_buff();
	sleep(2);

	while (1) {
		snake_move(&my_snake, direction,&my_food);			
		draw_snake(my_snake);
		lcd_send_buff();
		usleep(speed*1000);
	}	
	
	return NULL;
}
int main(int argc , char *argv[]) {
	struct sigaction sig;
	int mode_game = MODE_CONTROL_MPU;
	pthread_t  tid1,tid2;
	if( argc == 2 ){
		speed = atoi(argv[1]);
		printf("speed : %d\n" ,speed);
	}
	if(argc == 3) {
		if (strcmp(argv[2],"button") == 0){
			mode_game = MODE_CONTROL_BUTTON;	
		}
	} 

	printf("process id is %d\n", getpid());
	sig.sa_sigaction = signal_handle;
	sig.sa_flags = SA_SIGINFO;
	sigaction(MY_SIGNAL, &sig, NULL);


	pthread_create(&tid1, NULL, thread_read_file, NULL);	

	if( mode_game == MODE_CONTROL_BUTTON) {
		pthread_create(&tid2, NULL, thread_control, NULL);	
		pthread_join(tid2, NULL);
	}
	pthread_join(tid1, NULL);

	return 0;
}
