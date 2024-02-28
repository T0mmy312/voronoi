#include <iostream>
#include "perlin.h"

int main()
{
    for (int i = 0; i < 100; i++)
    {
        std::cout << nthDigitOfPi(i) << ", ";
    }
    std::cout << std::endl;
}