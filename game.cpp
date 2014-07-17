#include "game.hpp"

Game::Game() :
    cycleFrequency(32),
    gravityConstant(1),
    lastTimeRendered(0),
    averageRenderingTime(0)
{
    //ALL INITIALIZATION OF A GAME MATCH
}

int Game::getGravityConstant() { return this->gravityConstant; }
void Game::setGravityConstant(const int& newConstant) { this->gravityConstant = newConstant; }

void Game::retile(Map& map, Block* b, bool recursive)
{
    int k=0;
    bool hasLeft=false, hasRight=false, hasNorth=false, hasSouth=false;

    //WIP tileset algorithm

    if(b->x_grid_pos > 0) if(map.grid[b->x_grid_pos-1][b->y_grid_pos] != NULL) hasLeft = true;

    if(b->x_grid_pos < (int) map.grid.size() -1) if(map.grid[b->x_grid_pos+1][b->y_grid_pos] != NULL) hasRight = true;

    if(b->y_grid_pos > 0) if(map.grid[b->x_grid_pos][b->y_grid_pos-1] != NULL) hasNorth = true;

    if(b->y_grid_pos < (int) map.grid[0].size() -1) if(map.grid[b->x_grid_pos][b->y_grid_pos+1] != NULL) hasSouth = true;

    if(  hasNorth &&  hasSouth &&  hasLeft &&  hasRight) k = 0;
    if( !hasNorth &&  hasSouth &&  hasLeft &&  hasRight) k = 1;
    if(  hasNorth && !hasSouth &&  hasLeft &&  hasRight) k = 2;
    if(  hasNorth &&  hasSouth && !hasLeft &&  hasRight) k = 3;
    if(  hasNorth &&  hasSouth &&  hasLeft && !hasRight) k = 4;
    if( !hasNorth &&  hasSouth && !hasLeft &&  hasRight) k = 5;
    if(  hasNorth && !hasSouth && !hasLeft &&  hasRight) k = 6;
    if( !hasNorth &&  hasSouth &&  hasLeft && !hasRight) k = 7;
    if(  hasNorth && !hasSouth &&  hasLeft && !hasRight) k = 8;
    if(  hasNorth &&  hasSouth && !hasLeft && !hasRight) k = 9;
    if( !hasNorth && !hasSouth &&  hasLeft &&  hasRight) k = 10;
    if( !hasNorth &&  hasSouth && !hasLeft && !hasRight) k = 11;
    if(  hasNorth && !hasSouth && !hasLeft && !hasRight) k = 12;
    if( !hasNorth && !hasSouth && !hasLeft &&  hasRight) k = 13;
    if( !hasNorth && !hasSouth &&  hasLeft && !hasRight) k = 14;
    if( !hasNorth && !hasSouth && !hasLeft && !hasRight) k = 15;

    //COUT << "hasNorth = " << hasNorth << ", hasSouth = " << hasSouth << ", hasLeft = " << hasLeft << ", hasRight = " << hasRight << ENDL;

    b->animation.framesOf[k] = 1;
    b->animation.intervalOf[k] = 1000;
    b->animation.setCurrent(k);

    if(recursive)
    {
        if(hasNorth) retile(map, map.grid[b->x_grid_pos][b->y_grid_pos-1], false);
        if(hasSouth) retile(map, map.grid[b->x_grid_pos][b->y_grid_pos+1], false);
        if(hasLeft)  retile(map, map.grid[b->x_grid_pos-1][b->y_grid_pos], false);
        if(hasRight) retile(map, map.grid[b->x_grid_pos+1][b->y_grid_pos], false);
    }

}

Map* Game::loadMapFromFile(String filename)
{
    FILE* file = Util::loadFile(filename);
    Map* map = loadMapFromFile(file);
    Util::closeFile(file);
    return map;
}

