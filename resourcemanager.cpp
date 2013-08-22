#include "resourcemanager.h"
#include <iostream>

resourceManager::resourceManager():
    whitePrefix("Blue2"),
    blackPrefix("Blue"),
    boardSuffix("Black"),
    pathToPieces("Graphics/Pieces/")
{
    load();
}

const sf::Texture &resourceManager::typeToTexture(const int pieceType) const
{
    switch (pieceType) {
    case 1:
        return *whiteRook;
    case 2:
        return *whiteBishop;
    case 3:
        return *whiteKnight;
    case 4:
        return *whiteQueen;
    case 5:
        return *whitePawn;
    case 6:
        return *whiteKing;
    case -1:
        return *blackRook;
    case -2:
        return *blackBishop;
    case -3:
        return *blackKnight;
    case -4:
        return *blackQueen;
    case -5:
        return *blackPawn;
    case -6:
        return *blackKing;
    case 10: //let 10 denote the board for now
        return *board;
    }
}

void resourceManager::load()
{
    thor::ResourceKey<sf::Texture> blackRookT = thor::Resources::fromFile<sf::Texture>(pathToPieces + blackPrefix + "R.png");
    thor::ResourceKey<sf::Texture> blackBishopT = thor::Resources::fromFile<sf::Texture>(pathToPieces + blackPrefix + "B.png");
    thor::ResourceKey<sf::Texture> blackKnightT = thor::Resources::fromFile<sf::Texture>(pathToPieces + blackPrefix + "N.png");
    thor::ResourceKey<sf::Texture> blackQueenT = thor::Resources::fromFile<sf::Texture>(pathToPieces + blackPrefix + "Q.png");
    thor::ResourceKey<sf::Texture> blackKingT = thor::Resources::fromFile<sf::Texture>(pathToPieces + blackPrefix + "K.png");
    thor::ResourceKey<sf::Texture> blackPawnT = thor::Resources::fromFile<sf::Texture>(pathToPieces + blackPrefix + "P.png");
    thor::ResourceKey<sf::Texture> whiteRookT = thor::Resources::fromFile<sf::Texture>(pathToPieces + whitePrefix + "R.png");
    thor::ResourceKey<sf::Texture> whiteBishopT = thor::Resources::fromFile<sf::Texture>(pathToPieces + whitePrefix + "B.png");
    thor::ResourceKey<sf::Texture> whiteKnightT = thor::Resources::fromFile<sf::Texture>(pathToPieces + whitePrefix + "N.png");
    thor::ResourceKey<sf::Texture> whiteQueenT = thor::Resources::fromFile<sf::Texture>(pathToPieces + whitePrefix + "Q.png");
    thor::ResourceKey<sf::Texture> whiteKingT = thor::Resources::fromFile<sf::Texture>(pathToPieces + whitePrefix + "K.png");
    thor::ResourceKey<sf::Texture> whitePawnT = thor::Resources::fromFile<sf::Texture>(pathToPieces + whitePrefix + "P.png");

    thor::ResourceKey<sf::Texture> boardTexture = thor::Resources::fromFile<sf::Texture>("Graphics/Board" + boardSuffix + ".jpg");

    try
    {
        whiteRook = cache.acquire(whiteRookT);
        whiteBishop = cache.acquire(whiteBishopT);
        whiteKnight = cache.acquire(whiteKnightT);
        whiteQueen = cache.acquire(whiteQueenT);
        whitePawn = cache.acquire(whitePawnT);
        whiteKing = cache.acquire(whiteKingT);
        blackRook = cache.acquire(blackRookT);
        blackBishop = cache.acquire(blackBishopT);
        blackKnight = cache.acquire(blackKnightT);
        blackQueen = cache.acquire(blackQueenT);
        blackPawn = cache.acquire(blackPawnT);
        blackKing = cache.acquire(blackKingT);

        board = cache.acquire(boardTexture);
    }
    catch (thor::ResourceLoadingException& e)
    {
        std::cerr << e.what() << std::endl;
        //throw - the gui must catch it
    }
}

void resourceManager::reload(const std::string &theWhitePrefix, const std::string &theBlackPrefix, const std::string &theBoardSuffix)
{
    whitePrefix = theWhitePrefix;
    blackPrefix = theBlackPrefix;
    boardSuffix = theBoardSuffix;

    load();
}

boost::property_tree::ptree resourceManager::getTree() const
{
    boost::property_tree::ptree pt;
    pt.put("whitePrefix",whitePrefix);
    pt.put("blackPrefix",blackPrefix);
    pt.put("boardSuffix",boardSuffix);
    return pt;
}
