SRC= skein_block.o skein.o xkcd.o SHA3api_ref.o
CFLAGS = -std=c99 -Ofast
all: xkcd

xkcd: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
	rm -f xkcd
