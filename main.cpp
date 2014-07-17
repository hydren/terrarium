#include "main.hpp"

MACETE_ESCROTO_1_1;

int main(int argc, char **argv)
{	
    try{
	// initialize SDL
	SDL::initialize();
	MACETE_ESCROTO_1_2;
	
    SDL::cycleFrequency = 200;

	SDL::Window::create(640, 480, 16);
    SDL::Window::setCaption("Game", "game");
	
    MainMenu mainMenu;
    mainMenu.show();

    if(mainMenu.getSelectedOption() == MainMenu::NEW_GAME)
    {
        Game game;
        game.start();
    }

	SDL::finalize();
	MACETE_ESCROTO_1_3;

    }catch(const Exception& e)
    {
        COUT << "Unhandled Exception! Exception: " << e.message() << "\nTerminating execution..." << ENDL;
    }

	return 0;
}
