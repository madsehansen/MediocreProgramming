#pragma once
#include "IRandom.h"
class Random :
    public IRandom
{
public:

    // Inherited via IRandom
    int getNext() override;

    // Inherited via IRandom
    void seed() override;
};

