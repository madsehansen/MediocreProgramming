#pragma once

#include "IDataWriter.h"
#include "IRandom.h"

#include "Model.h"

class RandomPlayer
{
public:
    explicit RandomPlayer(
        IDataWriter< RegisterPlayer >* a_wRegisterPlayer,
        IDataWriter< Move >* a_wMove,
        IRandom* a_random,
        const std::string& a_name );

    void handleAssignedPlayer( const AssignedPlayer& a_sample );
    void handleBoard( const Board& a_sample );

private:
    IDataWriter< RegisterPlayer >* m_wRegisterPlayer { nullptr };
    IDataWriter< Move >* m_wMove { nullptr };
    IRandom* m_random { nullptr };

    std::string m_myName { "RandomPlayer" };
    PlayerToken m_myToken;
};

