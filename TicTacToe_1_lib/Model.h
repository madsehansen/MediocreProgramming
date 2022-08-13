#pragma once
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

struct Board
{
    GameState state;
    SquareState squares[ 3 ][ 3 ];
};

struct Move
{
    PlayerToken token;
    int row;
    int col;
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



inline auto ToString( PlayerToken a_token ) -> std::string
{
    switch ( a_token )
    {
    case PlayerToken::PlayX:
        return "X";
    case PlayerToken::PlayO:
        return "O";
    default:
        return "Illegal PlayerToken";
    }
}


inline auto ToString( SquareState a_state ) -> std::string
{
    switch ( a_state )
    {
    case SquareState::Empty:
        return " ";
    case SquareState::HasO:
        return "O";
    case SquareState::HasX:
        return "X";
    default:
        return "Illegal SquareState";
    }
}

inline auto ToString( GameState a_state ) -> std::string
{
    switch ( a_state )
    {
    case GameState::ToMoveX:
        return "ToMoveX";
    case GameState::ToMoveO:
        return "ToMoveO";
    case GameState::Draw:
        return "Draw";
    case GameState::VictoryX:
        return "VictoryX";
    case GameState::VictoryO:
        return "VictoryO";
    default:
        return "Illegal GameState";
    }
}