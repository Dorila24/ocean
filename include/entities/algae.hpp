#ifndef ALGAE_HPP
#define ALGAE_HPP
#include "entity.hpp"
class Algae : public Entity {
public:
    void update(class Ocean&,int,int) override {}
    void draw(sf::RenderTarget&,float,float,float) const override;};
#endif
