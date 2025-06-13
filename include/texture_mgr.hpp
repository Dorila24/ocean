#ifndef TEXTURE_MGR_HPP
#define TEXTURE_MGR_HPP
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class TextureMgr {
public:
    static TextureMgr& inst();
    const sf::Texture& get(const std::string& file);

private:
    std::unordered_map<std::string,sf::Texture> pool_;
    TextureMgr() = default;};
#endif
