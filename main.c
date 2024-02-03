#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "constsAndEnums.h"
#include "boardRepresentation.h"
#include "attacks.h"
#include "bitboard.h"

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
    struct BoardState *bbb = &board;
    parseFEN(startingPosition, bbb);
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
    printBoard(board, boardArray);

    return 0;
}
