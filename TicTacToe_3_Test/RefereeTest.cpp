#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_3_lib/Referee.h"

#include "DataWriterMock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe3Test
{
	TEST_CLASS( RefereeTest )
	{
		DataWriterMock< AssignedPlayer > m_wAssignedPlayer;
		DataWriterMock< Board > m_wBoard;

		std::unique_ptr<Referee> m_sut;

	public:

		TEST_METHOD_INITIALIZE( setup )
		{
			m_sut = std::make_unique<Referee>( &m_wAssignedPlayer, &m_wBoard );
		}
		TEST_METHOD_CLEANUP( cleanup )
		{
			m_sut.reset();
			m_wAssignedPlayer.data.clear();
			m_wBoard.data.clear();
		}

		TEST_METHOD( RegisteringTwoPlayersAsXAndO )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );
			Assert::AreEqual( std::string( "Player 1" ), m_wAssignedPlayer.data[ 0 ].name );
			Assert::IsTrue( PlayerToken::PlayX == m_wAssignedPlayer.data[ 0 ].token );
			Assert::AreEqual( std::string( "Player 2" ), m_wAssignedPlayer.data[ 1 ].name );
			Assert::IsTrue( PlayerToken::PlayO == m_wAssignedPlayer.data[ 1 ].token );
		}

		TEST_METHOD( FirstPlayerToMoveIsX )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( SecondPlayerToMoveIsO )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( (size_t)2, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 1 ].state() );
		}

		TEST_METHOD( DetectsWinOnFirstRow )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( (size_t)2, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 1 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( (size_t)3, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 2 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 1 } );

			Assert::AreEqual( (size_t)4, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 3 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 1 } );

			Assert::AreEqual( (size_t)5, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 4 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 2 } ); // Wins

			Assert::AreEqual( (size_t)6, m_wBoard.data.size() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 5 ].state() );
		}

		TEST_METHOD( DetectsWinOnSecondColumn )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 1 } );

			Assert::AreEqual( (size_t)2, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 1 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 0, 0 } );

			Assert::AreEqual( (size_t)3, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 2 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 1, 1 } );

			Assert::AreEqual( (size_t)4, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 3 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( (size_t)5, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 4 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 1 } ); // Wins

			Assert::AreEqual( (size_t)6, m_wBoard.data.size() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 5 ].state() );
		}

		TEST_METHOD( DetectsWinOnDownDiagonal )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( (size_t)2, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 1 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 0, 1 } );

			Assert::AreEqual( (size_t)3, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 2 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 1, 1 } );

			Assert::AreEqual( (size_t)4, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 3 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( (size_t)5, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 4 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 2 } ); // Wins

			Assert::AreEqual( (size_t)6, m_wBoard.data.size() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 5 ].state() );
		}

		TEST_METHOD( DetectsWinOnUpDiagonal )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 0 } );

			Assert::AreEqual( (size_t)2, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 1 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 0, 1 } );

			Assert::AreEqual( (size_t)3, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 2 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 1, 1 } );

			Assert::AreEqual( (size_t)4, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 3 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( (size_t)5, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 4 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 2 } ); // Wins

			Assert::AreEqual( (size_t)6, m_wBoard.data.size() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 5 ].state() );
		}

		TEST_METHOD( DetectsDraw )
		{
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );

			Assert::AreEqual( (size_t)1, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 0 } );

			Assert::AreEqual( (size_t)2, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 1 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 0, 2 } );

			Assert::AreEqual( (size_t)3, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 2 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 1 } );

			Assert::AreEqual( (size_t)4, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 3 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 0 } );

			Assert::AreEqual( (size_t)5, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 4 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 1, 2 } );

			Assert::AreEqual( (size_t)6, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 5 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 1, 1 } );

			Assert::AreEqual( (size_t)7, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 6 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 0 } );

			Assert::AreEqual( (size_t)8, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 7 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayO, 2, 1 } );

			Assert::AreEqual( (size_t)9, m_wBoard.data.size() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 8 ].state() );
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 2 } );

			Assert::AreEqual( (size_t)10, m_wBoard.data.size() );
			Assert::IsTrue( GameState::Draw == m_wBoard.data[ 9 ].state() );
		}
	};
}
