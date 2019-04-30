
INCLUDE_DIR=../lib
CFLAGS=-I$(INCLUDE_DIR) -Wall
LDFLAGS=-L$(INCLUDE_DIR) -llcd_lib

TARGET=test_lcd
CC=gcc

all:
	$(CC) test_lcd.c -o $(TARGET) $(CFLAGS) $(LDFLAGS)
	$(CC) logo.c -o ltest $(CFLAGS) $(LDFLAGS)


clean:
	rm $(TARGET) 

