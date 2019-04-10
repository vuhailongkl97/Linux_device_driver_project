
INCLUDE_DIR=../lib
CFLAGS=-I$(INCLUDE_DIR) -Wall
LDFLAGS=-L$(INCLUDE_DIR) -llcd_lib

TARGET=test
CC=gcc

all:
	$(CC) test_lcd.c -o test $(CFLAGS) $(LDFLAGS)

clean:
	rm $(TARGET) 

