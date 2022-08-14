#include "pch.h"

#include "RandomPlayer.h"

#include <vector>

RandomPlayer::RandomPlayer(
    IDataWriter< RegisterPlayer >* a_wRegisterPlayer,
    IDataWriter< Move >* a_wMove,
    AssignedPlayer* a_name )
    : m_wRegisterPlayer { a_wRegisterPlayer }
    , m_wMove { a_wMove }
    , m_data { a_name }
{
    if ( m_data == nullptr )
        throw std::invalid_argument( "a_name must contain a valid and unique name" );

    RegisterPlayer sample;
    sample.name = m_data->name;

    m_wRegisterPlayer->write( sample );
}

void RandomPlayer::handleAssignedPlayer( const AssignedPlayer& a_sample )
{
    if ( a_sample.name != m_data->name )
        return;

    m_data->token = a_sample.token;
}

void RandomPlayer::handleBoard( const Board& a_sample )
{
    if ( ( a_sample.state() == GameState::ToMoveX && m_data->token == PlayerToken::PlayX ) ||
        ( a_sample.state() == GameState::ToMoveO && m_data->token == PlayerToken::PlayO ) )
    {
        std::vector< std::pair< int, int > > freeSquares;

        for (int row = 0; row < 3; row++ )
            for ( int col = 0; col < 3; col++ )
                if ( a_sample.square( row, col ) == SquareState::Empty )
                    freeSquares.emplace_back( row, col );

        if ( freeSquares.size() > 0 )
        {
            int selected = std::rand() % freeSquares.size();
            Move myMove { m_data->token, freeSquares[ selected ].first, freeSquares[ selected ].second };

            m_wMove->write( myMove );
        }
    }
}
