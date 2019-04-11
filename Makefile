obj-m := lcd_driver.o
lcd_driver-objs := lcd_ctrl.o lcd_5110.o

obj-m += button_ctl.o

snake: 
	gcc snake.c lcd_lib.c -o testp -lpthread
food: 
	gcc food.c lcd_lib.c -o tf 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	make -C lib/
	make -C user_main/
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	make -C lib/ clean
	make -C user_main/ clean
