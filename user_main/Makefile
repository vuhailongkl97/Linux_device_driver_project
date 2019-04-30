
INCLUDE_DIR=../lib
CFLAGS=-I$(INCLUDE_DIR) -Wall -lm -pthread
LDFLAGS=-L$(INCLUDE_DIR) -llcd_lib

SOURCES = $(shell echo *.c)

TARGET=snake
CC=gcc

all:
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(TARGET) 

