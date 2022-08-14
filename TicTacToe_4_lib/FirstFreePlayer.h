#pragma once

#include "IDataWriter.h"

#include "Model.h"

class FirstFreePlayer
{
public:
    explicit FirstFreePlayer(
        IDataWriter< RegisterPlayer >* a_wRegisterPlayer,
        IDataWriter< Move >* a_wMove,
        AssignedPlayer* a_name );

    void handleAssignedPlayer( const AssignedPlayer& a_sample );
    void handleBoard( const Board& a_sample );

private:
    IDataWriter< RegisterPlayer >* m_wRegisterPlayer { nullptr };
    IDataWriter< Move >* m_wMove { nullptr };

    AssignedPlayer* m_data { nullptr };
};

