#include "entities/herbivore.hpp"
#include "entities/algae.hpp"
#include "entities/predator.hpp"
#include "ocean.hpp"
#include "texture_mgr.hpp"
#include "utils/rng.hpp"
#include "constants.hpp"

#include <array>
#include <algorithm>
#include <vector>

namespace {
    const std::array<std::pair<int,int>,4> dirs{{{0,-1},{-1,0},{1,0},{0,1}}};
    auto& rng = Rng::instance().engine();
}

static bool isPred (const Ocean& oc,int x,int y){ return dynamic_cast<Predator* >(oc.entityAt(x,y)); }
static bool isAlgae(const Ocean& oc,int x,int y){ return dynamic_cast<Algae*   >(oc.entityAt(x,y)); }
static Herbivore* getHerb(const Ocean& oc,int x,int y){ return dynamic_cast<Herbivore*>(oc.entityAt(x,y)); }

inline int fishDepthLimit(const Ocean& oc){ return oc.height() - 5; }

void Herbivore::update(Ocean& oc,int x,int y)
{
    ++age_;
    ++hunger_;

    bool hungry = (hunger_ >= HUNGRY_HERB);
    if(mateReady_ && hungry) mateReady_ = false;

    if(age_ >= AGE_LIMIT && oc.population().herb >= MAX_POP){
        oc.clearCell(x,y);
        return;}

    int foodX=-1, foodY=-1, foodD=999;
    if(hungry)
        for(int yy=0; yy<fishDepthLimit(oc); ++yy)
            for(int xx=0; xx<oc.width(); ++xx)
                if(isAlgae(oc,xx,yy)){
                    int d = std::abs(xx-x)+std::abs(yy-y);
                    if(d < foodD){ foodD=d; foodX=xx; foodY=yy; }}

    int mateX=-1, mateY=-1, mateD=999;
    if(mateReady_)
        for(int yy=0; yy<fishDepthLimit(oc); ++yy)
            for(int xx=0; xx<oc.width(); ++xx)
                if(auto* h=getHerb(oc,xx,yy))
                    if(h!=this && h->mateReady_){
                        int d = std::abs(xx-x)+std::abs(yy-y);
                        if(d < mateD){ mateD=d; mateX=xx; mateY=yy; }}

    std::array<int,4> order{0,1,2,3};
    std::shuffle(order.begin(),order.end(),rng);

    int bestDir=order[0], bestScore=-9999;
    for(int d:order){
        int nx=x+dirs[d].first, ny=y+dirs[d].second;
        if(nx<0||ny<0||nx>=oc.width()||ny>=fishDepthLimit(oc)) continue;
        if(!oc.isEmpty(nx,ny) && !isAlgae(oc,nx,ny))          continue;

        int score = 0;
        if(hungry && isAlgae(oc,nx,ny)) score += 1000;
        if(hungry && foodX>=0) score -= (std::abs(foodX-nx)+std::abs(foodY-ny));
        if(mateX>=0)           score -= (std::abs(mateX-nx)+std::abs(mateY-ny));

        for(int yy=ny-3; yy<=ny+3; ++yy)
            for(int xx=nx-3; xx<=nx+3; ++xx)
                if(isPred(oc,xx,yy)) score -= 5;

        if(score > bestScore){ bestScore=score; bestDir=d; }}

    int nx=x+dirs[bestDir].first, ny=y+dirs[bestDir].second;

    if(isAlgae(oc,nx,ny)){
        bool top = oc.entityAt(nx,ny-1)==nullptr;
        if(hungry || top){
            oc.clearCell(nx,ny);
            oc.shiftColumnDown(nx,ny-1);
            hunger_=0;
            mateReady_=true;
        }
    }
    if(oc.isEmpty(nx,ny)) oc.moveEntity(x,y,nx,ny);

    if(mateReady_)
    {
        const std::array<std::pair<int,int>,4> nbr{{{0,-1},{-1,0},{1,0},{0,1}}};
        for(int d=0; d<4; ++d){
            int ax=x+nbr[d].first, ay=y+nbr[d].second;
            if(auto* h=getHerb(oc,ax,ay); h && h->mateReady_){
                std::vector<std::pair<int,int>> spots;
                for(auto [ux,uy]:nbr){
                    int sx=x+ux, sy=y+uy;
                    if(oc.isEmpty(sx,sy) && sy<fishDepthLimit(oc)) spots.push_back({sx,sy});
                    sx=ax+ux; sy=ay+uy;
                    if(oc.isEmpty(sx,sy) && sy<fishDepthLimit(oc)) spots.push_back({sx,sy});}
                if(!spots.empty() && Rng::instance().chance(BIRTH_HERB)){
                    std::shuffle(spots.begin(),spots.end(),rng);
                    oc.spawn<Herbivore>(spots.front().first, spots.front().second);}
                mateReady_=h->mateReady_=false;
                break;}}}}

void Herbivore::draw(sf::RenderTarget& tgt,float cell,float gx,float gy) const
{
    static const sf::Texture& tex = TextureMgr::inst().get("herbivore.png");
    sf::Sprite spr(tex);
    spr.setScale({cell/tex.getSize().x, cell/tex.getSize().y});
    spr.setPosition({gx*cell, gy*cell});
    tgt.draw(spr);}
