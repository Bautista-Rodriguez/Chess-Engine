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
    parseFEN("8/8/6pr/6p1/5pPk/5P1p/5P1K/R7 w - - 0 1",boardPtr);
    searchMove(board,8);
    return 0;
}
