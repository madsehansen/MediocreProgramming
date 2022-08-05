// TicTacToe_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../IntraCom/IntraCom.h"

#include "Referee.h"
#include "FirstFreePlayer.h"

void printBoard( const Board& a_board )
{
    for ( int row = 0; row < 3; ++row )
    {
        for ( int col = 0; col < 3; ++col )
            std::cout << toString( a_board.squares[ row ][ col ] );
        std::cout << std::endl;
    }
    std::cout << toString( a_board.state ) << std::endl << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";

    bool stopRunning { false };

    IntraCom::IntraCom intracom;
    Referee referee { intracom };
    FirstFreePlayer player_one { intracom, "FFP 1"};
    FirstFreePlayer player_two { intracom, "FFP 2" };

    IntraCom::CommandReader< Board >* rBoard { intracom.getCommandReader< Board >( [&]( IntraCom::DataReader* a_reader ) {
        if ( a_reader == rBoard )
        {
            for ( const Board& b : rBoard->read() )
            {
                printBoard( b );
                if ( b.state == GameState::Draw )
                    stopRunning = true;
            }
        }
    } ) };


    intracom.start();

    while ( not stopRunning )
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    std::cout << "Bye World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
