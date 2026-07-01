#pragma once

#include "platformer_lib.h"

#include "assets.h"

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Renderer Constants
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Renderer Structs
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
struct OrthographicCamera2D{
    float zoom = 1.0f;
    Vec2 dimensions;
    Vec2 position;

};
struct Transform{
    Vec2 position;
    Vec2 size;
    IVec2 atlasOffset;
    IVec2 spriteSize;
};

struct RenderData{
    OrthographicCamera2D gameCamera;
    OrthographicCamera2D uiCamera;

    int transformCount;
    Transform transforms[MAX_TRANSFORMS];
};

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Renderer Globals
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
static RenderData* renderData;


// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Renderer Functions
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void draw_sprite(SpriteID spriteID, Vec2 pos){
    Sprite sprite = get_sprite(spriteID);

    Transform transform = {};
    transform.position = pos - Ivec2_Vec2(sprite.spriteSize) / 2.0f;
    transform.size = Ivec2_Vec2(sprite.spriteSize);
    transform.atlasOffset = sprite.atlasOffset;
    transform.spriteSize = sprite.spriteSize;

    renderData->transforms[renderData->transformCount++] = transform;
}