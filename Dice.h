#pragma once
#include<random>

struct Dice
{
    std::mt19937 mersenneTwister;
    std::random_device seed;
    std::uniform_int_distribution<int> distribution1d6;
    std::uniform_int_distribution<int> distribution1d8;
    std::uniform_int_distribution<int> distribution1d10;
    std::uniform_int_distribution<int> distribution1d12;
    std::uniform_int_distribution<int> distribution1d20;
    Dice():mersenneTwister(seed()), 
        distribution1d6(1, 6),
        distribution1d8(1, 8),
        distribution1d10(1, 10),
        distribution1d12(1, 12),
        distribution1d20(1, 20)
    {
        
    }

    // Nd6 throw
    int D6(int n)
    {
        int result = 0;
        for(int i=0;i<n;i++)
            result += distribution1d6(mersenneTwister);
        return result;
    }

    // Nd8 throw
    int D8(int n)
    {
        int result = 0;
        for (int i = 0; i < n; i++)
            result += distribution1d8(mersenneTwister);
        return result;
    }

    // Nd10 throw
    int D10(int n)
    {
        int result = 0;
        for (int i = 0; i < n; i++)
            result += distribution1d10(mersenneTwister);
        return result;
    }

    // Nd12 throw
    int D12(int n)
    {
        int result = 0;
        for (int i = 0; i < n; i++)
            result += distribution1d12(mersenneTwister);
        return result;
    }

    // Nd20 throw
    int D20(int n)
    {
        int result = 0;
        for (int i = 0; i < n; i++)
            result += distribution1d20(mersenneTwister);
        return result;
    }
};