Map* Game::loadMapFromFile(FILE* file)
{
    Map* map=NULL;

    //TODO all loading of info about the map;

    //PROTOTYPE
    std::vector< std::vector<int> > matrix;

    char c = fgetc(file);
    String str="";

    if( c != EOF ) do
    {
        if(c == ':')
        {
            c = fgetc(file);
            if( c == '\n') c = fgetc(file);
            if( c == EOF ) break;
        }
        //COUT << "hAS THANG!" << ENDL;
        //std::vector<int> vec;
        matrix.push_back( std::vector<int>() );
        do{
            if(c == ',') c = fgetc(file);
            //COUT <<"CATCHING WORD, ";
            do{
                //COUT<<" DA cHAR: " << c << ENDL;
                str += c;
                c = fgetc(file);
            }while( c >= '0' && c <= '9' );
            //COUT << "PUTTING DA NUMBER MADE BY: " << str << ", its "<< atoi(str.c_str()) << ENDL;
            matrix.back().push_back( atoi(str.c_str()) );
            str="";
        }while(c == ',');
    }
    while(c == ':');

    /*
    int mapwidth = matrix.size(), mapheight=0;

    for(unsigned int i=0; i<matrix.size(); i++)
    {
        for(unsigned int j=0; j<matrix[i].size(); j++)
        {
            COUT << matrix[i][j] << ',';
        }
        COUT << '\n';
    }
    COUT << ENDL;
*/
    std::vector< std::vector<int> > matrix_corrected;

    for(unsigned int i = 0 ; i<matrix[0].size() ; i++)
    {
        matrix_corrected.push_back( std::vector< int >());
        for(unsigned int j = 0 ; j<matrix.size() ; j++)
        {
            matrix_corrected[i].push_back( matrix[j][i] );
        }
    }
/*
    for(unsigned int j=0; j<matrix_corrected[0].size(); j++)
    {
        for(unsigned int i=0; i<matrix_corrected.size(); i++)
        {
            COUT << matrix_corrected[i][j] << ',';
        }
        COUT << '\n';
    }
    COUT << ENDL;

    COUT << "Width: " << matrix_corrected.size() << ", Height: " << matrix_corrected[0].size() << ENDL;
*/
    //COUT << "ALL OK!" << ENDL;

    SDLTexture* bg=NULL;
    SDLTexture* tex=NULL;

    try
    {
        bg = new SDLTexture("../project/images/background.jpg");
    }
    catch(Exception e) { COUT << e.message() << ENDL; }

    tex = new SDLTexture("../project/images/tileset-dirt.png");

    // maybe this  \/   is the problem
    map = new Map( *bg, matrix_corrected.size()*DEFAULT_BLOCK_SIZE, matrix_corrected[0].size()*DEFAULT_BLOCK_SIZE);

    for(unsigned int i = 0; i < matrix_corrected.size() ; i++)
    {
        for(unsigned int j = 0; j < matrix_corrected[0].size() ; j++)
        {
            if(matrix_corrected[i][j] == 1)
            {
                map->grid[i][j] = new Block( *tex, i, j, DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
                retile(*map, map->grid[i][j]);
            }
        }
    }

    //COUT << "STILL FINE!" << ENDL;
    return map;
}

void Game::saveMapToFile(Map &map, String filename)
{
    FILE* file = Util::createFile(filename);
    saveMapToFile(map, file);
    Util::closeFile(file);
}

void Game::saveMapToFile(Map& map, FILE *file)
{
    for(unsigned int j = 0; j < map.grid[0].size() ; j++)
    {
        int k=0;
        //this is imcomplete, I know, but there's only 1 type of block
        if(map.grid[0][j] != NULL) k=1;
        fprintf(file, "%d", k);

        for(unsigned int i = 1; i < map.grid.size() ; i++)
        {
            k=0;
            //this is imcomplete, I know, but there's only 1 type of block
            if(map.grid[i][j] != NULL) k=1;
            fprintf(file, ",%d", k);
        }

        fprintf(file, ":\n");
    }
}

//main game method - this is the serious stuff
void Game::start()
{
    //these instances are not generic, I know.
    //this is temporary!
    //=================================== ALL OF THIS SHOULD BE REARRANGED IN THE CONSTRUCTOR
    SDLTexture tex("../project/images/pijamaman-1.png");
    //SDLTexture tex("../project/images/guybrushtrepwood-1.png");
    SDLTexture tex2("../project/images/tileset-dirt.png");
    SDLTexture bg("../project/images/background.jpg");

    SDLTexture upkey_img("../project/images/upkey.png");
    SDLTexture downkey_img("../project/images/downkey.png");
    SDLTexture leftkey_img("../project/images/leftkey.png");
    SDLTexture rightkey_img("../project/images/rightkey.png");
    bool is_upkey_pressed=false,is_downkey_pressed=false,is_leftkey_pressed=false,is_rightkey_pressed=false;

    Actor player(tex, 0, 0, 28, 84);
    player.animation.framesOf[0] = 1;
    player.animation.intervalOf[0] = 1000;

    player.animation.framesOf[1] = 1;
    player.animation.intervalOf[1] = 1000;

    player.animation.framesOf[2] = 4;
    player.animation.intervalOf[2] = 130;

    player.animation.framesOf[3] = 4;
    player.animation.intervalOf[3] = 130;

    player.animation.setCurrent(0);

    player.spriteWidth = 56;
    player.spriteHeight = 84;

    player.y_accel = GRAVITY_CONSTANT;


    //Map map(bg,1024,768);

    Map* el_map = NULL;
    try
    {
        el_map = loadMapFromFile("../project/maps/brute.txt");
        COUT << "Map file loaded! " << ENDL;
    }
    catch(Exception e)
    {
        COUT << "Exception! " << e.message() << ENDL;
    }

    Map map = (el_map == NULL?  Map(bg, 1024, 768) : *el_map );

    map.visibleArea.w = SDL::screen->w;
    map.visibleArea.h = SDL::screen->h;

    SDLFont font("../project/fonts/liberation.ttf");
    font.isBold = true;
    font.setFontSize(12);

    SDLTexture *debug_info;
    String debug_info_str;

    SDL_Event event;

    //=================================== END OF "ALL OF THIS SHOULD BE REARRANGED IN THE CONSTRUCTOR"

    lastTimeRendered = 0; //the time when we last rendered
    averageRenderingTime=0; //debug info
    int averageCycleTime=0; //debug info
    int lastCycleTimeOfStart = SDL::getTimeSinceSDLInitialization() - 1; //the time when the last game cycle started
    float deltaTime; //amount of time since the last game cycle

    //deltaTime = (float)Game::cycleFrequency/(float)1000;

    //main loop
    while(true)
    {
        deltaTime = (SDL::getTimeSinceSDLInitialization() - lastCycleTimeOfStart)/((float)1000);
        //COUT << deltaTime << ENDL;
        lastCycleTimeOfStart=SDL::getTimeSinceSDLInitialization();
        //=========================== input treatment =================================
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_t:
                        {
                            player.y_velocity -=800;
                            break;
                        }
                        case SDLK_w:
                        case SDLK_UP:
                        {
                            //debug
                            is_upkey_pressed=true;

                            //y_vel = -4;
                            player.y_velocity -=400;
                            break;
                        }
                        case SDLK_s:
                        case SDLK_DOWN:
                        {
                            //debug
                            is_downkey_pressed=true;

                            //player.y_velocity +=400;
                            break;
                        }
                        case SDLK_a:
                        case SDLK_LEFT:
                        {
                            //debug
                            is_leftkey_pressed=true;

                            player.x_accel -=200;
                            player.animation.setCurrent(2);
                            break;
                        }
                        case SDLK_d:
                        case SDLK_RIGHT:
                        {
                            //debug
                            is_rightkey_pressed=true;

                            player.x_accel +=200;
                            player.animation.setCurrent(3);
                            break;
                        }
                        case SDLK_ESCAPE:
                        {
                            //for later, this is supposed to open an in-game menu
                            saveMapToFile(map, "../project/maps/brute.txt");
                            exit(0);
                        }
                        default:
                        {
                            break;
                        }
                    }
                    //printf("The %s key was pressed!\n", SDL_GetKeyName(event.key.keysym.sym));
                    break;
                }
                case SDL_KEYUP:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                        case SDLK_UP:
                        {
                            //debug
                            is_upkey_pressed=false;
                            break;
                        }

                        case SDLK_s:
                        case SDLK_DOWN:
                        {
                            //debug
                            is_downkey_pressed=false;

                            //nothing for now
                            break;
                        }

                        case SDLK_a:
                        case SDLK_LEFT:
                        {
                            //debug
                            is_leftkey_pressed=false;
                            player.animation.setCurrent(0);
                            player.x_accel = 0;
                            break;

                        }

                        case SDLK_d:
                        case SDLK_RIGHT:
                        {
                            //debug
                            is_rightkey_pressed=false;
                            player.animation.setCurrent(1);
                            player.x_accel = 0;
                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
                    //printf("The %s key was released!\n", SDL_GetKeyName(event.key.keysym.sym));
                    //player.animation.setCurrent(2);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    switch (event.button.button)
                    {
                        case 1: //Left button
                        {


                        if(map.grid[(map.visibleArea.x+event.button.x)/DEFAULT_BLOCK_SIZE][(map.visibleArea.y+event.button.y)/DEFAULT_BLOCK_SIZE] != NULL)
                        {
                            int i = (map.visibleArea.x+event.button.x)/DEFAULT_BLOCK_SIZE;
                            int j = (map.visibleArea.y+event.button.y)/DEFAULT_BLOCK_SIZE;
                            //Block* b = map.grid[i][j];
                            //COUT << "We need to delete this guy on i=" << i << ", j=" << j << " (" << (map.visibleArea.x+event.button.x) << ", " << (map.visibleArea.y+event.button.y) << ")" << ENDL;
                            delete map.grid[i][j];

                            map.grid[i][j] = NULL;

                            if(i > 0) if(map.grid[i-1][j] != NULL) retile(map, map.grid[i-1][j], false);
                            if(i < (int) map.grid.size()-1) if(map.grid[i+1][j] != NULL) retile(map, map.grid[i+1][j], false);
                            if(j > 0) if(map.grid[i][j-1] != NULL) retile(map, map.grid[i][j-1], false);
                            if(j < (int) map.grid[0].size() -1) if(map.grid[i][j+1] != NULL) retile(map, map.grid[i][j+1],false);
                        }

                        break;
                        }
                        case 3: //Right button
                        {

                        if(map.grid[(map.visibleArea.x+event.button.x)/DEFAULT_BLOCK_SIZE][(map.visibleArea.y+event.button.y)/DEFAULT_BLOCK_SIZE] == NULL)
                        {
                            Block* b = new Block(tex2, (map.visibleArea.x+event.button.x)/DEFAULT_BLOCK_SIZE, (map.visibleArea.y+event.button.y)/DEFAULT_BLOCK_SIZE, 16, 16);
                            map.grid[(int) (map.visibleArea.x+event.button.x)/DEFAULT_BLOCK_SIZE][(int) (map.visibleArea.y+event.button.y)/DEFAULT_BLOCK_SIZE] = b;
                            retile(map, b);
                        }

                        break;
                        }

                        default: break;
                    }
                    break;
                }
                case SDL_QUIT:
                {
                    // later this is supposed to ask if the user is sure to leave
                    exit(0);
                }
                default:
                {
                    break;
                }
            }
        }

        //===================   physics treatment   =====================

        //TODO make this drag be cycle-independent and time-dependent instead
        //if the player has no forces on it, slower it down because of drag
        if(player.x_accel == 0) player.x_velocity *=0.8; //slowing down 10% per cycle; this should be a property of the enviroment (e.g. underwater should drag more, like 0.5)
        if(player.y_accel == 0) player.y_velocity *=0.8;

        //update player velocity by acceleration
        player.x_velocity += player.x_accel*deltaTime;
        player.y_velocity += player.y_accel*deltaTime;

        //if player exceeds MAX_SPEED, limit it to MAX_SPEED
        if(player.x_velocity > PLAYER_MAX_HORIZONTAL_SPEED) player.x_velocity = PLAYER_MAX_HORIZONTAL_SPEED;
        else if(player.x_velocity < -PLAYER_MAX_HORIZONTAL_SPEED) player.x_velocity = -PLAYER_MAX_HORIZONTAL_SPEED;
        if(player.y_velocity > MAX_SPEED) player.y_velocity = MAX_SPEED;
        else if(player.y_velocity < -MAX_SPEED) player.y_velocity = -MAX_SPEED;

        //updating player position by velocity
        //Needs to be cycle independent
        player.bounds.x += player.x_velocity*deltaTime;
        player.bounds.y += player.y_velocity*deltaTime;

        //prevents the player from leaving the map bounds. BUG: velocity doesn't reset to zero
        if( player.bounds.x < 0 ) player.bounds.x = 0;
        if( player.bounds.y < 0 ) player.bounds.y = 0;
        if( player.bounds.x + player.bounds.w > map.getWidth() ) player.bounds.x = map.getWidth() - player.bounds.w;
        if( player.bounds.y + player.bounds.h > map.getHeight() ) player.bounds.y = map.getHeight() - player.bounds.h;

        /*
        //checking for collisions with the testing blocks
        for(std::vector<Block>::iterator it = floor.begin(); it!=floor.end(); ++it)
        {
            resolveCollisionBySteps(player, *it);
        }
        */

        //resolvecollision_Test(player, map);

        for(unsigned int i=0; i < map.grid.size(); i++)
        {
            for(unsigned int j=0; j < map.grid[i].size(); j++)
            {
                if(map.grid[i][j] != NULL)
                {
                    resolveCollisionBySteps(player, *(map.grid[i][j]), deltaTime);
                }
            }
        }


        //updates visible area position

        //map.visibleArea.x = (player.bounds.x+player.bounds.w/2) - map.visibleArea.w/2;
        //map.visibleArea.y = (player.bounds.y+player.bounds.h/2) - map.visibleArea.h/2;

        //optimized (less divisions)
        map.visibleArea.x = player.bounds.x + (player.bounds.w - map.visibleArea.w)/2;
        map.visibleArea.y = player.bounds.y + (player.bounds.h - map.visibleArea.h)/2;


        //lock the screen when reached the map's edges
        if ( map.visibleArea.x + map.visibleArea.w > map.getWidth())
        {
            map.visibleArea.x = map.getWidth() - map.visibleArea.w;
        }

        else if ( map.visibleArea.x < 0 )
        {
            map.visibleArea.x = 0;
        }

        if ( map.visibleArea.y + map.visibleArea.h > map.getHeight())
        {
            map.visibleArea.y = map.getHeight() - map.visibleArea.h;
        }

        else if ( map.visibleArea.y < 0 )
        {
            map.visibleArea.y = 0;
        }



        //================================  rendering  ====================================

        //only updates if passed a specific time since last rendering (in this case, cycles will switch between render and not render)
        if( SDL::getTimeSinceSDLInitialization() - lastTimeRendered > (32) )
        {
            Uint32 render_start = SDL::getTimeSinceSDLInitialization();
            //clear the window
            SDL::Window::clean();

            map.draw();

            //draw player
            player.draw(map.visibleArea);

            //debug info
            //COUT << "Player x="<<player.bounds.x<<", y="<<player.bounds.y<<", Vx="<<player.x_velocity<<", Vy="<<player.y_velocity<<", Ax="<<player.x_accel<<", Ay="<<player.y_accel<<", ANIM:"<<player.animation.current()<<ENDL;

            debug_info_str = "x=";
            debug_info_str = debug_info_str + player.bounds.x + "    y=" + player.bounds.y;
            debug_info = new SDLTexture(font.renderTexture(debug_info_str, SDL::getColor(SDL::Color::GREEN) ) );
            debug_info->render(20, 10);
            delete debug_info;

            debug_info_str = "Vx=";
            debug_info_str = debug_info_str + player.x_velocity + "    Vy=" + player.y_velocity;
            debug_info = new SDLTexture(font.renderTexture(debug_info_str, SDL::getColor(SDL::Color::GREEN) ) );
            debug_info->render(20, 30);
            delete debug_info;

            debug_info_str = "Ax=";
            debug_info_str = debug_info_str + player.x_accel + "    Ay=" + player.y_accel;
            debug_info = new SDLTexture(font.renderTexture(debug_info_str, SDL::getColor(SDL::Color::GREEN) ) );
            debug_info->render(20, 50);
            delete debug_info;

            /*
            debug_info_str = "va_x=";
            debug_info_str = debug_info_str + map.visibleArea.x + "\t va_y="+map.visibleArea.y;
            debug_info = new SDLTexture(font.renderTexture(debug_info_str, SDL::getColor(SDL::Color::RED) ) );
            debug_info->render(20, 10);
            delete debug_info;
            */

            debug_info_str = "average rendering time:";
            debug_info_str = debug_info_str + ((int) averageRenderingTime)+"ms, average cycle time "+averageCycleTime+"ms";
            debug_info_str = debug_info_str + ", delta time: "+deltaTime+"ms";
            debug_info = new SDLTexture(font.renderTexture(debug_info_str, SDL::getColor(SDL::Color::LIME) ) );
            debug_info->render(20, 70);
            delete debug_info;


            //debug keys
            if(is_leftkey_pressed) leftkey_img.render(475, 420);
            if(is_upkey_pressed) upkey_img.render(530, 365);
            if(is_downkey_pressed) downkey_img.render(530, 420);
            if(is_rightkey_pressed) rightkey_img.render(585, 420);

            //updates the window with the new content
            SDL::Window::refresh();

            //updates the average rendering time
            if( averageRenderingTime == 0 ) averageRenderingTime = SDL::getTimeSinceSDLInitialization()-render_start;
            else averageRenderingTime =(averageRenderingTime + (SDL::getTimeSinceSDLInitialization()-render_start))/2;

            lastTimeRendered = SDL::getTimeSinceSDLInitialization();
        }


        //updates the average time cycle
        if(averageCycleTime == 0 ) averageCycleTime = SDL::getTimeSinceSDLInitialization() - lastCycleTimeOfStart;
        else averageCycleTime = (averageCycleTime+(SDL::getTimeSinceSDLInitialization() - lastCycleTimeOfStart))/2;

        //Now a correct sleep
        SDL::rest();
    }
    //after the main loop
}

