#pragma once

#include <vector>
#include <memory>
#include <SDL3/SDL.h>

#include "ST_Entity.h"
#include "ST_RenderingSystem.h"
#include "ST_ISystem.h"

class ST_Renderer;

class ST_Layer
{
public:
    ST_Layer() = default;
    ~ST_Layer() = default;

    ST_Entity& createEntity();
    void update( float delta, const SDL_Event& event );
    void render();
    void cleanup();

    inline std::vector<std::unique_ptr<ST_Entity>>& getEntities() { return m_Entities; }

private:
    ST_RenderingSystem m_RenderingSystem{};
    std::vector<std::unique_ptr<ST_Entity>> m_Entities{};
};