#pragma once

#include "../IntraCom/IntraCom.h"

#include "Model.h"

class Referee
{
public:
    explicit Referee( IntraCom::IntraCom& a_intraCom );

private:
    void readData( IntraCom::DataReader* a_reader );

    void handleRegisterPlayer( const RegisterPlayer& a_sample );
    void handleMove( const Move& a_sample );

    void startGame();

    IntraCom::CommandReader< RegisterPlayer >* m_rRegisterPlayer { nullptr };
    IntraCom::CommandReader< Move >* m_rMove { nullptr };

    IntraCom::CommandWriter< AssignedPlayer >* m_wAssignedPlayer { nullptr };
    IntraCom::CommandWriter< Board >*          m_wBoard { nullptr };

    AssignedPlayer player_x;
    AssignedPlayer player_o;
    Board board;
};

