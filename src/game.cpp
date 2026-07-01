#include "game.h"
#include "assets.h"
#include "platformer_lib.h"

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Game Constants
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;
constexpr int TILESIZE = 8;
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Game Structs
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Game Functions
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
EXPORT_FN void update_game(GameStates* gameStatesIn, RenderData* renderDataIn, Input* inputIn){
    if(renderData != renderDataIn){
        gameStates = gameStatesIn;
        renderData = renderDataIn;
        input = inputIn;
    }
    if(!gameStates->initialized){
        renderData->gameCamera.dimensions = {(float)WORLD_WIDTH, (float)WORLD_HEIGHT};


        gameStates->initialized = true;
    }

    renderData->gameCamera.position.x = 0;
    renderData->gameCamera.position.y = 0;
    
    draw_sprite(SPRITE_TEST, {0.0f, 0.0f});
}