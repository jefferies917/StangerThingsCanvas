##########################################################################


#DEBUG  = -g -O0
DEBUG   = -O3
CC  = gcc
INCLUDE = -I/usr/local/include
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lpthread -lm -lwiringPiPca9685

SRC =   alpha_writer.c

OBJ =   $(SRC:.c=.o)

BINS=   $(SRC:.c=)

all:    $(BINS)

alpha: alpha_writer.o
	@echo [link]
	@$(CC) -o alpha_writer.o $(LDFLAGS) $(LDLIBS)

