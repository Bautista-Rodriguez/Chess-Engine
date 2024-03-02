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
    HashT ss;
    printf("%i",(4294967296 / sizeof(ss)));
    return 0;
    initMasks();
    initKeys();
    initHashTable();
    clearHashTable();
    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN(startingPosition,boardPtr);
    printBoard(board);
    getchar();
    freeHashTable();
    return 0;
}