//felipe's algorithm slightly modified
void Game::resolveCollisionBySteps(Actor& object, Block& block, float& deltaTime)
{
    int nX,nY;
    bool xFaster = (fabs(object.x_velocity) > fabs(object.y_velocity));

    nX = object.x_velocity*deltaTime / (float)DEFAULT_BLOCK_SIZE;

    nY = object.y_velocity*deltaTime / (float)DEFAULT_BLOCK_SIZE;

    float x_FinalPosition = object.bounds.x;
    float y_FinalPosition = object.bounds.y;

    object.bounds.x -= object.x_velocity*deltaTime;
    object.bounds.y -= object.y_velocity*deltaTime;

    bool yIntercepted = false;
    bool xIntercepted = false;

    if(xFaster)
    {
        //iterate over the slower axis
        for(int i=0; i<=nY && !yIntercepted; i++)
        {
            if(object.y_velocity > 0) //if moving down
            {
                object.bounds.y += DEFAULT_BLOCK_SIZE;
                if(block.intersects(object.bounds))
                {
                    object.bounds.y = block.y_grid_pos*DEFAULT_BLOCK_SIZE - object.bounds.h;
                    object.y_velocity = 0;
                    yIntercepted = true;
                }
            }
            else if(object.y_velocity < 0) //if moving up
            {
                object.bounds.y -= DEFAULT_BLOCK_SIZE;
                if(block.intersects(object.bounds))
                {
                    object.bounds.y = block.y_grid_pos*(DEFAULT_BLOCK_SIZE + 1);
                    object.y_velocity = 0;
                    yIntercepted = true;
                }
            }

            //iterate block-to-block over x direction to catch colisions
            for(int j = 0 ; j <= nX ; j++)
            {
                if(object.x_velocity >0) // going right
                {
                    object.bounds.x += DEFAULT_BLOCK_SIZE;
                    if(block.intersects(object.bounds))
                    {
                        object.bounds.x = block.x_grid_pos*DEFAULT_BLOCK_SIZE - object.bounds.w;
                        object.x_velocity = 0;
                        xIntercepted =true;
                        break;
                    }
                }
                else if(object.x_velocity < 0) // going left
                {
                    object.bounds.x -= DEFAULT_BLOCK_SIZE;
                    if(block.intersects(object.bounds))
                    {
                        object.bounds.x = block.x_grid_pos*(DEFAULT_BLOCK_SIZE + 1);
                        object.x_velocity = 0;
                        xIntercepted =true;
                        break;
                    }
                }
            }
            if(!xIntercepted) object.bounds.x = x_FinalPosition;
        }
    }
    else
    {
        //iterate over the slower axis
        for(int i=0; i<=nX && !xIntercepted; i++)
        {
            if(object.x_velocity > 0) //if moving right
            {
                object.bounds.x += DEFAULT_BLOCK_SIZE;
                if(block.intersects(object.bounds))
                {
                    object.bounds.x = block.x_grid_pos*DEFAULT_BLOCK_SIZE - object.bounds.w;
                    object.x_velocity = 0;
                    xIntercepted = true;
                }
            }
            else if(object.x_velocity < 0) //if moving left
            {
                object.bounds.x -= DEFAULT_BLOCK_SIZE;
                if(block.intersects(object.bounds))
                {
                    object.bounds.x = block.x_grid_pos*(DEFAULT_BLOCK_SIZE + 1);
                    object.x_velocity = 0;
                    xIntercepted = true;
                }
            }

            //iterate block-to-block over x direction to catch colisions
            for(int j = 0 ; j <= nY ; j++)
            {
                if(object.y_velocity > 0) // going down
                {
                    object.bounds.y += DEFAULT_BLOCK_SIZE;
                    if(block.intersects(object.bounds))
                    {
                        object.bounds.y = block.y_grid_pos*DEFAULT_BLOCK_SIZE - object.bounds.h;
                        object.y_velocity = 0;
                        yIntercepted = true;
                        break;
                    }
                }
                else if(object.y_velocity < 0) // going up
                {
                    object.bounds.y -= DEFAULT_BLOCK_SIZE;
                    if(block.intersects(object.bounds))
                    {
                        object.bounds.y = block.y_grid_pos*(DEFAULT_BLOCK_SIZE + 1);
                        object.y_velocity = 0;
                        yIntercepted = true;
                        break;
                    }
                }
            }
            if(!yIntercepted) object.bounds.y = y_FinalPosition;
        }
    }



//    //necessary to the algorithm. It is MINUS EQUAL(-=) here. That's OK! Trust me!
//    object.bounds.y -= object.y_velocity*deltaTime;
//    if( block.intersects(object.bounds) )
//    {
//        if(object.x_velocity > 0) //if moving right
//        {
//            object.bounds.x = block.x_grid_pos*DEFAULT_BLOCK_SIZE - object.bounds.w;
//        }
//        else if(object.x_velocity < 0) //if moving left
//        {
//            object.bounds.x = block.x_grid_pos*DEFAULT_BLOCK_SIZE + DEFAULT_BLOCK_SIZE;
//        }
//        object.x_velocity = 0;
//    }

//    object.bounds.x -= object.x_velocity*deltaTime;
//    object.bounds.y += object.y_velocity*deltaTime;
//    if( block.intersects(object.bounds) )
//    {
//        if(object.y_velocity > 0) //if moving down
//        {
//            object.bounds.y = block.y_grid_pos*DEFAULT_BLOCK_SIZE - object.bounds.h;
//        }
//        else if(object.y_velocity < 0) //guess this one =)
//        {
//            object.bounds.y = block.y_grid_pos*DEFAULT_BLOCK_SIZE + DEFAULT_BLOCK_SIZE;
//        }
//        object.y_velocity = 0;
//    }
//    object.bounds.x += object.x_velocity*deltaTime;

}

