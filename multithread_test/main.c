#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


static int direction = 0;

void *thread_read_file(void *argv) {
	char buf[10];
	char c;

	while(1) {
		FILE *fp = fopen("/dev/buttons","r");
		fgets(buf,10,fp);
		printf("\nread file /dev/buttons %d\n",atoi(buf));
		fclose(fp);
		sleep(1);
	}
}

void *thread_control_snake(void *argv) {
	
	while(1) {
		printf("control snake with direction from thread read file : %d \n",direction);
		sleep(2);
	}
	
}

int main(){
	int i ;
	pthread_t thread_rd_file_id, thread_control_id;

	pthread_create(&thread_rd_file_id, NULL, thread_read_file, NULL);
	pthread_create(&thread_control_id, NULL, thread_control_snake, NULL);

	while(1) {

	}
	
}
