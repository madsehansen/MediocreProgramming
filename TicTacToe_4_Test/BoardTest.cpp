#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_4_lib/Model.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe4Test
{
	TEST_CLASS( BoardTest )
	{

	public:

		TEST_METHOD( DefaultConstructedAllFieldsAreEmpty )
		{
			Board sut;

			for ( int row { 0 }; row < 3; ++row )
			{
				for ( int col { 0 }; col < 3; ++col )
				{
					Assert::AreEqual( SquareState::Empty, sut.square( row, col ) );
				}
			}
		}

		TEST_METHOD( GettingTheStateGivesTheSameStateAsPreviouslySet )
		{
			Board sut;

			sut.setState( GameState::VictoryO );
			Assert::AreEqual( GameState::VictoryO, sut.state() );

			sut.setState( GameState::VictoryX );
			Assert::AreEqual( GameState::VictoryX, sut.state() );
		}

		TEST_METHOD( GettingASquareGivesTheValuePreviouslySet )
		{
			Board sut;

			sut.setSquare( 0, 0, SquareState::HasO );
			Assert::AreEqual( SquareState::HasO, sut.square( 0, 0 ) );
		}

		TEST_METHOD( SettingWithTooLowRowThrowsException )
		{
			Board sut;

			Assert::ExpectException< std::range_error >( [&] { sut.setSquare( -1, 0, SquareState::HasO ); } );
		}

		TEST_METHOD( SettingWithTooLowColThrowsException )
		{
			Board sut;

			Assert::ExpectException< std::range_error >( [&] { sut.setSquare( 0, -1, SquareState::HasO ); } );
		}

		TEST_METHOD( SettingWithTooHighRowThrowsException )
		{
			Board sut;

			Assert::ExpectException< std::range_error >( [&] { sut.setSquare( 3, 0, SquareState::HasO ); } );
		}

		TEST_METHOD( SettingWithTooHighColThrowsException )
		{
			Board sut;

			Assert::ExpectException< std::range_error >( [&] { sut.setSquare( 0, 3, SquareState::HasO ); } );
		}
	};
}
