#include "pch.h"

#include "Referee.h"

Referee::Referee( IntraCom::IntraCom& a_intraCom )
    : m_rRegisterPlayer { a_intraCom.createReader< RegisterPlayer >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
    , m_rMove { a_intraCom.createReader< Move >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
    , m_wAssignedPlayer { a_intraCom.createWriter< AssignedPlayer >( ) }
    , m_wBoard { a_intraCom.createWriter< Board >() }
    , player_o { "", PlayerToken::PlayO }
    , player_x { "", PlayerToken::PlayX }
    , board { }
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
    if ( a_sample.token() == PlayerToken::PlayO )
    {
        board.setSquare( a_sample.row(), a_sample.col(), SquareState::HasO );
        board.setState( GameState::ToMoveX );
    }
    else if ( a_sample.token() == PlayerToken::PlayX )
    {
        board.setSquare( a_sample.row(), a_sample.col(), SquareState::HasX );
        board.setState( GameState::ToMoveO );
    }

    // Check for winner
    SquareState lookForWinner { board.square( a_sample.row(), a_sample.col() ) };
    bool hasWinner { false };
    // Check rows
    for ( int row = 0; row < 3; ++row )
        if ( board.square( row, 0 ) == lookForWinner &&
             board.square( row, 1 ) == lookForWinner &&
             board.square( row, 2 ) == lookForWinner )
        {
            hasWinner = true;
        }
    // Check columns
    for ( int col = 0; col < 3; ++col )
        if ( board.square( 0, col ) == lookForWinner &&
             board.square( 1, col ) == lookForWinner &&
             board.square( 2, col ) == lookForWinner )
        {
            hasWinner = true;
        }
    // Check diagonals
    if ( board.square( 0, 0 ) == lookForWinner &&
         board.square( 1, 1 ) == lookForWinner &&
         board.square( 2, 2 ) == lookForWinner )
    {
        hasWinner = true;
    }
    if ( board.square( 2, 0 ) == lookForWinner &&
         board.square( 1, 1 ) == lookForWinner &&
         board.square( 0, 2 ) == lookForWinner )
    {
        hasWinner = true;
    }

    if ( hasWinner )
        if ( lookForWinner == SquareState::HasO )
            board.setState( GameState::VictoryO );
        else
            board.setState( GameState::VictoryX );
    else
    {
        // Check for draw
        bool draw { true };
        for ( int row = 0; row < 3; ++row )
            for ( int col = 0; col < 3; ++col )
                if ( board.square( row, col ) == SquareState::Empty )
                    draw = false;
        if ( draw )
        {
            board.setState( GameState::Draw );
        }
    }
    m_wBoard->write( board );
}

void Referee::startGame()
{
    // Reset the board
    board = Board {};
    board.setState( GameState::ToMoveX );

    m_wBoard->write( board );
}
