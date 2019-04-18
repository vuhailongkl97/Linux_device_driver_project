obj-m := lcd_driver.o
lcd_driver-objs := lcd_ctrl.o lcd_5110.o

obj-m += button_ctl.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(DIR)/driver modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(DIR)/driver clean
