CC = gcc
CFLAGS = -Wall -g
OBJS = ship.o main.o panic.o util.o beam.o list.o
OUTPUT = starone

LIBS += -lm -ldl `sdl-config --libs` -lGL -lGLU -lGLEW -lpng
CFLAGS += `sdl-config --cflags` # -DWRITE_PPM

$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)

$(OBJS):
	$(CC) -c $(CFLAGS) $(subst $(OBJDIR)/,, $*.c) -o $*.o

clean:
	find \( -name core -o -name '*.o' -o -name '*.a' \) -exec rm -f {} \;
	rm $(OUTPUT)

main.o:	main.c
panic.o: panic.c
ship.o: ship.c
util.o: util.c
beam.o: beam.c
list.o: list.c
