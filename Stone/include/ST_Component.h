#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <SDL3_ttf/SDL_ttf.h>

#include "ST_Vector2D.h"
#include "ST_Entity.h"
#include "ST_AnimationClip.h"

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
    float facing = 1.0f;
};

struct Position
{
    float x = 0.0f;
    float y = 0.0f;
};

enum class RenderLayer
{
    Gameplay,
    UI
};

struct Sprite
{
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dest{};

    ST_Vector2D fixedDimension{};

    bool flip = false;
    RenderLayer renderLayer = RenderLayer::Gameplay;
    bool isVisible = true;
};

struct Clickable
{
    std::function<void()> onPressed{};
    std::function<void()> onReleased{};
    std::function<void()> onCancel{};

    bool pressed = false;
};

struct Parent
{
    ST_Entity* parent = nullptr;
};

struct Children
{
    std::vector<ST_Entity*> children{};
};

struct Collider
{
    std::string tag;
    SDL_FRect rect{};

    bool isEnabled = true;
};

struct Animation
{
    std::unordered_map<std::string, ST_AnimationClip> clips{};
    std::string currentClip{};
    float time{};       // time is accumulated for the current frame
    int currentFrame{}; // index of the current frame in the current clip
    float speed = 0.1f;

    bool isFinished = false;
};

struct Camera
{
    SDL_FRect view;
    float worldWidth;
    float worldHeight;
};

struct MapTile
{
    int row{};  // Row index of the tile in the 2D map matrix
    int col{};  // Col index of the tile in the 2D map matrix
};

struct Projectile
{
    int id;
    int angle = 30;
    int forceIncrement = 20;

    ST_Vector2D forceRange{ 20, 2000 };
    ST_Vector2D angleRange{ 0, 89 };

    int currentForce = static_cast<int>(forceRange.x);
};

struct AnglePointerUI
{
    int id{};

    float distFromPlayer{};
};

struct Health
{
    ST_Vector2D range{};

    float current = range.y;
};

struct DamageAccumulator
{
    float maxOverlap = 0.0f;
    ST_Entity* targetPlayer = nullptr;
};

enum class GameState
{
    Playing,
    GameOver
};

struct GameStateComponent
{
    GameState state = GameState::Playing;
    ST_Entity* winner = nullptr;
    ST_Entity* overlay = nullptr;
};

struct ScreenSpaceUI
{
    ST_Vector2D screenPosition; // fixed UI position
};

enum class PlayerState
{
    Idle,
    Walk,
    Hurt,
    Falling,
    Throwing
};

struct PlayerActionFlags
{
    bool isThrowing = false;
    bool isHurt = false;
};

struct PlayerStateComponent
{
    PlayerState state = PlayerState::Idle;
};

enum class LabelType
{
    PlayerPosition,
    PlayerColliderPosition
};

struct Label
{
    std::string text{};
    TTF_Font* font = nullptr;
    SDL_Color color{};
    LabelType type = LabelType::PlayerPosition;

    std::string textureCacheKey{};
    SDL_Texture* texture = nullptr;
    SDL_FRect dest{};

    bool isVisible = true;
    bool dirty = false;
};

// ------------ TAGS ---------------- //
struct PlayerTag
{
    int id;
};
struct PowerBarTag
{
    int id;
};
struct AngleFrameUITag
{
    int id;
};
struct HealthUITag
{
    int id;
};
struct ProjectileTag {};
struct ActivePlayerTag {};
struct PendingProjectileTag {};
struct DestructiveProjectileTag {};
struct PendingDestroy {};
struct PendingHealthUIUpdateTag {};
struct DamageAppliedTag {};
struct WinnerTag {};
