#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <SFML/Graphics/Texture.hpp>
#include <Thor/Resources.hpp>

class resourceManager
{
private:
    thor::MultiResourceCache cache;
    std::string whitePrefix;
    std::string blackPrefix;
    std::string boardSuffix;

    std::shared_ptr<sf::Texture>    whiteRook;
    std::shared_ptr<sf::Texture>    whiteBishop;
    std::shared_ptr<sf::Texture>    whiteKnight;
    std::shared_ptr<sf::Texture>    whiteQueen;
    std::shared_ptr<sf::Texture>    whitePawn;
    std::shared_ptr<sf::Texture>    whiteKing;
    std::shared_ptr<sf::Texture>    blackRook;
    std::shared_ptr<sf::Texture>    blackBishop;
    std::shared_ptr<sf::Texture>    blackKnight;
    std::shared_ptr<sf::Texture>    blackQueen;
    std::shared_ptr<sf::Texture>    blackPawn;
    std::shared_ptr<sf::Texture>    blackKing;


public:
    resourceManager();

    const sf::Texture& getWhiteRook() const;
    const sf::Texture& getWhiteBishop() const;
    const sf::Texture& getWhiteKnight() const;
    const sf::Texture& getWhiteQueen() const;
    const sf::Texture& getWhitePawn() const;
    const sf::Texture& getWhiteKing() const ;
    const sf::Texture& getBlackRook() const;
    const sf::Texture& getBlackBishop() const;
    const sf::Texture& getBlackKnight() const;
    const sf::Texture& getBlackQueen() const;
    const sf::Texture& getBlackPawn() const;
    const sf::Texture& getBlack() const;


    void load();
};

#endif // RESOURCEMANAGER_H
