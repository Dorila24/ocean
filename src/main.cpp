#include "ocean.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <cstring>

int main()
{
    const sf::Vector2u winSz{ unsigned(FIELD_W*CELL), unsigned(FIELD_H*CELL) };
    const char* ru = u8"Живой океан";
    sf::String base = sf::String::fromUtf8(ru, ru+std::strlen(ru));

#if SFML_VERSION_MAJOR >= 3
    sf::RenderWindow win(sf::VideoMode(winSz), base);
#else
    sf::RenderWindow win(sf::VideoMode(winSz.x, winSz.y), base);
#endif

    Ocean ocean(FIELD_W, FIELD_H);
    bool paused = false;
    sf::Clock tick, title;
    const sf::Time step   = sf::milliseconds(TICK_MS);
    const sf::Time titInt = sf::seconds(1);

    auto updateTitle = [&]{
        Population p = ocean.population();
        std::string s = base.toAnsiString() +
            " | A:" + std::to_string(p.algae) +
            " H:"  + std::to_string(p.herb) +
            " P:"  + std::to_string(p.pred) +
            " C:"  + std::to_string(p.crab);
        win.setTitle(sf::String(s));};
    updateTitle();

    while(win.isOpen())
    {
#if SFML_VERSION_MAJOR >= 3
        while(auto ev = win.pollEvent())
        {
            if(ev->is<sf::Event::Closed>()) win.close();
            if(auto kp = ev->getIf<sf::Event::KeyPressed>())
            {
                sf::Keyboard::Key  kc = kp->code;
                sf::Keyboard::Scan sc = kp->scancode;
                auto key=[&](auto k,auto s){
                    return (kc==k && kc!=sf::Keyboard::Key::Unknown)|| sc==s;};
                if(key(sf::Keyboard::Key::Escape,sf::Keyboard::Scan::Escape)) win.close();
                else if(key(sf::Keyboard::Key::Space,sf::Keyboard::Scan::Space)) paused=!paused;
                else if(key(sf::Keyboard::Key::R,sf::Keyboard::Scan::R))
                { ocean = Ocean(FIELD_W,FIELD_H); updateTitle(); }}}
#else
        sf::Event e{};
        while(win.pollEvent(e))
        {
            if(e.type==sf::Event::Closed) win.close();
            if(e.type==sf::Event::KeyPressed)
                switch(e.key.code){
                    case sf::Keyboard::Escape: win.close(); break;
                    case sf::Keyboard::Space:  paused=!paused; break;
                    case sf::Keyboard::R:      ocean = Ocean(FIELD_W,FIELD_H); updateTitle(); break;
                    default: break;}}
#endif
        if(!paused && tick.getElapsedTime()>=step){ ocean.update(); tick.restart(); }
        if(title.getElapsedTime()>=titInt){ updateTitle(); title.restart(); }

        win.clear();
        ocean.draw(win);
        win.display();}
    return 0;}
