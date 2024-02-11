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
    initMaskPawnAttacks();
    initMaskKnightAttacks();
    initMaskKingAttacks();
    initMaskRookRay();
    initMaskBishopRay();
    initMaskQueenRay();
    initRookOccupancyAttacks();
    initBishopOccupancyAttacks();

    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN(startingPosition, boardPtr);
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
    //printBoard(board,boardArray);
    moveGenerator(board,0);
    return 0;
}
