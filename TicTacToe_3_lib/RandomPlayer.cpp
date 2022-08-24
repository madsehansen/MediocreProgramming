#include "pch.h"

#include "RandomPlayer.h"

#include <vector>

RandomPlayer::RandomPlayer(
    IDataWriter< RegisterPlayer >* a_wRegisterPlayer,
    IDataWriter< Move >* a_wMove,
    IRandom* a_random,
    const std::string& a_name )
    : m_wRegisterPlayer { a_wRegisterPlayer }
    , m_wMove { a_wMove }
    , m_myName { a_name }
    , m_random { a_random }
{
    RegisterPlayer sample;
    sample.name = m_myName;

    m_wRegisterPlayer->write( sample );
}

void RandomPlayer::handleAssignedPlayer( const AssignedPlayer& a_sample )
{
    if ( a_sample.name != m_myName )
        return;

    m_myToken = a_sample.token;

    m_random->seed();
}

void RandomPlayer::handleBoard( const Board& a_sample )
{
    if ( ( a_sample.state() == GameState::ToMoveX && m_myToken == PlayerToken::PlayX ) ||
        ( a_sample.state() == GameState::ToMoveO && m_myToken == PlayerToken::PlayO ) )
    {
        std::vector< std::pair< int, int > > freeSquares;

        for (int row = 0; row < 3; row++ )
            for ( int col = 0; col < 3; col++ )
                if ( a_sample.square( row, col ) == SquareState::Empty )
                    freeSquares.emplace_back( row, col );

        if ( freeSquares.size() > 0 )
        {
            int selected = m_random->getNext() % freeSquares.size();
            Move myMove { m_myToken, freeSquares[ selected ].first, freeSquares[ selected ].second };

            m_wMove->write( myMove );
        }
    }
}
