#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define mpu_file "/dev/mpu6050"

struct mpu_xyz {
	int x;
	int y;
	int z;
};

int main(){
	struct mpu_xyz mpu;
	int x ,y, z, pitch, roll;
	FILE *fp = NULL;
	float range_per_digit = .000061f;
	
	while(1) { 
			fp = fopen(mpu_file,"r");
			if (fp == NULL) {
				perror("fopen");
				return -1;
			}
			else {
				fread(&mpu, sizeof(struct mpu_xyz), 1, fp);		
				x = mpu.x * range_per_digit * 9.80665f ;
				y = mpu.y * range_per_digit * 9.80665f;
				z = mpu.z * range_per_digit * 9.80665f;
				pitch = -(atan2(x, sqrt(y*y + z*z))*180.0)/M_PI;
				roll = (atan2(y, z)*180.0)/M_PI;	
				//printf("x :%8d y :%8d z: %8d\n", x, y, z);
				if ( pitch < -70 && roll > 15  && roll < 60) 
					printf("right\n");
				else if( pitch  < -40 && roll > 70) 
					printf("left\n");
				else if( pitch  < -70 && roll < 20) 
					printf("up\n");
				else if( pitch   > -45 && roll < 10) 
					printf("down\n");
				else {
					printf("pitch :%5d roll :%5d\n", pitch, roll);
				}
			}
			fclose(fp);
			sleep(1);
		}	
	return 0;
}
