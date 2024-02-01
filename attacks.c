#include "boardRepresentation.h"
#include "attacks.h"

const U64 notAFile = 0xFEFEFEFEFEFEFEFEULL;
const U64 notHFile = 0x7F7F7F7F7F7F7F7FULL;
const U64 notHGFile = 0x3F3F3F3F3F3F3F3FULL;
const U64 notABFile = 0xFCFCFCFCFCFCFCFCULL;

const U64 borders = 0xFF818181818181FFULL;
const U64 notBorders = ~borders;

const U64 AFile = 0x0101010101010101ULL;
const U64 BFile = AFile << 1;
const U64 CFile = AFile << 2;
const U64 DFile = AFile << 3;
const U64 EFile = AFile << 4;
const U64 FFile = AFile << 5;
const U64 GFile = AFile << 6;
const U64 HFile = AFile << 7;

const U64 Rank1 = 0xFFULL;
const U64 Rank2 = Rank1 << 8;
const U64 Rank3 = Rank1 << 8*2;
const U64 Rank4 = Rank1 << 8*3;
const U64 Rank5 = Rank1 << 8*4;
const U64 Rank6 = Rank1 << 8*5;
const U64 Rank7 = Rank1 << 8*6;
const U64 Rank8 = Rank1 << 8*7;

const U64 A1Diagonal = 0x8040201008040201ULL;
const U64 H1Diagonal = 0x0102040810204080ULL;

U64 maskPawnAttacks(int side, int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);
    if(!side)
    {
        if((bitboard << 7) & notHFile)
            attacks |= (bitboard << 7);
        if((bitboard << 9) & notAFile)
            attacks |= (bitboard << 9);
    }
    else
    {
        if((bitboard >> 7) & notAFile)
            attacks |= (bitboard >> 7);
        if((bitboard >> 9) & notHFile)
            attacks |= (bitboard >> 9);
    }
    return attacks;
}

void initMaskPawnAttacks()
{
    for(int square=0;square<64;square++)
    {
        pawnAttacks[0][square]=maskPawnAttacks(0,square);
        pawnAttacks[1][square]=maskPawnAttacks(1,square);
    }
}

U64 maskKnightAttacks(int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    if((bitboard << 6) & notHGFile)
        attacks |= (bitboard << 6);
    if((bitboard << 15) & notHFile)
        attacks |= (bitboard << 15);
    if((bitboard << 10) & notABFile)
        attacks |= (bitboard << 10);
    if((bitboard << 17) & notAFile)
        attacks |= (bitboard << 17);

    if((bitboard >> 6) & notABFile)
        attacks |= (bitboard >> 6);
    if((bitboard >> 15) & notAFile)
        attacks |= (bitboard >> 15);
    if((bitboard >> 10) & notHGFile)
        attacks |= (bitboard >> 10);
    if((bitboard >> 17) & notHFile)
        attacks |= (bitboard >> 17);
    return attacks;
}

void initMaskKnightAttacks()
{
    for(int square=0;square<64;square++)
    {
        knightAttacks[square]=maskKnightAttacks(square);
    }
}

U64 maskKingAttacks(int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    if((bitboard << 7) & notHFile)
        attacks |= (bitboard << 7);
    if((bitboard << 9) & notAFile)
        attacks |= (bitboard << 9);
    if((bitboard >> 7) & notAFile)
        attacks |= (bitboard >> 7);
    if((bitboard >> 9) & notHFile)
        attacks |= (bitboard >> 9);
    if((bitboard << 1) & notAFile)
        attacks |= (bitboard << 1);
    if((bitboard >> 1) & notHFile)
        attacks |= (bitboard >> 1);
    attacks |= (bitboard << 8);
    attacks |= (bitboard >> 8);

    return attacks;
}

void initMaskKingAttacks()
{
    for(int square=0;square<64;square++)
    {
        kingAttacks[square]=maskKingAttacks(square);
    }
}

U64 maskRookRay(int square)
{
    U64 ray = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

    for(int i = 0;i < 8 - rank; i++)
    {
        ray |= (bitboard << (8 * i));
    }
    for(int i = 0;i < file + 1; i++)
    {
        ray |= (bitboard >> (1 * i));
    }
    for(int i = 0;i < rank + 1; i++)
    {
        ray |= (bitboard >> (8 * i));
    }
    for(int i = 0;i < 8 - file; i++)
    {
        ray |= (bitboard << (1 * i));
    }

    ray &= ~bitboard;

    return ray;
}

void initMaskRookRay()
{
    for(int square=0;square<64;square++)
    {
        rookRay[square]=maskRookRay(square);
    }
}

U64 maskBishopRay(int square)
{
    U64 ray = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

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
    ray &= ~bitboard;

    return ray;
}

void initMaskBishopRay()
{
    for(int square=0;square<64;square++)
    {
        bishopRay[square]=maskBishopRay(square);
    }
}

U64 maskQueenRay(int square)
{
    U64 ray = 0ULL;

    ray |= maskBishopRay(square);
    ray |= maskRookRay(square);

    return ray;
}

void initMaskQueenRay()
{
    for(int square=0;square<64;square++)
    {
        queenRay[square]=maskQueenRay(square);
    }
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
    int rank = square/8;
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
