#ifndef OCEAN_HPP
#define OCEAN_HPP
#include <memory>
#include <SFML/Graphics.hpp>
#include "constants.hpp"

struct Population { int algae{}, herb{}, pred{}, crab{}; };

class Entity; class Algae; class Herbivore; class Predator; class Crab;

/*────────────────────────────────────────────*/
class Ocean {
public:
    Ocean(int w,int h);
    ~Ocean();

    /* перемещение объявляем, но не определяем здесь */
    Ocean(Ocean&&) noexcept;
    Ocean& operator=(Ocean&&) noexcept;

    Ocean(const Ocean&)            = delete;
    Ocean& operator=(const Ocean&) = delete;

    void update();
    void draw(sf::RenderTarget&) const;

    int  width()  const;
    int  height() const;
    int  minAlgaeRow() const;

    bool    isEmpty (int,int) const;
    Entity* entityAt(int,int) const;
    void    moveEntity      (int,int,int,int);
    void    clearCell       (int,int);
    void    shiftColumnDown (int,int);

    template<class T,class... Args>
    void spawn(int,int,Args&&...);

    Population population() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
#endif
