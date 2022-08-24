#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_3_lib/RandomPlayer.h"

#include "DataWriterMock.h"
#include "RandomMock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe3Test
{
	TEST_CLASS( RandomPlayerTest )
	{
		DataWriterMock<RegisterPlayer> m_wRegisterPlayer;
		DataWriterMock<Move> m_wMove;
		RandomMock m_random { { 1, 2, 3, 4, 5, 6, 7, 8, 9 } };
		std::unique_ptr<RandomPlayer> m_sut;

	public:

		TEST_METHOD_INITIALIZE( setup )
		{
			m_sut = std::make_unique<RandomPlayer>(
				&m_wRegisterPlayer,
				&m_wMove,
				&m_random,
				"TestPlayerRandom" );
		}
		TEST_METHOD_CLEANUP( cleanup )
		{
			m_sut.reset();
			m_wRegisterPlayer.data.clear();
			m_wMove.data.clear();
		}

		TEST_METHOD( NewObjectTriesToRegister )
		{
			Assert::AreEqual( std::string( "TestPlayerRandom" ), m_wRegisterPlayer.data[ 0 ].name );
		}
		TEST_METHOD( MakesMoveWhenBoardSayThePlayerShallPlay )
		{
			AssignedPlayer assignedPlayer = { m_wRegisterPlayer.data[ 0 ].name, PlayerToken::PlayO };
			m_sut->handleAssignedPlayer( assignedPlayer );

			Board board( GameState::ToMoveO,
					SquareState::Empty, SquareState::Empty, SquareState::Empty,
					SquareState::Empty, SquareState::Empty, SquareState::Empty,
					SquareState::Empty, SquareState::Empty, SquareState::Empty );
			m_sut->handleBoard( board );

			// Could be anywhere...
			Assert::IsTrue( PlayerToken::PlayO == m_wMove.data[ 0 ].token() );
		}

		TEST_METHOD( MakesNoMoveWhenBoardSayThePlayerShallNotPlay )
		{
			AssignedPlayer assignedPlayer = { m_wRegisterPlayer.data[ 0 ].name, PlayerToken::PlayO };
			m_sut->handleAssignedPlayer( assignedPlayer );

			Board board( GameState::ToMoveX,
					SquareState::Empty, SquareState::Empty, SquareState::Empty,
					SquareState::Empty, SquareState::Empty, SquareState::Empty,
					SquareState::Empty, SquareState::Empty, SquareState::Empty );
			m_sut->handleBoard( board );

			Assert::AreEqual( true, m_wMove.data.empty() );
		}

		TEST_METHOD( MakesMoveWhenBoardSayThePlayerShallPlayAndFirstSquareIsUsed )
		{
			AssignedPlayer assignedPlayer = { m_wRegisterPlayer.data[ 0 ].name, PlayerToken::PlayO };
			m_sut->handleAssignedPlayer( assignedPlayer );

			Board board( GameState::ToMoveO,
					SquareState::HasX,  SquareState::Empty, SquareState::Empty,
					SquareState::Empty, SquareState::Empty, SquareState::Empty,
					SquareState::Empty, SquareState::Empty, SquareState::Empty );
			m_sut->handleBoard( board );

			Assert::AreEqual( false, m_wMove.data.empty() );
			Assert::IsTrue( 0 != m_wMove.data[ 0 ].col() || 0 != m_wMove.data[ 0 ].row() );
			Assert::IsTrue( PlayerToken::PlayO == m_wMove.data[ 0 ].token() );
		}

		TEST_METHOD( MakesNoMoveWhenBoardSayThePlayerShallPlayButNoSquareFree )
		{
			AssignedPlayer assignedPlayer = { m_wRegisterPlayer.data[ 0 ].name, PlayerToken::PlayO };
			m_sut->handleAssignedPlayer( assignedPlayer );

			Board board( GameState::ToMoveO,
					SquareState::HasO, SquareState::HasX, SquareState::HasX,
					SquareState::HasO, SquareState::HasX, SquareState::HasO,
					SquareState::HasX, SquareState::HasO, SquareState::HasX );
			m_sut->handleBoard( board );

			Assert::AreEqual( true, m_wMove.data.empty() );
		}

	};
}
