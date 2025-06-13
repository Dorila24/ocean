#include "entities/crab.hpp"
#include "ocean.hpp"
#include "texture_mgr.hpp"
#include "constants.hpp"

Crab::Crab() = default;

void Crab::update(Ocean& oc,int x,int y)
{
    if(timer_.getElapsedTime().asMilliseconds() < CRAB_PERIOD_MS) return;
    timer_.restart();

    int nx = x + dir_;
    if(nx < 0 || nx >= oc.width() || !oc.isEmpty(nx,y))
        dir_ = -dir_, nx = x + dir_;

    if(oc.isEmpty(nx,y)) oc.moveEntity(x,y,nx,y);}

void Crab::draw(sf::RenderTarget& tgt,float cell,float gx,float gy) const
{
    static const sf::Texture& tex = TextureMgr::inst().get("crab.png");
    sf::Sprite spr(tex);

    sf::Vector2f scale(cell/tex.getSize().x, cell/tex.getSize().y);
    spr.setScale(scale);
    spr.setPosition({gx*cell, gy*cell});

    tgt.draw(spr);}
