VPATH = src:include:src/utils
CC = g++
CFLAG = -I include
# Replace this with the path to your allegro install
PATH_TO_ALLEGRO = [replace this]
LIBS = -I $(PATH_TO_ALLEGRO)/include/allegro5 \
       -L $(PATH_TO_ALLEGRO)/lib/ -lallegro   \
       -lallegro_image -lallegro_font         \
       -lallegro_primitives -lallegro_ttf     \
       -lBox2D
OBJ = animation.o     \
      block.o         \
      engine.o        \
      entity.o        \
      file_parser.o   \
      game.o          \
      main.o          \
      main_menu.o     \
      map.o           \
      menu.o          \
      physics.o       \
      util.o          \
      util_exception.o\
      util_math.o     \
      util_string.o
HEADERS = $(wildcard include/*.hpp)

all : terrarium

%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAG)

terrarium : $(OBJ) $(HEADERS)
	$(CC) -o terrarium $(OBJ) $(LIBS)

.PHONY : clean
clean :
	rm terrarium $(OBJ)
