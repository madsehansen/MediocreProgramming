#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_1_lib/FirstFreePlayer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe1Test
{
	TEST_CLASS( FirstFreePlayerTest )
	{
		std::unique_ptr<IntraCom::IntraCom> m_intraCom;
		IntraCom::DataReader< RegisterPlayer >* m_rRegisterPlayer { nullptr };
		IntraCom::DataReader< Move >* m_rMove { nullptr };

		IntraCom::DataWriter< AssignedPlayer >* m_wAssignedPlayer { nullptr };
		IntraCom::DataWriter< Board >* m_wBoard { nullptr };

		std::unique_ptr<FirstFreePlayer> m_sut;

		std::vector< RegisterPlayer > m_registeredPlayers;
		void readRegisterPlayer( const std::vector< RegisterPlayer >& a_data )
		{
			m_registeredPlayers.insert(
				m_registeredPlayers.end(),
				a_data.begin(),
				a_data.end()
			);
		}

		std::vector< Move > m_moves;
		void readMove( const std::vector< Move >& a_data )
		{
			m_moves.insert(
				m_moves.end(),
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

			m_rRegisterPlayer = m_intraCom->createReader< RegisterPlayer >( [this]( IntraCom::Reader* ) { this->readRegisterPlayer( this->m_rRegisterPlayer->read() ); } );
			m_rMove = m_intraCom->createReader< Move >( [this]( IntraCom::Reader* ) { this->readMove( this->m_rMove->read() ); } );

			m_wAssignedPlayer = m_intraCom->createWriter< AssignedPlayer >();
			m_wBoard = m_intraCom->createWriter< Board >();

			m_sut = std::make_unique<FirstFreePlayer>( *m_intraCom, "TestPlayerFF" );

			m_intraCom->start();
		}
		TEST_METHOD_CLEANUP( cleanup )
		{
			m_intraCom->stop();

			m_sut.release();
			m_rRegisterPlayer = nullptr;
			m_registeredPlayers.clear();
			m_rMove = nullptr;
			m_moves.clear();

			m_wAssignedPlayer = nullptr;
			m_wBoard = nullptr;

			m_intraCom.release();
		}

		TEST_METHOD( NewObjectTriesToRegister )
		{
			Assert::AreEqual( true, waitForData( m_registeredPlayers ) );
			Assert::AreEqual( std::string( "TestPlayerFF" ), m_registeredPlayers[ 0 ].name );
		}
		TEST_METHOD( MakesMoveWhenBoardSayThePlayerShallPlay )
		{
			Assert::AreEqual( true, waitForData( m_registeredPlayers ) );

			AssignedPlayer assignedPlayer = { m_registeredPlayers[ 0 ].name, PlayerToken::PlayO };
			m_wAssignedPlayer->write( assignedPlayer );

			Board board = { GameState::ToMoveO, 
				{
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty },
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty },
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty } },
				};
			m_wBoard->write( board );

			Assert::AreEqual( true, waitForData( m_moves ) );
			Assert::AreEqual( 0, m_moves[ 0 ].col );
			Assert::AreEqual( 0, m_moves[ 0 ].row );
			Assert::IsTrue( PlayerToken::PlayO == m_moves[ 0 ].token );
		}

		TEST_METHOD( MakesNoMoveWhenBoardSayThePlayerShallNotPlay )
		{
			Assert::AreEqual( true, waitForData( m_registeredPlayers ) );

			AssignedPlayer assignedPlayer = { m_registeredPlayers[ 0 ].name, PlayerToken::PlayO };
			m_wAssignedPlayer->write( assignedPlayer );

			Board board = { GameState::ToMoveX,
				{
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty },
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty },
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty } },
			};
			m_wBoard->write( board );

			Assert::AreEqual( false, waitForData( m_moves ) );
		}

		TEST_METHOD( MakesMoveWhenBoardSayThePlayerShallPlayAndFirstSquareIsUsed )
		{
			Assert::AreEqual( true, waitForData( m_registeredPlayers ) );

			AssignedPlayer assignedPlayer = { m_registeredPlayers[ 0 ].name, PlayerToken::PlayO };
			m_wAssignedPlayer->write( assignedPlayer );

			Board board = { GameState::ToMoveO,
				{
					{ SquareState::HasX,  SquareState::Empty, SquareState::Empty },
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty },
					{ SquareState::Empty, SquareState::Empty, SquareState::Empty } },
			};
			m_wBoard->write( board );

			Assert::AreEqual( true, waitForData( m_moves ) );
			Assert::AreEqual( 1, m_moves[ 0 ].col );
			Assert::AreEqual( 0, m_moves[ 0 ].row );
			Assert::IsTrue( PlayerToken::PlayO == m_moves[ 0 ].token );
		}

		TEST_METHOD( MakesNoMoveWhenBoardSayThePlayerShallPlayButNoSquareFree )
		{
			Assert::AreEqual( true, waitForData( m_registeredPlayers ) );

			AssignedPlayer assignedPlayer = { m_registeredPlayers[ 0 ].name, PlayerToken::PlayO };
			m_wAssignedPlayer->write( assignedPlayer );

			Board board = { GameState::ToMoveO,
				{
					{ SquareState::HasO, SquareState::HasX, SquareState::HasX },
					{ SquareState::HasO, SquareState::HasX, SquareState::HasO },
					{ SquareState::HasX, SquareState::HasO, SquareState::HasX } },
			};
			m_wBoard->write( board );

			Assert::AreEqual( false, waitForData( m_moves ) );
		}

	};
}
