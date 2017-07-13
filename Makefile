CC = g++
CFLAGS = -shared -fpic -lm -ldl -o

all:
	$(CC) $(CFLAGS) libid3.so id3v2.cpp

install:
	@install libid3.so /usr/lib
	@install libid3.so /usr/local/lib
	@install id3v2.h /usr/include

clean:
	rm -rf libid3.so
