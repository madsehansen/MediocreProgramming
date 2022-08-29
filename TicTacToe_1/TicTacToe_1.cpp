// TicTacToe_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../IntraCom/IntraCom.h"

#include "../TicTacToe_1_lib/Referee.h"
#include "../TicTacToe_1_lib/FirstFreePlayer.h"
#include "../TicTacToe_1_lib/RandomPlayer.h"

void printBoard( const Board& a_board )
{
    std::cout << std::endl << ToString( a_board.state ) << std::endl;

    for ( int row { 0 }; row < 3; ++row )
    {
        for ( int col { 0 }; col < 3; ++col )
        {
            std::cout << ToString( a_board.squares[ row ][ col ] );
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "Start game!\n";

    bool stopRunning { false };

    IntraCom::IntraCom intracom;
    Referee referee { intracom };
    FirstFreePlayer player_one { intracom, "FFP 1"};
    RandomPlayer    player_two { intracom, "RP 2" };

    IntraCom::DataReader< Board >* rBoard { intracom.createReader< Board >( [&]( IntraCom::Reader* a_reader ) {
        if ( a_reader == rBoard )
        {
            for ( const Board& b : rBoard->read() )
            {
                printBoard( b );
                if ( b.state == GameState::Draw ||
                    b.state == GameState::VictoryO ||
                    b.state == GameState::VictoryX )
                {
                    stopRunning = true;
                }
            }
        }
    } ) };


    intracom.start();

    while ( not stopRunning )
    {
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    }

    std::cout << "Game ended!\n";
}

