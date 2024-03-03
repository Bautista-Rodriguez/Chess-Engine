#include <stdio.h>
#include <stdlib.h>

#include "constsAndEnums.h"
#include "board/bitboard.h"
#include "board/moveGen.h"
#include "board/perft.h"
#include "search/search.h"
#include "search/transpositionTable.h"
#include "board/boardRepresentation.h"

int main()
{
    initMasks();
    initHashTable();
    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",boardPtr);
    //printBoard(board);
    searchMove(board,8);
    freeHashTable();
    return 0;
}
