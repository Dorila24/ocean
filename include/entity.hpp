#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <SFML/Graphics.hpp>
class Ocean;

class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(Ocean&,int,int) = 0;
    virtual void draw (sf::RenderTarget&,float,float,float) const = 0;};
#endif
