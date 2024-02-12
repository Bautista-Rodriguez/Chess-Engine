#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "constsAndEnums.h"
#include "boardRepresentation.h"
#include "attacks.h"
#include "bitboard.h"
#include "moveGen.h"

int main()
{
    initMasks();

    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN("r3k2r/pppq1ppp/2npbn2/2b1p3/2B1P3/2NPBN2/PPPQ1PPP/R3K2R w KQkq - 6 8", boardPtr);
    int boardArray[8][8] = {
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32},
        {32,32,32,32,32,32,32,32}
    };
    mapBoard(board,boardArray);
    int moveList[256];
    struct BoardState boardCopy;
    boardPtr = &boardCopy;
    parseFEN("r3k2r/pppq1ppp/2npbn2/2b1p3/2B1P3/2NPBN2/PPPQ1PPP/R3K2R w KQkq - 6 8", boardPtr);
    return 0;
}
