#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <functional>

#include "ST_Vector2D.h"

struct Transform
{
    ST_Vector2D position{};
    ST_Vector2D oldPosition{};
    float rotation{};
    float scale{};
};

struct Velocity
{
    ST_Vector2D direction{};
    float speed{};
};

struct Position
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Sprite
{
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dest{};
};

struct Collider
{
    std::string tag;
    SDL_FRect rect{};
};

//struct Animation
//{
//    std::unordered_map<std::string, AnimationClip> clips{};
//    std::string currentClip{};
//    float time{};   // time is accumulated for the current frame
//    int currentFrame{}; // index of the current frame in the current clip
//    float speed = 0.1f;
//};

struct Camera
{
    SDL_FRect view;
    float worldWidth;
    float worldHeight;
};


struct TimedSpawner
{
    float spawnInterval{};
    std::function<void()> spawnCallback{};
    float timer{};
};

struct SceneState
{
    int coinsCollected = 0;
};

struct MapTile
{
    int row{};  // Row index of the tile in the 2D map matrix
    int col{};  // Col index of the tile in the 2D map matrix
};


// ------------ TAGS ---------------- //
struct PlayerTag {};
struct DestructiveProjectileTag {};
