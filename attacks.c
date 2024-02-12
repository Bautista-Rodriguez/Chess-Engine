
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "attacks.h"
#include "bitboard.h"

U64 maskPawnAttacks(int side, int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);
    if(!side)
    {
        if((bitboard << 7) & notFileH)
            attacks |= (bitboard << 7);
        if((bitboard << 9) & notFileA)
            attacks |= (bitboard << 9);
    }
    else
    {
        if((bitboard >> 7) & notFileA)
            attacks |= (bitboard >> 7);
        if((bitboard >> 9) & notFileH)
            attacks |= (bitboard >> 9);
    }
    return attacks;
}

U64 maskKnightAttacks(int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    if((bitboard << 6) & notFileGH)
        attacks |= (bitboard << 6);
    if((bitboard << 15) & notFileH)
        attacks |= (bitboard << 15);
    if((bitboard << 10) & notFileAB)
        attacks |= (bitboard << 10);
    if((bitboard << 17) & notFileA)
        attacks |= (bitboard << 17);

    if((bitboard >> 6) & notFileAB)
        attacks |= (bitboard >> 6);
    if((bitboard >> 15) & notFileA)
        attacks |= (bitboard >> 15);
    if((bitboard >> 10) & notFileGH)
        attacks |= (bitboard >> 10);
    if((bitboard >> 17) & notFileH)
        attacks |= (bitboard >> 17);
    return attacks;
}

U64 maskKingAttacks(int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    if((bitboard << 7) & notFileH)
        attacks |= (bitboard << 7);
    if((bitboard << 9) & notFileA)
        attacks |= (bitboard << 9);
    if((bitboard >> 7) & notFileA)
        attacks |= (bitboard >> 7);
    if((bitboard >> 9) & notFileH)
        attacks |= (bitboard >> 9);
    if((bitboard << 1) & notFileA)
        attacks |= (bitboard << 1);
    if((bitboard >> 1) & notFileH)
        attacks |= (bitboard >> 1);
    attacks |= (bitboard << 8);
    attacks |= (bitboard >> 8);

    return attacks;
}

U64 maskRookRay(int square)
{
    U64 ray = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

    for(int i = 0;i < 7 - rank; i++)
    {
        ray |= (bitboard << (8 * i));
    }
    for(int i = 0;i < file; i++)
    {
        ray |= (bitboard >> (1 * i));
    }
    for(int i = 0;i < rank; i++)
    {
        ray |= (bitboard >> (8 * i));
    }
    for(int i = 0;i < 7 - file; i++)
    {
        ray |= (bitboard << (1 * i));
    }

    ray &= ~bitboard;

    return ray;
}

U64 maskBishopRay(int square)
{
    U64 ray = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;

    for(int i = 0;i < 8 - file; i++)
    {
        ray |= (bitboard << (9 * i));
    }
    for(int i = 0;i < file + 1; i++)
    {
        ray |= (bitboard << (7 * i));
    }
    for(int i = 0;i < file + 1; i++)
    {
        ray |= (bitboard >> (9 * i));
    }
    for(int i = 0;i < 8 - file; i++)
    {
        ray |= (bitboard >> (7 * i));
    }
    ray &= ~fileA;
    ray &= ~fileH;
    ray &= ~rank1;
    ray &= ~rank8;
    ray &= ~bitboard;

    return ray;
}



U64 maskQueenRay(int square)
{
    U64 ray = 0ULL;

    ray |= maskBishopRay(square);
    ray |= maskRookRay(square);

    return ray;
}

U64 maskRookAttacks(int square, U64 position)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

    for(int i = 0;i < 8 - rank; i++)
    {
        attacks |= (bitboard << (8 * i));
        if((bitboard << (8 * i)) & position) break;
    }
    for(int i = 0;i < file + 1; i++)
    {
        attacks |= (bitboard >> (1 * i));
        if((bitboard >> (1 * i)) & position) break;
    }
    for(int i = 0;i < rank + 1; i++)
    {
        attacks |= (bitboard >> (8 * i));
        if((bitboard >> (8 * i)) & position) break;
    }
    for(int i = 0;i < 8 - file; i++)
    {
        attacks |= (bitboard << (1 * i));
        if((bitboard << (1 * i)) & position) break;
    }

    attacks &= ~bitboard;

    return attacks;
}

