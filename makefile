CC = g++
CFLAGS = $(shell xml2-config --cflags)

SDLLIB = $(shell sdl-config --libs)


LIBS = $(SDLLIB) -lGL -lm -lGLU -lSDL_image $(shell xml2-config --libs)
NAME = space

OBJECTS = \
	./src/main.o \
	./src/stm.o \
	./src/objects/planets.o\
	./src/objects/suns.o
	

ALL: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBS)
	
TEST: ALL
	./$(NAME)

CLEAN:
	rm $(OBJECTS) 

CLEAN2: CLEAN
	rm $(NAME)
NEW:
	make CLEAN -i
	make ALL      
       
%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
