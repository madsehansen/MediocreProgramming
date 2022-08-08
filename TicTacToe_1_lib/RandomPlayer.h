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
    void readData( IntraCom::Reader* a_reader );

    void handleAssignedPlayer( const AssignedPlayer& a_sample );
    void handleBoard( const Board& a_sample );

    IntraCom::DataReader< AssignedPlayer >* m_rAssignedPlayer { nullptr };
    IntraCom::DataReader< Board >* m_rBoard { nullptr };

    IntraCom::DataWriter< RegisterPlayer >* m_wRegisterPlayer { nullptr };
    IntraCom::DataWriter< Move >* m_wMove { nullptr };

    std::string m_myName { "RandomPlayer" };
    PlayerToken m_myToken;
};

