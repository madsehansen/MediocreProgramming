#include "pch.h"

#include "RandomPlayer.h"

#include <vector>
#include <time.h>

RandomPlayer::RandomPlayer(
    IntraCom::IntraCom& a_intraCom,
    const std::string& a_name )
    : m_rAssignedPlayer { a_intraCom.createReader< AssignedPlayer >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
, m_rBoard { a_intraCom.createReader< Board >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
, m_wRegisterPlayer { a_intraCom.createWriter< RegisterPlayer >() }
, m_wMove { a_intraCom.createWriter< Move >() }
, m_myName { a_name }
{
    RegisterPlayer sample;
    sample.name = m_myName;

    m_wRegisterPlayer->write( sample );
}

void RandomPlayer::readData( IntraCom::Reader* a_reader )
{
    if ( a_reader == m_rAssignedPlayer )
    {
        for ( AssignedPlayer& sample : m_rAssignedPlayer->read() )
        {
            handleAssignedPlayer( sample );
        }
    }
    if ( a_reader == m_rBoard )
    {
        for ( Board& sample : m_rBoard->read() )
        {
            handleBoard( sample );
        }
    }
}

void RandomPlayer::handleAssignedPlayer( const AssignedPlayer& a_sample )
{
    if ( a_sample.name != m_myName )
    {
        return;
    }

    m_myToken = a_sample.token;

    time_t t { time( nullptr ) };
    std::srand( static_cast<int>( t ) );
}

void RandomPlayer::handleBoard( const Board& a_sample )
{
    if ( ( a_sample.state == GameState::ToMoveX && m_myToken == PlayerToken::PlayX ) ||
        ( a_sample.state == GameState::ToMoveO && m_myToken == PlayerToken::PlayO ) )
    {
        Move myMove {};
        myMove.token = m_myToken;

        std::vector< std::pair< int, int > > freeSquares;

        for ( int row { 0 }; row < 3; row++ )
        {
            for ( int col { 0 }; col < 3; col++ )
            {
                if ( a_sample.squares[ row ][ col ] == SquareState::Empty )
                {
                    freeSquares.emplace_back( row, col );
                }
            }
        }

        if ( freeSquares.size() > 0 )
        {
            auto selected { std::rand() % freeSquares.size() };
            myMove.row = freeSquares[ selected ].first;
            myMove.col = freeSquares[ selected ].second;

            m_wMove->write( myMove );
        }
    }
}
