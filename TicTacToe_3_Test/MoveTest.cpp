#include "pch.h"
#include "CppUnitTest.h"

#include "../TicTacToe_3_lib/Model.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToe3Test
{
	TEST_CLASS( MoveTest )
	{

	public:

		TEST_METHOD( DefaultConstructedAllFieldsAreEmpty )
		{
			Move sut;

			Assert::AreEqual( PlayerToken::PlayO, sut.token() );
			Assert::AreEqual( 0, sut.row() );
			Assert::AreEqual( 0, sut.col() );
		}

		TEST_METHOD( GettingTheTokenGivesTheSameTokenAsPreviouslySet )
		{
			Move sut( PlayerToken::PlayX, 0, 0 );

			Assert::AreEqual( PlayerToken::PlayX, sut.token() );
		}

		TEST_METHOD( SettingWithTooLowRowThrowsException )
		{
			Assert::ExpectException< std::range_error >( [] { Move sut( PlayerToken::PlayO, -1, 0 ); } );
		}

		TEST_METHOD( SettingWithTooLowColThrowsException )
		{
			Assert::ExpectException< std::range_error >( [] { Move sut( PlayerToken::PlayO, 0, -1 ); } );
		}

		TEST_METHOD( SettingWithTooHighRowThrowsException )
		{
			Assert::ExpectException< std::range_error >( [] { Move sut( PlayerToken::PlayO, 3, 0 ); } );
		}

		TEST_METHOD( SettingWithTooHighColThrowsException )
		{
			Assert::ExpectException< std::range_error >( [] { Move sut( PlayerToken::PlayO, 0, 3 ); } );
		}
	};
}
