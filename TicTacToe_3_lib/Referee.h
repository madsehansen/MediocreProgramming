#pragma once

#include "IDataWriter.h"

#include "Model.h"

class Referee
{
public:
    explicit Referee( IDataWriter< AssignedPlayer >* a_wAssignedPlayer, IDataWriter< Board >* a_wBoard );

    void handleRegisterPlayer( const RegisterPlayer& a_sample );
    void handleMove( const Move& a_sample );

private:
    void startGame();

    IDataWriter< AssignedPlayer >* m_wAssignedPlayer { nullptr };
    IDataWriter< Board >*          m_wBoard { nullptr };

    GameData game;
};

