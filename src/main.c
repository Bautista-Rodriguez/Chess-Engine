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
    parseFEN(startingPosition,boardPtr);
    searchMove(board,6);
    return 0;
}
