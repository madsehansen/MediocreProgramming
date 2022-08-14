#pragma once

#include "../TicTacToe_4_lib/IDataWriter.h"

#include <vector>

template < typename T >
class DataWriterMock : public IDataWriter< T >
{
public:
    void write( const T& a_data ) override
    {
        data.push_back( a_data );
    }

    // This class is for tests only, no need for proper encapsulation
    std::vector<T> data;
};
