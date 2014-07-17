TEMPLATE = app
CONFIG += console
CONFIG -= qt

win32: LIBS += -lmingw32 -lSDLmain -lSDL
unix: LIBS += `sdl-config --cflags --libs`
LIBS += -lSDL_image -lSDL_mixer -lSDL_ttf

SOURCES += main.cpp \
    util.cpp \
    sdl.cpp \
    entity.cpp \
    block.cpp \
    animated_entity.cpp \
    actor.cpp \
    game.cpp \
    map.cpp \
    character.cpp \
    pc.cpp \
    main_menu.cpp

HEADERS += \
    util.hpp \
    texture.hpp \
    sdl.hpp \
    main.hpp \
    entity.hpp \
    block.hpp \
    animated_entity.hpp \
    actor.hpp \
    game.hpp \
    map.hpp \
    character.hpp \
    pc.hpp \
    main_menu.hpp
