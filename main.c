#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "constsAndEnums.h"
#include "boardRepresentation.h"
#include "attacks.h"
#include "bitboard.h"
#include "moveGen.h"
#include "perft.h"
#include "makeUnmake.h"

int main()
{
    initMasks();
    //decodeMove(0b101110000000000000001000);
    struct BoardState board;
    struct BoardState *boardPtr=&board;
    parseFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBPPP3/q4N2/Pp4PP/R2Q1RK1 b kq d3 0 1",boardPtr);
    int moveList[256];
    perftTest();
    //printf("%i",(0b1000 & (1ULL << 3)));
    return 0;
}
