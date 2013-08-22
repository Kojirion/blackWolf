#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <SFML/Graphics/Texture.hpp>
#include <Thor/Resources.hpp>
#include <boost/property_tree/ptree.hpp>

class resourceManager
{
public:
    enum class pieceType
    {
        blackRook, blackKnight, blackBishop, blackQueen, blackKing, blackPawn,
        whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing, whitePawn
    };

    resourceManager();

    const sf::Texture& typeToTexture(const int pieceType) const;


    void load();

    void reload(const std::string& theWhitePrefix,
        const std::string& theBlackPrefix, const std::string& theBoardSuffix);

    boost::property_tree::ptree getTree() const;

private:
    thor::MultiResourceCache cache;
    std::string whitePrefix;
    std::string blackPrefix;
    std::string boardSuffix;
    std::string pathToPieces;

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

    std::shared_ptr<sf::Texture> board;



};

/*thor::ResourceKey<sf::Texture> toResourceKey(resourceManager::pieceType type)
{
    if (type == resourceManager::pieceType::blackRook)
        return thor::Resources::fromFile<sf::Texture>("BlackR.png");
    else if (type == resourceManager::pieceType::blackBishop)
        return thor::Resources::fromFile<sf::Texture>("BlackB.png");
    else if (type == resourceManager::pieceType::blackKnight)
        return thor::Resources::fromFile<sf::Texture>("BlackN.png");
    else if (type == resourceManager::pieceType::blackQueen)
        return thor::Resources::fromFile<sf::Texture>("BlackQ.png");
    else if (type == resourceManager::pieceType::blackPawn)
        return thor::Resources::fromFile<sf::Texture>("BlackP.png");
    else if (type == resourceManager::pieceType::blackKing)
        return thor::Resources::fromFile<sf::Texture>("BlackK.png");
    else if (type == resourceManager::pieceType::whiteRook)
        return thor::Resources::fromFile<sf::Texture>("WhiteR.png");
    else if (type == resourceManager::pieceType::whiteBishop)
        return thor::Resources::fromFile<sf::Texture>("WhiteB.png");
    else if (type == resourceManager::pieceType::whiteKnight)
        return thor::Resources::fromFile<sf::Texture>("WhiteN.png");
    else if (type == resourceManager::pieceType::whiteQueen)
        return thor::Resources::fromFile<sf::Texture>("WhiteQ.png");
    else if (type == resourceManager::pieceType::whitePawn)
        return thor::Resources::fromFile<sf::Texture>("WhiteP.png");
    else if (type == resourceManager::pieceType::whiteKing)
        return thor::Resources::fromFile<sf::Texture>("WhiteK.png");
}*/

#endif // RESOURCEMANAGER_H
