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
    parseFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",boardPtr);
    searchMove(board,8);
    return 0;
}
