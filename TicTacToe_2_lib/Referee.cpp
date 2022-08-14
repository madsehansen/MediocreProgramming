#include "pch.h"

#include "Referee.h"

Referee::Referee( IntraCom::IntraCom& a_intraCom )
    : m_rRegisterPlayer { a_intraCom.createReader< RegisterPlayer >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
    , m_rMove { a_intraCom.createReader< Move >( [this]( IntraCom::Reader* a_reader ) { readData( a_reader ); } ) }
    , m_wAssignedPlayer { a_intraCom.createWriter< AssignedPlayer >( ) }
    , m_wBoard { a_intraCom.createWriter< Board >() }
    , game { Board{ }, AssignedPlayer{ "", PlayerToken::PlayX }, AssignedPlayer{ "", PlayerToken::PlayO } }
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
    if ( game.playerX.name.empty() )
        sample = &game.playerX;
    else if ( game.playerO.name.empty() )
        sample = &game.playerO;
    else
        return;

    sample->name = a_sample.name;

    m_wAssignedPlayer->write( *sample );

    if ( not game.playerX.name.empty() && not game.playerO.name.empty() )
        startGame();
}

void Referee::handleMove( const Move& a_sample )
{
    // Make move
    if ( a_sample.token() == PlayerToken::PlayO )
    {
        game.board.setSquare( a_sample.row(), a_sample.col(), SquareState::HasO );
        game.board.setState( GameState::ToMoveX );
    }
    else if ( a_sample.token() == PlayerToken::PlayX )
    {
        game.board.setSquare( a_sample.row(), a_sample.col(), SquareState::HasX );
        game.board.setState( GameState::ToMoveO );
    }

    // Check for winner
    SquareState lookForWinner { game.board.square( a_sample.row(), a_sample.col() ) };
    bool hasWinner { false };
    // Check rows
    for ( int row = 0; row < 3; ++row )
        if ( game.board.square( row, 0 ) == lookForWinner &&
             game.board.square( row, 1 ) == lookForWinner &&
             game.board.square( row, 2 ) == lookForWinner )
        {
            hasWinner = true;
        }
    // Check columns
    for ( int col = 0; col < 3; ++col )
        if ( game.board.square( 0, col ) == lookForWinner &&
             game.board.square( 1, col ) == lookForWinner &&
             game.board.square( 2, col ) == lookForWinner )
        {
            hasWinner = true;
        }
    // Check diagonals
    if ( game.board.square( 0, 0 ) == lookForWinner &&
         game.board.square( 1, 1 ) == lookForWinner &&
         game.board.square( 2, 2 ) == lookForWinner )
    {
        hasWinner = true;
    }
    if ( game.board.square( 2, 0 ) == lookForWinner &&
         game.board.square( 1, 1 ) == lookForWinner &&
         game.board.square( 0, 2 ) == lookForWinner )
    {
        hasWinner = true;
    }

    if ( hasWinner )
        if ( lookForWinner == SquareState::HasO )
            game.board.setState( GameState::VictoryO );
        else
            game.board.setState( GameState::VictoryX );
    else
    {
        // Check for draw
        bool draw { true };
        for ( int row = 0; row < 3; ++row )
            for ( int col = 0; col < 3; ++col )
                if ( game.board.square( row, col ) == SquareState::Empty )
                    draw = false;
        if ( draw )
        {
            game.board.setState( GameState::Draw );
        }
    }
    m_wBoard->write( game.board );
}

void Referee::startGame()
{
    // Reset the board
    game.board = Board {};
    game.board.setState( GameState::ToMoveX );

    m_wBoard->write( game.board );
}
