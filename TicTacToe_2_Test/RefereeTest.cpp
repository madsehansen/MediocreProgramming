#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_2_lib/Referee.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe2Test
{
	TEST_CLASS( RefereeTest )
	{
		std::unique_ptr<IntraCom::IntraCom> m_intraCom;
		IntraCom::DataReader< AssignedPlayer >* m_rAssignedPlayer { nullptr };
		IntraCom::DataReader< Board >* m_rBoard { nullptr };

		IntraCom::DataWriter< RegisterPlayer >* m_wRegisterPlayer { nullptr };
		IntraCom::DataWriter< Move >* m_wMove { nullptr };

		std::unique_ptr<Referee> m_sut;

		std::vector< AssignedPlayer > m_assignedPlayers;
		void readAssignedPlayer( const std::vector< AssignedPlayer >& a_data )
		{
			m_assignedPlayers.insert(
				m_assignedPlayers.end(),
				a_data.begin(),
				a_data.end()
			);
		}

		std::vector< Board > m_boards;
		void readBoard( const std::vector< Board >& a_data )
		{
			m_boards.insert(
				m_boards.end(),
				a_data.begin(),
				a_data.end()
			);
		}

		template< typename T >
		bool waitForData( const T& data, size_t a_target = 1 )
		{
			for ( int tries { 0 }; tries < 10; ++tries )
			{
				if ( data.size() < a_target )
					std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
			}

			return data.size() >= a_target;
		}

	public:

		TEST_METHOD_INITIALIZE( setup )
		{
			m_intraCom = std::make_unique<IntraCom::IntraCom>();

			m_rAssignedPlayer = m_intraCom->createReader< AssignedPlayer >( [this]( IntraCom::Reader* ) { this->readAssignedPlayer( this->m_rAssignedPlayer->read() ); } );
			m_rBoard = m_intraCom->createReader< Board >( [this]( IntraCom::Reader* ) { this->readBoard( this->m_rBoard->read() ); } );

			m_wRegisterPlayer = m_intraCom->createWriter< RegisterPlayer >();
			m_wMove = m_intraCom->createWriter< Move >();

			m_sut = std::make_unique<Referee>( *m_intraCom );

			m_intraCom->start();
		}
		TEST_METHOD_CLEANUP( cleanup )
		{
			m_intraCom->stop();

			m_sut.release();
			m_rAssignedPlayer = nullptr;
			m_assignedPlayers.clear();
			m_rBoard = nullptr;
			m_boards.clear();

			m_wRegisterPlayer = nullptr;
			m_wMove = nullptr;

			m_intraCom.release();
		}

		TEST_METHOD( RegisteringTwoPlayersAsXAndO )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );
			Assert::AreEqual( std::string( "Player 1" ), m_assignedPlayers[ 0 ].name );
			Assert::IsTrue( PlayerToken::PlayX == m_assignedPlayers[ 0 ].token );
			Assert::AreEqual( std::string( "Player 2" ), m_assignedPlayers[ 1 ].name );
			Assert::IsTrue( PlayerToken::PlayO == m_assignedPlayers[ 1 ].token );
		}

		TEST_METHOD( FirstPlayerToMoveIsX )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
		}

		TEST_METHOD( SecondPlayerToMoveIsO )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 2 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 1 ].state() );
		}

		TEST_METHOD( DetectsWinOnFirstRow )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 2 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 1 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 3 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 2 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 4 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 3 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 5 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 4 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 2 } ); // Wins

			Assert::AreEqual( true, waitForData( m_boards, 6 ) );
			Assert::IsTrue( GameState::VictoryX == m_boards[ 5 ].state() );
		}

		TEST_METHOD( DetectsWinOnSecondColumn )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 2 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 1 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 0, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 3 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 2 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 1, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 4 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 3 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 5 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 4 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 2, 1 } ); // Wins

			Assert::AreEqual( true, waitForData( m_boards, 6 ) );
			Assert::IsTrue( GameState::VictoryX == m_boards[ 5 ].state() );
		}

		TEST_METHOD( DetectsWinOnDownDiagonal )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 2 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 1 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 0, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 3 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 2 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 1, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 4 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 3 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 5 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 4 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 2, 2 } ); // Wins

			Assert::AreEqual( true, waitForData( m_boards, 6 ) );
			Assert::IsTrue( GameState::VictoryX == m_boards[ 5 ].state() );
		}

		TEST_METHOD( DetectsWinOnUpDiagonal )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 2, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 2 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 1 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 0, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 3 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 2 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 1, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 4 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 3 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 5 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 4 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 2 } ); // Wins

			Assert::AreEqual( true, waitForData( m_boards, 6 ) );
			Assert::IsTrue( GameState::VictoryX == m_boards[ 5 ].state() );
		}

		TEST_METHOD( DetectsDraw )
		{
			m_wRegisterPlayer->write( { "Player 1" } );
			m_wRegisterPlayer->write( { "Player 2" } );
			Assert::AreEqual( true, waitForData( m_assignedPlayers, 2 ) );

			Assert::AreEqual( true, waitForData( m_boards ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 0 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 2 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 1 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 0, 2 } );

			Assert::AreEqual( true, waitForData( m_boards, 3 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 2 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 0, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 4 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 3 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 5 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 4 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 1, 2 } );

			Assert::AreEqual( true, waitForData( m_boards, 6 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 5 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 1, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 7 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 6 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 2, 0 } );

			Assert::AreEqual( true, waitForData( m_boards, 8 ) );
			Assert::IsTrue( GameState::ToMoveO == m_boards[ 7 ].state() );
			m_wMove->write( Move { PlayerToken::PlayO, 2, 1 } );

			Assert::AreEqual( true, waitForData( m_boards, 9 ) );
			Assert::IsTrue( GameState::ToMoveX == m_boards[ 8 ].state() );
			m_wMove->write( Move { PlayerToken::PlayX, 2, 2 } );

			Assert::AreEqual( true, waitForData( m_boards, 10 ) );
			Assert::IsTrue( GameState::Draw == m_boards[ 9 ].state() );
		}
	};
}
