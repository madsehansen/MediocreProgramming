#pragma once

template < typename T >
class IDataWriter
{
public:
    ~IDataWriter() = default;

    virtual void write( const T& ) = 0;
};
