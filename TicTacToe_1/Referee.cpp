#include "Referee.h"

Referee::Referee( IntraCom::IntraCom& a_intraCom )
    : m_rRegisterPlayer { a_intraCom.getCommandReader< RegisterPlayer >( [this]( IntraCom::DataReader* a_reader ) { readData( a_reader ); } ) }
    , m_rMove { a_intraCom.getCommandReader< Move >( [this]( IntraCom::DataReader* a_reader ) { readData( a_reader ); } ) }
    , m_wAssignedPlayer { a_intraCom.getCommandWriter< AssignedPlayer >( ) }
    , m_wBoard { a_intraCom.getCommandWriter< Board >() }
{
    player_o.token = PlayerToken::PlayO;
    player_x.token = PlayerToken::PlayX;
}

void Referee::readData( IntraCom::DataReader* a_reader )
{
    if ( a_reader == m_rRegisterPlayer )
    {
        for ( RegisterPlayer& sample : m_rRegisterPlayer->read() )
            handleRegisterPlayer( sample );
    }
    if ( a_reader == m_rMove )
    {
        for ( Move& sample : m_rMove->read() )
            handleMove( sample );
    }
}

void Referee::handleRegisterPlayer( const RegisterPlayer& a_sample )
{
    AssignedPlayer* sample { nullptr };
    if ( player_x.name.empty() )
        sample = &player_x;
    else if ( player_o.name.empty() )
        sample = &player_o;
    else
        return;

    sample->name = a_sample.name;

    m_wAssignedPlayer->write( *sample );

    if ( not player_x.name.empty() && not player_o.name.empty() )
        startGame();
}

void Referee::handleMove( const Move& a_sample )
{
    if ( a_sample.token == PlayerToken::PlayO )
    {
        board.squares[ a_sample.row ][ a_sample.col ] = SquareState::HasO;
        board.state = GameState::ToMoveX;
    }
    else if ( a_sample.token == PlayerToken::PlayX )
    {
        board.squares[ a_sample.row ][ a_sample.col ] = SquareState::HasX;
        board.state = GameState::ToMoveO;
    }

    bool done { true };
    for ( int row = 0; row < 3; ++row )
        for ( int col = 0; col < 3; ++col )
            if ( board.squares[ row ][ col ] == SquareState::Empty )
                done = false;
    if ( done )
    {
        board.state = GameState::Draw;
    }

    m_wBoard->write( board );
}

void Referee::startGame()
{
    // Reset the board
    board = Board {};
    board.state = GameState::ToMoveX;

    m_wBoard->write( board );
}
