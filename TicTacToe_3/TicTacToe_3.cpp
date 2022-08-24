// TicTacToe_3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../IntraCom/IntraCom.h"

#include "../TicTacToe_3_lib/DataReader.h"
#include "../TicTacToe_3_lib/DataWriter.h"
#include "../TicTacToe_3_lib/Random.h"

#include "../TicTacToe_3_lib/Referee.h"
#include "../TicTacToe_3_lib/FirstFreePlayer.h"
#include "../TicTacToe_3_lib/RandomPlayer.h"

void printBoard( const Board& a_board )
{
    std::wcout << std::endl << ToString( a_board.state() ) << std::endl;
    for ( int row = 0; row < 3; ++row )
    {
        for ( int col = 0; col < 3; ++col )
            std::wcout << ToString( a_board.square( row, col ) );
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "Start game!\n";

    bool stopRunning { false };

    IntraCom::IntraCom intracom;

    DataWriter< AssignedPlayer > wReferee_AssignedPlayer { intracom };
    DataWriter< Board > wReferee_Board { intracom };

    DataWriter<RegisterPlayer> wPlayer_one_RegisterPlayer { intracom };
    DataWriter<Move> wPlayer_one_Move { intracom };

    DataWriter<RegisterPlayer> wPlayer_two_RegisterPlayer { intracom };
    DataWriter<Move> wPlayer_two_Move { intracom };

    Random random;

    Referee referee { &wReferee_AssignedPlayer, &wReferee_Board  };
    FirstFreePlayer player_one { &wPlayer_one_RegisterPlayer, &wPlayer_one_Move, "FFP 1" };
    RandomPlayer    player_two { &wPlayer_one_RegisterPlayer, &wPlayer_one_Move, &random, "RP 2" };

    DataReader< Referee, RegisterPlayer > referee_RegisterPlayer { intracom, &referee, &Referee::handleRegisterPlayer };
    DataReader< Referee, Move > referee_Move { intracom, &referee, &Referee::handleMove };

    DataReader< FirstFreePlayer, AssignedPlayer > player_one_AssignedPlayer { intracom, &player_one, &FirstFreePlayer::handleAssignedPlayer };
    DataReader< FirstFreePlayer, Board > player_one_Board { intracom, &player_one, &FirstFreePlayer::handleBoard };

    DataReader< RandomPlayer, AssignedPlayer > player_two_AssignedPlayer { intracom, &player_two, &RandomPlayer::handleAssignedPlayer };
    DataReader< RandomPlayer, Board > player_two_Board { intracom, &player_two, &RandomPlayer::handleBoard };

    IntraCom::DataReader< Board >* rBoard { intracom.createReader< Board >( [&]( IntraCom::Reader* a_reader ) {
        if ( a_reader == rBoard )
        {
            for ( const Board& b : rBoard->read() )
            {
                printBoard( b );
                if ( b.state() == GameState::Draw ||
                     b.state() == GameState::VictoryO ||
                     b.state() == GameState::VictoryX )
                    stopRunning = true;
            }
        }
    } ) };


    intracom.start();

    while ( not stopRunning )
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    std::cout << "Game ended!\n";
}

