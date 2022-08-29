#pragma once

#include <stdexcept>
#include <stdint.h>
#include <string>

enum class GameState : int32_t
{
    ToMoveX,
    ToMoveO,
    Draw,
    VictoryX,
    VictoryO,
};

enum class SquareState : int32_t
{
    Empty,
    HasX,
    HasO,
};

enum class PlayerToken : int32_t
{
    PlayX,
    PlayO,
};

class Board
{
public:
    explicit Board()
        : m_state { GameState::ToMoveX }
        , m_squares {
            SquareState::Empty, SquareState::Empty, SquareState::Empty,
            SquareState::Empty, SquareState::Empty, SquareState::Empty,
            SquareState::Empty, SquareState::Empty, SquareState::Empty
    }
    {}

    explicit Board( GameState a_state,
        SquareState a_0_0, SquareState a_0_1, SquareState a_0_2,
        SquareState a_1_0, SquareState a_1_1, SquareState a_1_2,
        SquareState a_2_0, SquareState a_2_1, SquareState a_2_2 )
        : m_state { a_state }
        , m_squares {
            a_0_0, a_0_1, a_0_2,
            a_1_0, a_1_1, a_1_2,
            a_2_0, a_2_1, a_2_2
        }
    {}

    auto state() const -> GameState
    {
        return m_state;
    }

    void setState( GameState a_state )
    {
        m_state = a_state;
    }

    auto square(int row, int col) const -> SquareState
    {
        if ( row < 0 || row > 2 )
        {
            throw std::range_error( "row must be 0..2" );
        }
        if ( col < 0 || col > 2 )
        {
            throw std::range_error( "col must be 0..2" );
        }

        return m_squares[ row ][ col ];
    }

    void setSquare( int row, int col, SquareState state )
    {
        if ( row < 0 || row > 2 )
        {
            throw std::range_error( "row must be 0..2" );
        }
        if ( col < 0 || col > 2 )
        {
            throw std::range_error( "col must be 0..2" );
        }

        m_squares[ row ][ col ] = state;
    }

private:
    GameState m_state;
    SquareState m_squares[ 3 ][ 3 ];
};

class Move
{
public:
    explicit Move()
        : m_token { PlayerToken::PlayO }
        , m_row { 0 }
        , m_col { 0 }
    {}

    explicit Move( PlayerToken a_token, int a_row, int a_col )
        : m_token { a_token }
        , m_row { a_row }
        , m_col { a_col }
    {
        if ( a_row < 0 || a_row > 2 )
        {
            throw std::range_error( "a_row must be 0..2" );
        }
        if ( a_col < 0 || a_col > 2 )
        {
            throw std::range_error( "a_col must be 0..2" );
        }
    }

    auto token() const -> PlayerToken
    {
        return m_token;
    }

    auto row() const -> int
    {
        return m_row;
    }

    auto col() const -> int
    {
        return m_col;
    }

private:
    PlayerToken m_token;
    int m_row;
    int m_col;
};

struct RegisterPlayer
{
    std::string name;
};

struct AssignedPlayer
{
    std::string name;
    PlayerToken token;
};

struct GameData
{
    Board board;
    AssignedPlayer playerX;
    AssignedPlayer playerO;
};


inline auto ToString( PlayerToken a_token ) -> std::wstring
{
    switch ( a_token )
    {
    case PlayerToken::PlayX:
        return L"X";
    case PlayerToken::PlayO:
        return L"O";
    default:
        return L"Illegal PlayerToken";
    }
}


inline auto ToString( SquareState a_state ) -> std::wstring
{
    switch ( a_state )
    {
    case SquareState::Empty:
        return L" ";
    case SquareState::HasO:
        return L"O";
    case SquareState::HasX:
        return L"X";
    default:
        return L"Illegal SquareState";
    }
}

inline auto ToString( GameState a_state ) -> std::wstring
{
    switch ( a_state )
    {
    case GameState::ToMoveX:
        return L"ToMoveX";
    case GameState::ToMoveO:
        return L"ToMoveO";
    case GameState::Draw:
        return L"Draw";
    case GameState::VictoryX:
        return L"VictoryX";
    case GameState::VictoryO:
        return L"VictoryO";
    default:
        return L"Illegal GameState";
    }
}