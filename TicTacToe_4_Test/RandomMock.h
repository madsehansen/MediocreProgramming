#pragma once
#include "../TicTacToe_4_lib/IRandom.h"

#include <vector>

class RandomMock :
    public IRandom
{
public:
    explicit RandomMock( std::vector<int> a_data )
        : m_data( std::move( a_data ) )
    { }

    // Inherited via IRandom
    int getNext() override
    {
        if ( m_data.empty() )
        {
            return 0;
        }

        auto value { m_data.front() };
        m_data.erase( m_data.begin() );

        return value;
    }

    // Inherited via IRandom
    void seed() override
    {
        // Nothing
    }

    std::vector<int> m_data;
};
