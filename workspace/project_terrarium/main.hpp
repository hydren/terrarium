#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#ifdef _WIN32
    #define MACETE_ESCROTO_1_0 static FILE* MACETE_CONSOLE_STREAM_FILE;
    #define MACETE_ESCROTO_1_1 FILE* Main::MACETE_CONSOLE_STREAM_FILE;
    #define MACETE_ESCROTO_1_2 Main::MACETE_CONSOLE_STREAM_FILE = fopen("CON", "w" ); freopen( "CON", "w", stdout ); freopen( "CON", "w", stderr );
    #define MACETE_ESCROTO_1_3 fclose(Main::MACETE_CONSOLE_STREAM_FILE);
#else
    #define MACETE_ESCROTO_1_0
    #define MACETE_ESCROTO_1_1
    #define MACETE_ESCROTO_1_2
    #define MACETE_ESCROTO_1_3
#endif

#include <cstdlib>

#include "util.hpp"
#include "sdl.hpp"

class Main
{
    public:

    //para mostrar no terminal
    MACETE_ESCROTO_1_0;
	
	static int menu();

};




#endif // MAIN_HPP_INCLUDED