U64 maskBishopAttacks(int square, U64 position)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;

    for(int i = 0;i < 8 - file; i++)
    {
        attacks |= (bitboard << (9 * i));
        if((bitboard << (9 * i)) & position) break;
    }
    for(int i = 0;i < file + 1; i++)
    {
        attacks |= (bitboard << (7 * i));
        if((bitboard << (7 * i)) & position) break;
    }
    for(int i = 0;i < file + 1; i++)
    {
        attacks |= (bitboard >> (9 * i));
        if((bitboard >> (9 * i)) & position) break;
    }
    for(int i = 0;i < 8 - file; i++)
    {
        attacks |= (bitboard >> (7 * i));
        if((bitboard >> (7 * i)) & position) break;
    }
    attacks &= ~bitboard;

    return attacks;
}

U64 maskQueenAttacks(int square, U64 position)
{
    U64 attacks = 0ULL;
    attacks |= maskBishopAttacks(square, position);
    attacks |= maskRookAttacks(square, position);
    return attacks;
}

U64 occupancyRookAttacks(int square, int number)
{
    U64 rookRay = maskRookRay(square);
    int numberOfBits = popCount(rookRay);
    U64 occupancy = 0ULL;
    for(int i = 0;i < numberOfBits;i++)
    {
        int index = bitScan(rookRay);
        occupancy |= ((number >> i) & 1ULL) << index;
        rookRay &= rookRay - 1;
    }
    return occupancy;
}

void initRookOccupancyAttacks()
{
    for(int i = 0;i < 64;i++)
    {
        for(int j = 0;j < 4096;j++)
        {
            U64 occup = occupancyRookAttacks(i, j);
            U64 magic_index = (occup * rookMagicNumbers[i]) >> rookShifts[i];
            rookOccupancy[i][magic_index] = maskRookAttacks(i, occup);
        }
    }
    return;
}
void initBishopOccupancyAttacks()
{
    for(int i = 0;i < 64;i++)
    {
        for(int j = 0;j < 512;j++)
        {
            U64 occup = occupancyBishopAttacks(i, j);
            U64 magic_index = (occup * bishopMagicNumbers[i]) >> bishopShifts[i];
            bishopOccupancy[i][magic_index] = maskBishopAttacks(i, occup);
        }
    }
    return;
}

U64 occupancyBishopAttacks(int square, int number)
{
    U64 bishopRay = maskBishopRay(square);
    int numberOfBits = popCount(bishopRay);
    U64 occupancy = 0ULL;
    for(int i = 0;i < numberOfBits;i++)
    {
        int index = bitScan(bishopRay);
        occupancy |= ((number >> i) & 1ULL) << index;
        bishopRay &= bishopRay - 1;
    }
    return occupancy;
}

U64 getRookAttacks(int square, U64 occupancy)
{
    U64 attacks = 0ULL;
    occupancy &= rookRay[square];
    occupancy *= rookMagicNumbers[square];
    occupancy >>= rookShifts[square];
    attacks = rookOccupancy[square][occupancy];
    return attacks;
}

U64 getBishopAttacks(int square, U64 occupancy)
{
    U64 attacks = 0ULL;
    occupancy &= bishopRay[square];
    occupancy *= bishopMagicNumbers[square];
    occupancy >>= bishopShifts[square];
    attacks = bishopOccupancy[square][occupancy];
    return attacks;
}

U64 getQueenAttacks(int square, U64 occupancy)
{
    U64 attacks = 0ULL;
    attacks |= getBishopAttacks(square, occupancy);
    attacks |= getRookAttacks(square, occupancy);
    return attacks;
}

void initMaskKingAttacks()
{
    for(int square=0;square<64;square++)
    {
        kingAttacks[square]=maskKingAttacks(square);
    }
}

void initMaskKnightAttacks()
{
    for(int square=0;square<64;square++)
    {
        knightAttacks[square]=maskKnightAttacks(square);
    }
}


void initMaskPawnAttacks()
{
    for(int square=0;square<64;square++)
    {
        pawnAttacks[0][square]=maskPawnAttacks(0,square);
        pawnAttacks[1][square]=maskPawnAttacks(1,square);
    }
}

void initMaskRookRay()
{
    for(int square=0;square<64;square++)
    {
        rookRay[square]=maskRookRay(square);
    }
}

void initMaskBishopRay()
{
    for(int square=0;square<64;square++)
    {
        bishopRay[square]=maskBishopRay(square);
    }
}

void initMaskQueenRay()
{
    for(int square=0;square<64;square++)
    {
        queenRay[square]=maskQueenRay(square);
    }
}

void initMasks()
{
    initMaskPawnAttacks();
    initMaskKnightAttacks();
    initMaskKingAttacks();
    initMaskRookRay();
    initMaskBishopRay();
    initMaskQueenRay();
    initRookOccupancyAttacks();
    initBishopOccupancyAttacks();
    return;
}
