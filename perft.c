#include "perft.h"
#include "boardRepresentation.h"
#include "constsAndEnums.h"
#include "moveGen.h"
#include "makeUnmake.h"

#include "attacks.h"
#include "bitboard.h"

void perftTest()
{
    const U64 nodesPerDepth[6][6]=
    {{20,400,8902,197281,4865609,119060324},
     {48,2039,97862,4085603,193690690,8031647685},
     {14,191,2812,43238,674624,11030083},
     {6,264,9467,422333,15833292,706045033},
     {44,1486,62379,2103487,89941194,0},
     {46,2079,89890,3894594,164075551,6923051137}
    };
    const char positions[6][100]=
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
    };
    struct BoardState board;
    struct BoardState *boardPtr=&board;
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr=&boardCopy;

    ////
    /*
    for(int i = 4;i < 5;i++)
    {
        /*parseFEN("r3k2r/p1p1qpb1/bn1ppnp1/1B1PN3/1p2P3/P1N2Q1p/1PPB1PPP/R3K2R b KQkq - 1 2",boardPtr);
        //makeMove(boardPtr,0b000000100000110000100001);
        //       decodeMove(0b000000100000110000100001);
        /*int bb[8][8]={32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32,
                32,32,32,32,32,32,32,32};
        mapBoard(board,bb);
        printBoard(board,bb);
        printf("\n");

        int moveArray[256];
        int totalNodes=0, nodes=0;
    moveGenerator(board, moveArray);
    for(int i = 0;moveArray[i] != 0;i++)
    {
        copyBoardState(board,boardCopyPtr);

        if(!makeMove(boardPtr,moveArray[i])){
                decodeMove(moveArray[i]);
            nodes=perftPosition(boardPtr,0);
        }
        else
            nodes=0;
        if(nodes)
            printf("----------Nodes: %i------------\n",nodes);
        totalNodes+=nodes;
        copyBoardState(boardCopy,boardPtr);

    }
    printf("Total Nodes: %i\n",totalNodes);
    }
    */
    ////

    for(int i = 0;i < 6;i++)
    {
        parseFEN(positions[i],boardPtr);
        for(int j = 0;j < 4;j++)
        {
            U64 nodes;
            nodes=perftPosition(boardPtr,j+1);
            printf("Nodes in pos %i depth %i: %i\n\n",i+1,j+1,nodes);
            if(nodes==nodesPerDepth[i][j])
                printf("SUCCESS!\n\n");
            else
                printf("FAILURE :(\n\n");
        }
    }
    return;
}


U64 perftPosition(struct BoardState *board, int depth)
{
    if(depth==0)
        return 1;
    struct BoardState initBoard;
    struct BoardState *initBoardPtr=&initBoard;
    U64 nodes = 0;
    int moveArray[256];
    moveGenerator(*board, moveArray);
    for(int i = 0;moveArray[i] != 0;i++)
    {
        copyBoardState(*board,initBoardPtr);
        if(!makeMove(board,moveArray[i]))
        {
            //printf("--\n\n");
            //decodeMove(moveArray[i]);
            //printf("--\n\n");
            nodes += perftPosition(board,depth - 1);
        }
        copyBoardState(initBoard,board);
    }

    return nodes;
}


