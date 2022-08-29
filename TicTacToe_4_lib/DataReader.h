#pragma once

#include "../IntraCom/IntraCom.h"

template < typename OBJ, typename T >
class DataReader
{
public:
    explicit DataReader(
        IntraCom::IntraCom& a_intraCom,
        OBJ* a_obj,
        void (OBJ::*a_callback)( const T& ) )
        : m_reader { a_intraCom.createReader< T >( [this]( IntraCom::Reader* a_reader ) { read( a_reader ); } ) }
        , m_callback { [=]( const T& a_data ) { (a_obj->*a_callback)( a_data ); } }
    {
    }

    // No default, no copying, destruct default
    DataReader() = delete;
    DataReader( const DataReader& ) = delete;
    DataReader( DataReader&& ) = delete;
    DataReader& operator=( const DataReader& ) = delete;
    DataReader& operator=( DataReader&& ) = delete;
    ~DataReader() = default;

private:
    void read( IntraCom::Reader* a_reader )
    {
        if ( a_reader == m_reader )
        {
            for ( T& sample : m_reader->read() )
            {
                m_callback( sample );
            }
        }
    }

    IntraCom::DataReader< T >* m_reader { nullptr };
    std::function< void( const T& ) > m_callback;
};
