#include <stdio.h>
#include <stdlib.h>

#include "constsAndEnums.h"
#include "board/boardRepresentation.h"
#include "board/bitboard.h"
#include "board/moveGen.h"
#include "board/perft.h"
#include "search/search.h"

int main()
{
    initMasks();
    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",boardPtr);
    searchMove(board,7);
    return 0;
}
