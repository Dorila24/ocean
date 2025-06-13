#include "entities/algae.hpp"
#include "texture_mgr.hpp"
#include "constants.hpp"

void Algae::draw(sf::RenderTarget& tgt,float cell,float gx,float gy) const
{
    static const sf::Texture& tex = TextureMgr::inst().get("algae.png");
    sf::Sprite spr(tex);

    sf::Vector2f scale(cell/tex.getSize().x, cell/tex.getSize().y);
    spr.setScale(scale);
    spr.setPosition({gx*cell, gy*cell});

    tgt.draw(spr);}
