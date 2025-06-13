#include "entities/predator.hpp"
#include "entities/herbivore.hpp"
#include "entities/algae.hpp"
#include "ocean.hpp"
#include "texture_mgr.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"

#include <array>
#include <algorithm>
#include <vector>

namespace { const std::array<std::pair<int,int>,4> dirs{{{0,-1},{-1,0},{1,0},{0,1}}}; }
static bool      isHerb(const Ocean& oc,int x,int y){ return dynamic_cast<Herbivore*>(oc.entityAt(x,y)); }
static Predator* getPred(const Ocean& oc,int x,int y){ return dynamic_cast<Predator* >(oc.entityAt(x,y)); }
static bool      isAlgae(const Ocean& oc,int x,int y){ return dynamic_cast<Algae*   >(oc.entityAt(x,y)); }

inline int fishDepthLimit(const Ocean& oc){ return oc.height() - 5; }

/*──────────────────────────────────────────*/
void Predator::update(Ocean& oc,int x,int y)
{
    ++age_;
    ++hunger_;

    bool veryHungry = (hunger_ >= HUNGRY_PRED);
    if(mateReady_ && veryHungry) mateReady_ = false;

    /* старение */
    if(age_ >= AGE_LIMIT && oc.population().pred >= MAX_POP){
        oc.clearCell(x,y);
        return;}

    /* поиск жертвы */
    int tx=-1,ty=-1,minD=999;
    for(int yy=0; yy<fishDepthLimit(oc); ++yy)
        for(int xx=0; xx<oc.width(); ++xx)
            if(isHerb(oc,xx,yy)){
                int d=std::abs(xx-x)+std::abs(yy-y);
                if(d<minD){minD=d; tx=xx; ty=yy;}}

    /* поиск партнёра */
    int mx=-1,my=-1;
    if(mateReady_)
        for(int yy=0; yy<fishDepthLimit(oc); ++yy)
            for(int xx=0; xx<oc.width(); ++xx)
                if(auto* p=getPred(oc,xx,yy))
                    if(p!=this && p->mateReady_){
                        int d=std::abs(xx-x)+std::abs(yy-y);
                        if(d<minD){minD=d; mx=xx; my=yy;}}

    int dir;
    if(tx>=0 && veryHungry)
        dir = (std::abs(tx-x)>std::abs(ty-y))?((tx<x)?1:2):((ty<y)?0:3);
    else if(mx>=0)
        dir = (std::abs(mx-x)>std::abs(my-y))?((mx<x)?1:2):((my<y)?0:3);
    else{
        std::array<int,4> rnd{0,1,2,3};
        std::shuffle(rnd.begin(),rnd.end(),Rng::instance().engine());
        dir=rnd[0];}

    int nx=x+dirs[dir].first, ny=y+dirs[dir].second;

    /* фильтр поля */
    if(nx<0||ny<0||nx>=oc.width()) return;
    if(ny>=fishDepthLimit(oc))     return;
    if(!oc.isEmpty(nx,ny)&&!isAlgae(oc,nx,ny)&&!isHerb(oc,nx,ny)) return;
    if(!veryHungry && isHerb(oc,nx,ny)) return;

    /* действие */
    if(veryHungry && isHerb(oc,nx,ny)){
        oc.clearCell(nx,ny);
        hunger_=0;
        mateReady_=true;}
    if(isAlgae(oc,nx,ny)) oc.clearCell(nx,ny);
    if(oc.isEmpty(nx,ny)) oc.moveEntity(x,y,nx,ny);

    /* спаривание */
    if(mateReady_){
        const std::array<std::pair<int,int>,4> nbr{{{0,-1},{-1,0},{1,0},{0,1}}};
        for(auto [dx,dy]:nbr){
            int ax=x+dx, ay=y+dy;
            if(auto* p=getPred(oc,ax,ay); p && p->mateReady_){
                std::vector<std::pair<int,int>> spots;
                for(auto [ux,uy]:nbr){
                    int sx=x+ux, sy=y+uy;
                    if(oc.isEmpty(sx,sy) && sy<fishDepthLimit(oc)) spots.push_back({sx,sy});
                    sx=ax+ux; sy=ay+uy;
                    if(oc.isEmpty(sx,sy) && sy<fishDepthLimit(oc)) spots.push_back({sx,sy});}
                if(!spots.empty() && Rng::instance().chance(BIRTH_PRED)){
                    std::shuffle(spots.begin(),spots.end(),Rng::instance().engine());
                    oc.spawn<Predator>(spots.front().first, spots.front().second);}
                mateReady_=p->mateReady_=false;
                break;}}}}
void Predator::draw(sf::RenderTarget& tgt,float cell,float gx,float gy) const
{
    static const sf::Texture& tex = TextureMgr::inst().get("predator.png");
    sf::Sprite spr(tex);
    spr.setScale({cell/tex.getSize().x, cell/tex.getSize().y});
    spr.setPosition({gx*cell, gy*cell});
    tgt.draw(spr);}
