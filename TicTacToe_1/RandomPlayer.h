#pragma once

#include "../IntraCom/IntraCom.h"

#include "Model.h"

class RandomPlayer
{
public:
    explicit RandomPlayer(
        IntraCom::IntraCom& a_intraCom,
        const std::string& a_name );

private:
    void readData( IntraCom::DataReader* a_reader );

    void handleAssignedPlayer( const AssignedPlayer& a_sample );
    void handleBoard( const Board& a_sample );

    IntraCom::CommandReader< AssignedPlayer >* m_rAssignedPlayer { nullptr };
    IntraCom::CommandReader< Board >* m_rBoard { nullptr };

    IntraCom::CommandWriter< RegisterPlayer >* m_wRegisterPlayer { nullptr };
    IntraCom::CommandWriter< Move >* m_wMove { nullptr };

    std::string m_myName { "RandomPlayer" };
    PlayerToken m_myToken;
};

