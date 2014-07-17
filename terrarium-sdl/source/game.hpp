#ifndef GAME_HPP
#define GAME_HPP

#include "sdl.hpp"
#include "actor.hpp"
#include "block.hpp"
#include "map.hpp"

#include <vector>

#define GRAVITY_CONSTANT 500
#define MAX_SPEED 800 //deprecated
#define PLAYER_MAX_HORIZONTAL_SPEED 200

class Game
{
    private:

    //game attributes
    int cycleFrequency, gravityAcceleration, globalMaximumSpeed;
    Uint32 lastTimeRendered, averageRenderingTime;

    std::vector<Texture*> textureGarbageCollector;

    //thing for showing debug thing on the screen
    std::map<int, bool> debugFlags;

    Map* map;
    Actor* player;

    //not sure why I would need these...
    enum {NO_COLLISION, COLLIDES_TOP, COLLIDES_BOTTOM, COLLIDES_LEFT, COLLIDES_RIGHT};

    int collisionTypeWith(Actor player, Block block);


    //loads the map
    static Map* loadMapFromFile(FILE* file);
    static Map* loadMapFromFile(String filename);

    //saves the map
    static void saveMapToFile(Map& map, FILE* file);
    static void saveMapToFile(Map& map, String filename);

    //algorithms to detect collision
    void resolveCollisionBySteps(Actor& player, Block& block, float& deltaTime);
    void resolveCollisionByProjection(Actor& player, Block& block);
    void resolvecollision_Test(Actor &player, Map &map);
    void doPhysics(const float& deltaTime);

    //adjusts the tiles used by the given block, and its neightborhood if 'recursive' is true
    static inline void retile(Map& map, Block* b, bool recursive=true);

    public:

    Game();

    ~Game();

    void start();

    int getGravityConstant();
    void setGravityConstant(const int& newConstant);
};

#endif // GAME_HPP
