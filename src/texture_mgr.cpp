#include "texture_mgr.hpp"
#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>

TextureMgr& TextureMgr::inst() { static TextureMgr tm; return tm; }

/* ───── заглушка цвета ───── */
static sf::Texture makePlaceholder(sf::Color c)
{
    sf::Image img({4u,4u}, c);
    sf::Texture t;
    (void)t.loadFromImage(img);
    return t;}

const sf::Texture& TextureMgr::get(const std::string& file)
{
    if(auto it = pool_.find(file); it != pool_.end())
        return it->second;

    auto& tex = pool_[file];

    namespace fs = std::filesystem;
    fs::path cwd  = fs::current_path();
    fs::path full = cwd / "assets" / file;
    if(!fs::exists(full))
        full = cwd.parent_path() / "assets" / file;

    if(!tex.loadFromFile(full.string()))
    {
        std::cerr << "[TextureMgr] cannot load " << full
                  << " – using placeholder\n";

        tex = makePlaceholder(
                file=="algae.png"     ? sf::Color::Green  :
                file=="herbivore.png" ? sf::Color::Yellow :
                file=="predator.png"  ? sf::Color::Red    :
                                        sf::Color(200,140,0));}
    tex.setSmooth(true);
    return tex;}
