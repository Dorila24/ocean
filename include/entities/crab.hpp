#ifndef CRAB_HPP
#define CRAB_HPP
#include "entity.hpp"
#include <SFML/System/Clock.hpp>

class Crab : public Entity {
public:
    Crab();
    void update(class Ocean&,int,int) override;
    void draw (sf::RenderTarget&,float,float,float) const override;
private:
    int dir_ = 1;
    sf::Clock timer_;};
#endif
