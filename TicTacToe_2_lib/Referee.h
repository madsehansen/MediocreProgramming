#pragma once

#include "../IntraCom/IntraCom.h"

#include "Model.h"

class Referee
{
public:
    explicit Referee( IntraCom::IntraCom& a_intraCom );

private:
    void readData( IntraCom::Reader* a_reader );

    void handleRegisterPlayer( const RegisterPlayer& a_sample );
    void handleMove( const Move& a_sample );

    void startGame();

    IntraCom::DataReader< RegisterPlayer >* m_rRegisterPlayer { nullptr };
    IntraCom::DataReader< Move >* m_rMove { nullptr };

    IntraCom::DataWriter< AssignedPlayer >* m_wAssignedPlayer { nullptr };
    IntraCom::DataWriter< Board >*          m_wBoard { nullptr };

    GameData game;
};