//carlos' algorithm - very buggy ¬¬
void Game::resolveCollisionByProjection(Actor& object, Block& block)
{
    //to avoid repeated multiplications
    int block_x_pos = block.x_grid_pos*DEFAULT_BLOCK_SIZE;
    int block_y_pos = block.x_grid_pos*DEFAULT_BLOCK_SIZE;
    int collisionType=-1;

    //checks whether collides or not
    if(  block.intersects(object.bounds)  )
    {
        COUT << "COLIDE!!!" << ENDL;
        //undo last move step
        object.bounds.x -= object.x_velocity;
        object.bounds.y -= object.y_velocity;

        //checks if object could only collide vertically
        if( (object.bounds.x < block_x_pos && object.bounds.x + object.bounds.w > block_x_pos) //leftmost edge before block leftmost edge but rightmost edge after block leftmost edge
         || (object.bounds.x > block_x_pos && object.bounds.x < block_x_pos + DEFAULT_BLOCK_SIZE) ) //leftmost edge after block leftmost edge but before block rightmost edge
        {
            if(object.y_velocity > 0) collisionType = COLLIDES_TOP;
            else collisionType = COLLIDES_BOTTOM;
        }
        //checks if object could only collide horizontally
        else if( (object.bounds.y < block_y_pos && object.bounds.y + object.bounds.h > block_y_pos) //top edge before block top edge but bottom edge after block top edge
              || (object.bounds.y > block_y_pos && object.bounds.y < block_y_pos + DEFAULT_BLOCK_SIZE) ) //top edge after block top edge but before block bottom edge
        {
            if(object.x_velocity > 0) collisionType = COLLIDES_LEFT;
            else collisionType = COLLIDES_RIGHT;
        }
        //if we got here, the block could collide horizontally or vertically
        else
        {
            //to avoid zero roundings
            float tx=0, ty=0;

            //check if the object is in the "left outer case"
            if( object.bounds.x + object.bounds.w < block_x_pos )
            {
                tx = (block_x_pos - object.bounds.x - object.bounds.w)/object.x_velocity;
                if( object.bounds.y + object.bounds.h < block_y_pos )
                {
                    ty = (block_y_pos - object.bounds.y - object.bounds.h)/object.y_velocity;
                }
                else if( object.bounds.y > block_y_pos + DEFAULT_BLOCK_SIZE )
                {
                    ty = (object.bounds.y - block_y_pos - DEFAULT_BLOCK_SIZE)/object.y_velocity;
                }
            }
            //check if the object is in the "right outer case"
            else if( object.bounds.x > block_x_pos+DEFAULT_BLOCK_SIZE )
            {
                tx = (object.bounds.x - block_x_pos - DEFAULT_BLOCK_SIZE)/object.x_velocity;
                if( object.bounds.y + object.bounds.h < block_y_pos )
                {
                    ty = (block_y_pos - object.bounds.y - object.bounds.h)/object.y_velocity;
                }
                else if( object.bounds.y > block_y_pos + DEFAULT_BLOCK_SIZE )
                {
                    ty = (object.bounds.y - block_y_pos - DEFAULT_BLOCK_SIZE)/object.y_velocity;
                }
            }
            //check which imaginary line the object crosses first. tx is horizontal
            if(tx < ty)
            {
                if(object.y_velocity > 0) collisionType = COLLIDES_TOP;
                else collisionType = COLLIDES_BOTTOM;
            }
            else
            {
                if(object.x_velocity > 0) collisionType = COLLIDES_LEFT;
                else collisionType = COLLIDES_RIGHT;
            }

        }

        switch( collisionType )
        {
            case COLLIDES_TOP:
            case COLLIDES_BOTTOM:
            {
                object.y_velocity=0;
                object.bounds.x += object.x_velocity;
                break;
            }
            case COLLIDES_LEFT:
            case COLLIDES_RIGHT:
            {
                object.x_velocity=0;
                object.bounds.y += object.y_velocity;
                break;
            }
            default: break; //shit...
        }

    }

}

