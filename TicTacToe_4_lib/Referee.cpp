#include "pch.h"

#include "Referee.h"

Referee::Referee(
    IDataWriter< AssignedPlayer >* a_wAssignedPlayer,
    IDataWriter< Board >* a_wBoard,
    GameData* a_game )
    : m_wAssignedPlayer { a_wAssignedPlayer }
    , m_wBoard { a_wBoard }
    , m_game { a_game }
{
    if ( m_game == nullptr )
        throw std::invalid_argument( "a_game must contain a valid object" );
}


void Referee::handleRegisterPlayer( const RegisterPlayer& a_sample )
{
    AssignedPlayer* sample { nullptr };
    if ( m_game->playerX.name.empty() )
        sample = &m_game->playerX;
    else if ( m_game->playerO.name.empty() )
        sample = &m_game->playerO;
    else
        return;

    sample->name = a_sample.name;

    m_wAssignedPlayer->write( *sample );

    if ( not m_game->playerX.name.empty() && not m_game->playerO.name.empty() )
        startGame();
}

void Referee::handleMove( const Move& a_sample )
{
    // Make move
    if ( a_sample.token() == PlayerToken::PlayO )
    {
        m_game->board.setSquare( a_sample.row(), a_sample.col(), SquareState::HasO );
        m_game->board.setState( GameState::ToMoveX );
    }
    else if ( a_sample.token() == PlayerToken::PlayX )
    {
        m_game->board.setSquare( a_sample.row(), a_sample.col(), SquareState::HasX );
        m_game->board.setState( GameState::ToMoveO );
    }

    // Check for winner
    SquareState lookForWinner { m_game->board.square( a_sample.row(), a_sample.col() ) };
    bool hasWinner { false };
    // Check rows
    for ( int row = 0; row < 3; ++row )
        if ( m_game->board.square( row, 0 ) == lookForWinner &&
             m_game->board.square( row, 1 ) == lookForWinner &&
             m_game->board.square( row, 2 ) == lookForWinner )
        {
            hasWinner = true;
        }
    // Check columns
    for ( int col = 0; col < 3; ++col )
        if ( m_game->board.square( 0, col ) == lookForWinner &&
             m_game->board.square( 1, col ) == lookForWinner &&
             m_game->board.square( 2, col ) == lookForWinner )
        {
            hasWinner = true;
        }
    // Check diagonals
    if ( m_game->board.square( 0, 0 ) == lookForWinner &&
         m_game->board.square( 1, 1 ) == lookForWinner &&
         m_game->board.square( 2, 2 ) == lookForWinner )
    {
        hasWinner = true;
    }
    if ( m_game->board.square( 2, 0 ) == lookForWinner &&
         m_game->board.square( 1, 1 ) == lookForWinner &&
         m_game->board.square( 0, 2 ) == lookForWinner )
    {
        hasWinner = true;
    }

    if ( hasWinner )
        if ( lookForWinner == SquareState::HasO )
            m_game->board.setState( GameState::VictoryO );
        else
            m_game->board.setState( GameState::VictoryX );
    else
    {
        // Check for draw
        bool draw { true };
        for ( int row = 0; row < 3; ++row )
            for ( int col = 0; col < 3; ++col )
                if ( m_game->board.square( row, col ) == SquareState::Empty )
                    draw = false;
        if ( draw )
        {
            m_game->board.setState( GameState::Draw );
        }
    }
    m_wBoard->write( m_game->board );
}

void Referee::startGame()
{
    // Reset the board
    m_game->board = Board {};
    m_game->board.setState( GameState::ToMoveX );

    m_wBoard->write( m_game->board );
}
