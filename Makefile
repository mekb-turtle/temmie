CC=cc
CFLAGS=-Wall -O2
LFLAGS=-lm -s -lSDL2

IMGS=temmie.h
OBJS=temmie.o

TARGET=temmie

.PHONY: all clean

all: $(TARGET)

%.h: %.ff
	cat > $@ <<<"#include <stdint.h>"$$'\n'"uint8_t data_$(@:%.h=%)[] = {"$$'\n'"$$(od -vAn -tx1 < $< | sed "s/ /,0x/g;s/^,//;s/$$/,/")};"

$(TARGET): $(IMGS) $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fv -- $(OBJS) $(IMGS) $(TARGET)
