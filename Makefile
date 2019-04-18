DRIVER_DIR = ./driver
LIB_DIR = ./lib
USER_DIR = ./user_main

export DIR=$(PWD)

all:
	make -C $(DRIVER_DIR)
	make -C $(LIB_DIR)
	make -C $(USER_DIR)

clean:
	make -C $(DRIVER_DIR) clean
	make -C $(LIB_DIR) clean
	make -C $(USER_DIR) clean
