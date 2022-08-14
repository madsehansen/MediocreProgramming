#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_4_lib/Referee.h"

#include "DataWriterMock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe4Test
{
	TEST_CLASS( RefereeTest )
	{
		DataWriterMock< AssignedPlayer > m_wAssignedPlayer;
		DataWriterMock< Board > m_wBoard;

		GameData m_game;
		std::unique_ptr<Referee> m_sut;

	public:

		TEST_METHOD_INITIALIZE( setup )
		{
			m_game = { Board{ }, AssignedPlayer{ "", PlayerToken::PlayX }, AssignedPlayer{ "", PlayerToken::PlayO } };
			m_sut = std::make_unique<Referee>( &m_wAssignedPlayer, &m_wBoard, &m_game );
		}
		TEST_METHOD_CLEANUP( cleanup )
		{
			m_sut.reset();
			m_wAssignedPlayer.data.clear();
			m_wBoard.data.clear();
		}

		TEST_METHOD( RegisteringTwoPlayersAsXAndO )
		{
			// Arrange

			// Act
			m_sut->handleRegisterPlayer( { "Player 1" } );
			m_sut->handleRegisterPlayer( { "Player 2" } );

			// Assert
			Assert::AreEqual( (size_t)2, m_wAssignedPlayer.data.size() );
			Assert::AreEqual( std::string( "Player 1" ), m_wAssignedPlayer.data[ 0 ].name );
			Assert::IsTrue( PlayerToken::PlayX == m_wAssignedPlayer.data[ 0 ].token );
			Assert::AreEqual( std::string( "Player 2" ), m_wAssignedPlayer.data[ 1 ].name );
			Assert::IsTrue( PlayerToken::PlayO == m_wAssignedPlayer.data[ 1 ].token );
		}

		TEST_METHOD( FirstPlayerToMoveIsX )
		{
			// Arrange
			m_game.playerX.name = "Player 1"; // Make first player exist

			// Act
			m_sut->handleRegisterPlayer( { "Player 2" } );

			// Assert
			Assert::AreEqual( false, m_wAssignedPlayer.data.empty() );
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::ToMoveX == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( SecondPlayerToMoveIsO )
		{
			// Arrange
			m_game.board.setState( GameState::ToMoveX );

			// Act
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 0 } );

			// Assert
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::ToMoveO == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( DetectsWinOnFirstRow )
		{
			// Arrange
			m_game.board = Board{ GameState::ToMoveX,
				SquareState::HasX,  SquareState::HasX,  SquareState::Empty,
				SquareState::HasO,  SquareState::HasO,  SquareState::Empty,
				SquareState::Empty, SquareState::Empty, SquareState::Empty,
				};

			// Act
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 2 } ); // Wins

			// Assert
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( DetectsWinOnSecondColumn )
		{
			// Arrange
			m_game.board = Board { GameState::ToMoveX,
				SquareState::HasO,  SquareState::HasX,  SquareState::Empty,
				SquareState::HasO,  SquareState::HasX,  SquareState::Empty,
				SquareState::Empty, SquareState::Empty, SquareState::Empty,
			};

			// Act
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 1 } ); // Wins

			// Assert
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( DetectsWinOnDownDiagonal )
		{
			// Arrange
			m_game.board = Board { GameState::ToMoveX,
				SquareState::HasX,  SquareState::HasO,  SquareState::Empty,
				SquareState::HasO,  SquareState::HasX,  SquareState::Empty,
				SquareState::Empty, SquareState::Empty, SquareState::Empty,
			};

			// Act
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 2 } ); // Wins

			// Assert
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( DetectsWinOnUpDiagonal )
		{
			// Arrange
			m_game.board = Board { GameState::ToMoveX,
				SquareState::Empty, SquareState::HasO,  SquareState::Empty,
				SquareState::HasO,  SquareState::HasX,  SquareState::Empty,
				SquareState::HasX,  SquareState::Empty, SquareState::Empty,
			};

			// Act
			m_sut->handleMove( Move { PlayerToken::PlayX, 0, 2 } ); // Wins

			// Assert
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::VictoryX == m_wBoard.data[ 0 ].state() );
		}

		TEST_METHOD( DetectsDraw )
		{
			// Arrange
			m_game.board = Board { GameState::ToMoveX,
				SquareState::HasX, SquareState::HasO, SquareState::HasO,
				SquareState::HasX, SquareState::HasO, SquareState::HasX,
				SquareState::HasO, SquareState::HasX, SquareState::Empty,
			};

			// Act
			m_sut->handleMove( Move { PlayerToken::PlayX, 2, 2 } );

			// Assert
			Assert::AreEqual( false, m_wBoard.data.empty() );
			Assert::IsTrue( GameState::Draw == m_wBoard.data[ 0 ].state() );
		}
	};
}
