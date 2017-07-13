CC = g++
CFLAGS = -O3 -lid3 -o
SRC = mp3info.cpp

all: mp3info

mp3info: 
	$(CC) $(SRC) $(CFLAGS) mp3info

install:
	@install mp3info /usr/bin

clean:
	rm -rf mp3info
