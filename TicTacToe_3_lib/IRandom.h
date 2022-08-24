#pragma once

class IRandom
{
public:
    ~IRandom() = default;

    virtual void seed() = 0;
    virtual int getNext() = 0;
};
