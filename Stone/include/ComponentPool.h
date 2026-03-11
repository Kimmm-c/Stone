#pragma once

#include <memory>
#include <vector>

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
};

template<typename T>
class ComponentPool : public IComponentPool
{
public:
    ComponentPool() = default;
    ~ComponentPool() = default;

    friend class Registry;

private:
    template <typename... Args>
    T& add( Args&&... args )
    {

    }

private:
    std::vector<T> m_Components{};
};