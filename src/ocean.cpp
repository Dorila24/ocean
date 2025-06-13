#include "ocean.hpp"
#include "entities/algae.hpp"
#include "entities/herbivore.hpp"
#include "entities/predator.hpp"
#include "entities/crab.hpp"

#include <vector>
#include <memory>
#include <random>
#include <cstdlib>

/* ── скрытая реализация ─────────────────────────── */
struct Ocean::Impl {
    int w{}, h{};
    std::vector<std::vector<std::unique_ptr<Entity>>> grid;
    Impl(int W,int H): w(W), h(H) {
        grid.resize(H);
        for(auto& r : grid) r.resize(W);}};

/* ── перемещение ─────────────────────────────────── */
Ocean::Ocean(Ocean&&) noexcept            = default;
Ocean& Ocean::operator=(Ocean&&) noexcept = default;

/* ── конструктор ─────────────────────────────────── */
Ocean::Ocean(int w,int h): pImpl(std::make_unique<Impl>(w,h))
{
    std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<> rndX(0,w-1);
    for(int i=0; i<4; ++i){
        int x = rndX(rng);
        int y = h - SAND_ROWS - 6;
        spawn<Herbivore>(x,y);}
    for(int i=0; i<2; ++i){
        int x = rndX(rng);
        int y = h - SAND_ROWS - 10;
        spawn<Predator>(x,y);}

    spawn<Crab>(w/2, h - SAND_ROWS/2);}
Ocean::~Ocean() = default;

/* ── базовые методы ─────────────────────────────── */
int Ocean::width()  const { return pImpl->w; }
int Ocean::height() const { return pImpl->h; }
int Ocean::minAlgaeRow() const { return pImpl->h - MAX_ALGAE; }

bool Ocean::isEmpty(int x,int y) const{
    return x>=0 && y>=0 && x<width() && y<height() && !pImpl->grid[y][x];}
Entity* Ocean::entityAt(int x,int y) const{
    return (x>=0 && y>=0 && x<width() && y<height()) ? pImpl->grid[y][x].get() : nullptr;}
void Ocean::moveEntity(int x1,int y1,int x2,int y2){
    pImpl->grid[y2][x2] = std::move(pImpl->grid[y1][x1]);}
void Ocean::clearCell(int x,int y){ pImpl->grid[y][x].reset(); }

void Ocean::shiftColumnDown(int x,int fromY)
{
    for(int y=fromY; y>=0; --y){
        if(y+1>=height()) break;
        if(!isEmpty(x,y+1)) break;
        if(isEmpty(x,y))   continue;
        moveEntity(x,y,x,y+1);}}

/* ── шаблон spawn ───────────────────────────────── */
template<class T,class...A>
void Ocean::spawn(int x,int y,A&&...a){
    if(isEmpty(x,y))
        pImpl->grid[y][x] = std::make_unique<T>(std::forward<A>(a)...);}

/* ── рост водорослей: снизу вверх ───────────────── */
static void tryGrowAlgae(Ocean& oc,int x)
{
    static std::mt19937 rng{ std::random_device{}() };
    if(std::uniform_int_distribution<>(0,99)(rng) >= ALGAE_GROW_CHANCE)
        return;

    const int ySand   = oc.height() - SAND_ROWS;
    const int yBottom = ySand - 1;
    const int yTopLim = oc.minAlgaeRow();

    int y = yBottom;
    while(y >= yTopLim && dynamic_cast<Algae*>(oc.entityAt(x,y)))
        --y;
    ++y;
    int yGrow = y - 1;
    if(yGrow < yTopLim)      return;
    if(!oc.isEmpty(x,yGrow)) return;

    oc.spawn<Algae>(x,yGrow);}

/* ── статистика ─────────────────────────────────── */
Population Ocean::population() const
{
    Population p{};
    for(auto& row:pImpl->grid)
        for(auto& e:row)
            if(e){
                if     (dynamic_cast<Algae*    >(e.get())) ++p.algae;
                else if(dynamic_cast<Herbivore*>(e.get())) ++p.herb;
                else if(dynamic_cast<Predator* >(e.get())) ++p.pred;
                else                                       ++p.crab;}
    return p;}

/* ── логика ─────────────────────────────────────── */
void Ocean::update()
{
    for(int y=0; y<height(); ++y)
        for(int x=0; x<width(); ++x)
            if(pImpl->grid[y][x])
                pImpl->grid[y][x]->update(*this,x,y);
    for(int x=0; x<width(); ++x)
        tryGrowAlgae(*this,x);}

/* ── рендер ─────────────────────────────────────── */
void Ocean::draw(sf::RenderTarget& tgt) const
{
    tgt.clear(sf::Color(10,60,120));

    sf::RectangleShape sand({float(width()*CELL), float(SAND_ROWS*CELL)});
    sand.setFillColor(sf::Color(194,178,128));
    sand.setPosition({0.f, (height()-SAND_ROWS)*CELL});
    tgt.draw(sand);
    for(int y=0; y<height(); ++y)
        for(int x=0; x<width(); ++x)
            if(pImpl->grid[y][x])
                pImpl->grid[y][x]->draw(tgt,CELL,float(x),float(y));}
