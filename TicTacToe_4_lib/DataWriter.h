#pragma once

#include "IDataWriter.h"

#include "../IntraCom/IntraCom.h"

template < typename T >
class DataWriter : public IDataWriter< T >
{
public:
    explicit DataWriter(IntraCom::IntraCom& a_intraCom)
        : m_writer { a_intraCom.createWriter<T>() }
    { }

    void write( const T& a_data ) override
    {
        m_writer->write( a_data );
    }

private:
    IntraCom::DataWriter<T>* m_writer { nullptr };
};
