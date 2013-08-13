#include "resourcemanager.h"
#include <iostream>

resourceManager::resourceManager():
    whitePrefix("White"),
    blackPrefix("Black"),
    boardSuffix("brown")
{
    thor::ResourceKey<sf::Texture> blackRookT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/BlackR.png");
    thor::ResourceKey<sf::Texture> blackBishopT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/BlackB.png");
    thor::ResourceKey<sf::Texture> blackKnightT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/BlackN.png");
    thor::ResourceKey<sf::Texture> blackQueenT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/BlackQ.png");
    thor::ResourceKey<sf::Texture> blackKingT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/BlackK.png");
    thor::ResourceKey<sf::Texture> blackPawnT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/BlackP.png");
    thor::ResourceKey<sf::Texture> whiteRookT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/WhiteR.png");
    thor::ResourceKey<sf::Texture> whiteBishopT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/WhiteB.png");
    thor::ResourceKey<sf::Texture> whiteKnightT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/WhiteN.png");
    thor::ResourceKey<sf::Texture> whiteQueenT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/WhiteQ.png");
    thor::ResourceKey<sf::Texture> whiteKingT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/WhiteK.png");
    thor::ResourceKey<sf::Texture> whitePawnT = thor::Resources::fromFile<sf::Texture>("Graphics/Pieces/WhiteP.png");

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

    }
    catch (thor::ResourceLoadingException& e)
    {
        std::cerr << e.what() << std::endl;
        //throw - the gui must catch it
    }
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
    }
}

void resourceManager::load()
{

}
