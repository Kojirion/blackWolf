#pragma once
#include "../Common/Color.hpp"
#include "../messages/Entity.hpp"

class GameData : private Messages::Registrant
{
public:
    GameData();

    bool userTurn() const;
    Color getUserColor() const;

private:
    Color m_userColor;
    Color m_turnColor;
};
