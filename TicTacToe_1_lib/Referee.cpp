#include "pch.h"

#include "Referee.h"

Referee::Referee( IntraCom::IntraCom& a_intraCom )
    : m_rRegisterPlayer { a_intraCom.createReader< RegisterPlayer >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
    , m_rMove { a_intraCom.createReader< Move >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
    , m_wAssignedPlayer { a_intraCom.createWriter< AssignedPlayer >( ) }
    , m_wBoard { a_intraCom.createWriter< Board >() }
    , player_o { "", PlayerToken::PlayO }
    , player_x { "", PlayerToken::PlayX }
    , board { GameState::Draw, {} }
{
}

void Referee::readData( IntraCom::Reader* a_reader )
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
    // Make move
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

    // Check for draw
    bool done { true };
    for ( int row = 0; row < 3; ++row )
        for ( int col = 0; col < 3; ++col )
            if ( board.squares[ row ][ col ] == SquareState::Empty )
                done = false;
    if ( done )
    {
        board.state = GameState::Draw;
    }

    // Check for winner, draw may have been set incorrectly, so we cannot check that
    SquareState lookForWinner { board.squares[ a_sample.row ][ a_sample.col ] };
    bool hasWinner { false };
    // Check rows
    for ( int row = 0; row < 3; ++row )
        if ( board.squares[ row ][ 0 ] == lookForWinner &&
             board.squares[ row ][ 1 ] == lookForWinner &&
             board.squares[ row ][ 2 ] == lookForWinner )
            {
                hasWinner = true;
            }
    // Check columns
    for ( int col = 0; col < 3; ++col )
        if ( board.squares[ 0 ][ col ] == lookForWinner &&
             board.squares[ 1 ][ col ] == lookForWinner &&
             board.squares[ 2 ][ col ] == lookForWinner )
        {
            hasWinner = true;
        }
    // Check diagonals
    if ( board.squares[ 0 ][ 0 ] == lookForWinner &&
         board.squares[ 1 ][ 1 ] == lookForWinner &&
         board.squares[ 2 ][ 2 ] == lookForWinner )
    {
        hasWinner = true;
    }
    if ( board.squares[ 2 ][ 0 ] == lookForWinner &&
         board.squares[ 1 ][ 1 ] == lookForWinner &&
         board.squares[ 0 ][ 2 ] == lookForWinner )
    {
        hasWinner = true;
    }

    if ( hasWinner )
        if ( lookForWinner == SquareState::HasO )
            board.state = GameState::VictoryO;
        else
            board.state = GameState::VictoryX;

    m_wBoard->write( board );
}

void Referee::startGame()
{
    // Reset the board
    board = Board {};
    board.state = GameState::ToMoveX;

    m_wBoard->write( board );
}
