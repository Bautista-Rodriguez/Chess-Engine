#include <stdio.h>
#include <stdlib.h>

#include "constsAndEnums.h"
#include "board/bitboard.h"
#include "board/moveGen.h"
#include "board/perft.h"
#include "search/search.h"
#include "search/transpositionTable.h"
#include "board/boardRepresentation.h"

//   0100   0000    0010 0100    0010 1101

int main()
{
    initMasks();
    initKeys();
    struct BoardState board;
    struct BoardState *boardPtr = &board;
    parseFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",boardPtr);
    printf("%lld",perftPosition(boardPtr, 5));
    /*int boardArray[8][8]=
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
    /*makeMove(boardPtr,encodeMove(0b0001,0,0,e2,e4),0);
    mapBoard(board,boardArray);
    printBoard(board,boardArray);
    printf("\n %llx\n",generateHashKey(board));
    makeMove(boardPtr,encodeMove(0b0000,1,0,d7,d6),0);
    mapBoard(board,boardArray);
    printBoard(board,boardArray);
    printf("\n %llx\n",generateHashKey(board));
    makeMove(boardPtr,encodeMove(0b0000,0,0,e4,e5),0);
    mapBoard(board,boardArray);
    printBoard(board,boardArray);
    printf("\n %llx\n",generateHashKey(board));
    makeMove(boardPtr,encodeMove(0,1,0,f7,f6),0);
    printf("\n %llx\n",(generateHashKey(board)));
    mapBoard(board,boardArray);
    printBoard(board,boardArray);*/

    return 0;
}
