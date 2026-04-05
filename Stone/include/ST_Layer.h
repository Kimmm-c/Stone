#pragma once

#include <vector>
#include <memory>
#include <SDL3/SDL.h>

#include "ST_Entity.h"
#include "ST_RenderingSystem.h"
#include "ST_MapRenderingSystem.h"
#include "ST_UIRenderSystem.h"
#include "ST_ISystem.h"

class ST_Renderer;
class Camera;

class ST_Layer
{
public:
    ST_Layer() = default;
    ~ST_Layer() = default;

    ST_Entity& createEntity();
    void update( float delta, const SDL_Event& event );
    void render( Camera* camera = nullptr );
    void cleanup();

    inline std::vector<std::unique_ptr<ST_Entity>>& getEntities() { return m_Entities; }

private:
    std::unique_ptr<ST_IRenderSystem> m_RenderingSystem = std::make_unique<ST_RenderingSystem>();
    std::unique_ptr<ST_IRenderSystem> m_MapRenderingSystem = std::make_unique<ST_MapRenderingSystem>();
    std::unique_ptr<ST_IRenderSystem> m_UIRenderingSystem = std::make_unique<ST_UIRenderSystem>();
    std::vector<std::unique_ptr<ST_Entity>> m_Entities{};
};