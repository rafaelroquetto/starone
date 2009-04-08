CC = gcc
CFLAGS = -Wall -g
OBJS = asteroid.o ship.o main.o panic.o util.o beam.o list.o image.o gl_util.o
OUTPUT = starone

LIBS += -lm -ldl `sdl-config --libs` -lGL -lGLU -lpng
CFLAGS += `sdl-config --cflags` # -DWRITE_PPM

$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)

$(OBJS):
	$(CC) -c $(CFLAGS) $(subst $(OBJDIR)/,, $*.c) -o $*.o

clean:
	find \( -name core -o -name '*.o' -o -name '*.a' \) -exec rm -f {} \;
	rm $(OUTPUT)

asteroid.o: asteroid.c
main.o:	main.c
panic.o: panic.c
ship.o: ship.c
util.o: util.c
beam.o: beam.c
list.o: list.c
image.o: image.c
gl_util.o: gl_util.c
