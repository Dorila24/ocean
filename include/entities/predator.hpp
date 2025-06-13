#ifndef PREDATOR_HPP
#define PREDATOR_HPP
#include "entity.hpp"
class Predator : public Entity {
public:
    void update(class Ocean&,int,int) override;
    void draw (sf::RenderTarget&,float,float,float) const override;
private:
    int  hunger_ = 0, age_ = 0;
    bool mateReady_ = false;};
#endif
