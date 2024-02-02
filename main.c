#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "boardRepresentation.h"
#include "attacks.c"
#include "bitboard.c"

enum square{
    a1,b1,c1,d1,e1,f1,g1,h1,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a8,b8,c8,d8,e8,f8,g8,h8
};

enum color{
    white,black
};

int main()
{
    initMaskPawnAttacks();
    initMaskKnightAttacks();
    initMaskKingAttacks();
    initMaskRookRay();
    initMaskQueenRay();
    initMaskBishopRay();

    U64 pieces = 0LL;
    pieces |= 1ULL << f5;
    pieces |= 1ULL << c6;
    pieces |= 1ULL << g2;
    pieces |= 1ULL << b1;
    pieces |= 1ULL << e3;
    pieces |= 1ULL << c4;
    pieces |= 1ULL << e7;
    pieces |= 1ULL << h4;
    for(int j = 0;j < 64;j++)
    {
        for(int i=0;i<4096;i++)
            occupancyRook[j][i]=occupancyRookAttacks(j,i);
    }
    for(int j = 0;j < 64;j++)
    {
        for(int i=0;i<512;i++)
            occupancyBishop[j][i]=occupancyBishopAttacks(j,i);
    }
    U64 bishopRay = maskBishopRay(e4);
    printBitboard(bishopRay);
    printf("\n");
    for(int j = 63;j < 64;j++)
    {
        for(int i=511;i<512;i++){
            printBitboard(occupancyBishop[e4][i]);
            printf("\n");
        }
    }
    //U64 maskRook=maskRookRay(a1);
    //printBitboard(maskRook);
    return 0;
}
