#include "main_menu.hpp"

MainMenu::MainMenu() :
    selected_option(0),
    number_of_options(2)
{}

int MainMenu::show()
{
    SDLFont font("resources/fonts/liberation.ttf");
    font.renderingMode = SDLFont::BLENDED;
    font.isBold = true;
    font.setFontSize(28);

    SDLTexture option_img_newgame = font.renderTexture("New game", SDL::getColor(SDL::Color::AZURE) );
    SDLTexture option_img_exit = font.renderTexture("Exit", SDL::getColor(SDL::Color::AZURE) );
    SDLTexture bg("resources/images/background.jpg");

    font.setFontSize(48);
    font.isItalic = true;
    SDLTexture ui_title(font.renderTexture("Project Terrarium", SDL::getColor(SDL::Color::ORANGE) ) );

    SDLTexture selection_img(SDL_CreateRGBSurface(0, SDL::screen->w, font.getMaximumPixelHeight(), 16, 0,0,0,0));
    selection_img.fillWithColor(SDL::getColorNumber(SDL::Color::GOLD));

    SDL_Event event;
    bool decided=false;
    while(!decided)
    {
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_UP:
                        {
                            if(selected_option == 0) selected_option = number_of_options-1;
                            else selected_option--;
                            break;
                        }
                        case SDLK_DOWN:
                        {
                            if(selected_option == number_of_options-1) selected_option = 0;
                            else selected_option++;
                            break;
                        }
                        case SDLK_RETURN:
                        {
                            decided=true;
                            break;
                        }
                        default: break;
                    }
                    //COUT << "Selected option: " << selected_option << ENDL;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    switch (event.button.button)
                    {
                        case 1: //LEFT MOUSE BUTTON
                        {
                            //TODO select option area
                            break;
                        }
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    //TODO select mouse option
                    break;
                }
                case SDL_QUIT:
                {
                    // later this is supposed to ask if the user is sure to leave
                    exit(0);
                }
            }
        }

        //RENDER MENU

        SDL::Window::clean();

        bg.render();

        ui_title.render(25, 25);

        selection_img.render(0, 75 + (selected_option+1)*font.getMaximumPixelHeight());

        option_img_newgame.render(150, 75 + 1 * font.getMaximumPixelHeight());
        option_img_exit.render(150, 75 + 2 * font.getMaximumPixelHeight());

        SDL::Window::refresh();
        SDL::rest(100);
    }

    return selected_option;
}
