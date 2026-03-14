#pragma once

#include <memory>

#include "Registry.h"

class ISystem
{
public:
    virtual void update( const std::unique_ptr<Registry>& registry ) = 0;
};