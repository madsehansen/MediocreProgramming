#include "FirstFreePlayer.h"

FirstFreePlayer::FirstFreePlayer(
    IntraCom::IntraCom& a_intraCom,
    const std::string& a_name )
    : m_rAssignedPlayer { a_intraCom.getCommandReader< AssignedPlayer >( [this]( IntraCom::DataReader* a_reader ) { readData( a_reader ); } ) }
    , m_rBoard { a_intraCom.getCommandReader< Board >( [this]( IntraCom::DataReader* a_reader ) { readData( a_reader ); } ) }
    , m_wRegisterPlayer { a_intraCom.getCommandWriter< RegisterPlayer >( ) }
    , m_wMove { a_intraCom.getCommandWriter< Move >() }
    , m_myName { a_name }
{
    RegisterPlayer sample;
    sample.name = m_myName;

    m_wRegisterPlayer->write( sample );
}

void FirstFreePlayer::readData( IntraCom::DataReader* a_reader )
{
    if ( a_reader == m_rAssignedPlayer )
    {
        for ( AssignedPlayer& sample : m_rAssignedPlayer->read() )
            handleAssignedPlayer( sample );
    }
    if ( a_reader == m_rBoard )
    {
        for ( Board& sample : m_rBoard->read() )
            handleBoard( sample );
    }
}

void FirstFreePlayer::handleAssignedPlayer( const AssignedPlayer& a_sample )
{
    if ( a_sample.name != m_myName )
        return;

    m_myToken = a_sample.token;
}

void FirstFreePlayer::handleBoard( const Board& a_sample )
{
    if ( ( a_sample.state == GameState::ToMoveX && m_myToken == PlayerToken::PlayX ) ||
        ( a_sample.state == GameState::ToMoveO && m_myToken == PlayerToken::PlayO ) )
    {
        Move myMove {};
        myMove.token = m_myToken;

        for ( myMove.row = 0; myMove.row < 3; myMove.row++ )
            for ( myMove.col = 0; myMove.col < 3; myMove.col++ )
                if ( a_sample.squares[ myMove.row ][ myMove.col ] == SquareState::Empty )
                {
                    m_wMove->write( myMove );
                    return;
                }
    }
}