//felipe's 2nd algorithm, also with bugs
void Game::resolvecollision_Test(Actor &player, Map &map)
{
    if( player.y_velocity > 0 ) //if moving down
    {
        int collisionPoints = (player.bounds.w/DEFAULT_BLOCK_SIZE);
        int point_x, point_y;

        for( int i=0; i <= collisionPoints; i++ )
        {
            point_x = (player.bounds.x+i*DEFAULT_BLOCK_SIZE)/DEFAULT_BLOCK_SIZE;
            point_y = (player.bounds.y+player.bounds.h)/DEFAULT_BLOCK_SIZE;

            if( map.grid[point_x][point_y] != NULL )
            {
                player.bounds.y = point_y*DEFAULT_BLOCK_SIZE - player.bounds.h - 1;
            }
        }
    }
    else if( player.y_velocity < 0 ) //if moving up
    {
        int collisionPoints = (player.bounds.w/DEFAULT_BLOCK_SIZE);
        int point_x, point_y;

        for( int i=0; i <= collisionPoints; i++ )
        {
            point_x = (player.bounds.x+i*DEFAULT_BLOCK_SIZE)/DEFAULT_BLOCK_SIZE;
            point_y = (player.bounds.y)/DEFAULT_BLOCK_SIZE;

            if( map.grid[point_x][point_y] != NULL )
            {
                player.bounds.y = point_y*DEFAULT_BLOCK_SIZE + map.grid[point_x][point_y]->spriteHeight + 1;
            }
        }
    }

    if( player.x_velocity > 0 ) // if moving right
    {
        int collisionPoints = (player.bounds.h/DEFAULT_BLOCK_SIZE);
        int point_x, point_y;

        for( int i=0; i <= collisionPoints; i++ )
        {
            point_x = (player.bounds.x+player.bounds.w)/DEFAULT_BLOCK_SIZE;
            point_y = (player.bounds.y+i*DEFAULT_BLOCK_SIZE)/DEFAULT_BLOCK_SIZE;

            if( map.grid[point_x][point_y] != NULL )
            {
                player.bounds.x = point_x*DEFAULT_BLOCK_SIZE - player.bounds.w - 1;
            }
        }

        //COUT << point_x << " " << point_y << ENDL;
    }
    else if( player.x_velocity < 0 ) // if moving left
    {
        int collisionPoints = (player.bounds.h/DEFAULT_BLOCK_SIZE);
        int point_x, point_y;

        for( int i=0; i <= collisionPoints; i++ )
        {
            point_x = (player.bounds.x)/DEFAULT_BLOCK_SIZE;
            point_y = (player.bounds.y+i*DEFAULT_BLOCK_SIZE)/DEFAULT_BLOCK_SIZE;

            if( map.grid[point_x][point_y] != NULL )
            {
                player.bounds.x = point_x*DEFAULT_BLOCK_SIZE + map.grid[point_x][point_y]->spriteWidth + 1;
            }
        }
    }
}
