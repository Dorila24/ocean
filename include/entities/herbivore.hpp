#ifndef HERBIVORE_HPP
#define HERBIVORE_HPP

#include "entity.hpp"


class Herbivore : public Entity
{
public:
    void update(class Ocean& oc,int x,int y) override;
    void draw (sf::RenderTarget& tgt,float cell,float gx,float gy) const override;

private:
    int  hunger_ = 0;
    int  age_    = 0;
    bool mateReady_ = false;};

#endif
