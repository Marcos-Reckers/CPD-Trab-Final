#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace hash
{
    class Hash
    {
    private:
        size_t size = 0;

    public:
        Hash();
        ~Hash();
        
        size_t Size();
    };
    
} // namespace hash
