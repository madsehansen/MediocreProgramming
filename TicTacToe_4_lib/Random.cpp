#include "pch.h"
#include "Random.h"

#include <cstdlib>
#include <time.h>

int Random::getNext()
{
    return std::rand();
}

void Random::seed()
{
    time_t t { time( nullptr ) };
    std::srand( static_cast<int>( t ) );
}
