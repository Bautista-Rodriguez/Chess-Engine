#include <stdio.h>
#include <stdlib.h>

#include "constsAndEnums.h"
#include "board/boardRepresentation.h"
#include "board/bitboard.h"
#include "board/moveGen.h"
#include "board/perft.h"
#include "search/search.h"
#include "search/transpositionTable.h"

//   0100   0000    0010 0100    0010 1101

int main()
{
    initMasks();
    initKeys();
    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3",boardPtr);
    int boardArray[8][8]=
    {
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32
    };
    mapBoard(board,boardArray);
    printBoard(board,boardArray);
    //searchMove(board,8);
    makeMove(boardPtr,0b010100000010010000101101ULL,0);
    //decodeMove(0b010100000010010000101101ULL);
    mapBoard(board,boardArray);
    printBoard(board,boardArray);
    printf("\n %llx",generateHashKey(board));
    return 0;
}
