#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "util.hpp"
#include "sdl.hpp"

class MainMenu
{
    private:

    int selected_option;
    const int number_of_options;

    public:

    enum MENU_OPTION{
        NEW_GAME=0,
        EXIT=1
    };

    MainMenu();

    inline int getSelectedOption(){ return selected_option; }

    //shows the menu and return the selected option
    int show();

};

#endif // MAIN_MENU_HPP
