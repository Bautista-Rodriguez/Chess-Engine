#include "perft.h"
#include "boardRepresentation.h"
#include "constsAndEnums.h"
#include "moveGen.h"
#include "makeUnmake.h"

#include "attacks.h"
#include "bitboard.h"

static const char cpositions[128][100] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    "4k3/8/8/8/8/8/8/4K2R w K - 0 1",
    "4k3/8/8/8/8/8/8/R3K3 w Q - 0 1",
    "4k2r/8/8/8/8/8/8/4K3 w k - 0 1",
    "r3k3/8/8/8/8/8/8/4K3 w q - 0 1",
    "4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1",
    "r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1",
    "8/8/8/8/8/8/6k1/4K2R w K - 0 1",
    "8/8/8/8/8/8/1k6/R3K3 w Q - 0 1",
    "4k2r/6K1/8/8/8/8/8/8 w k - 0 1",
    "r3k3/1K6/8/8/8/8/8/8 w q - 0 1",
    "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1",
    "r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1",
    "r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1",
    "r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1",
    "1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1",
    "2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1",
    "r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1",
    "8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1",
    "8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1",
    "8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1",
    "K7/8/2n5/1n6/8/8/8/k6N w - - 0 1",
    "k7/8/2N5/1N6/8/8/8/K6n w - - 0 1",
    "8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1",
    "8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1",
    "8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1",
    "K7/8/2n5/1n6/8/8/8/k6N b - - 0 1",
    "k7/8/2N5/1N6/8/8/8/K6n b - - 0 1",
    "B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1",
    "8/8/2k5/3b4/8/3B4/8/2K5 w - - 0 1",
    "5b2/5B2/2k5/8/2K5/8/8/8 w - - 0 1",
    "3k4/3p4/8/8/8/8/3P4/3K4 w - - 0 1",
    "6k1/5pp1/8/8/8/8/PP4PP/6K1 w - - 0 1",
    "8/Pk6/8/8/8/8/6Kp/8 w - - 0 1",
    "8/8/3p4/2pP4/8/8/8/8 w - d3 0 1",
    "8/8/3p4/2pP4/8/8/8/8 w - d6 0 1",
    "8/8/8/8/8/8/6k1/4K2R b K - 0 1",
    "8/8/8/8/8/8/1k6/R3K3 b Q - 0 1",
    "4k2r/6K1/8/8/8/8/8/8 b k - 0 1",
    "r3k3/1K6/8/8/8/8/8/8 b q - 0 1",
    "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",
    "r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1",
    "r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1",
    "r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1",
    "1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1",
    "2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1",
    "r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1",
    "K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1",
    "B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1",
    "8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1",
    "k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1",
    "K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1",
    "K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1",
    "7k/RR6/8/8/8/8/rr6/7K w - - 0 1",
    "R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1",
    "7k/RR6/8/8/8/8/rr6/7K b - - 0 1",
    "R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1",
    "6kq/8/8/8/8/8/8/7K w - - 0 1",
    "6KQ/8/8/8/8/8/8/7k b - - 0 1",
    "K7/8/8/3Q4/4q3/8/8/7k w - - 0 1",
    "6qk/8/8/8/8/8/8/7K b - - 0 1",
    "6KQ/8/8/8/8/8/8/7k b - - 0 1",
    "K7/8/8/3Q4/4q3/8/8/7k b - - 0 1",
    "8/8/8/8/8/K7/P7/k7 w - - 0 1",
    "8/8/8/8/8/7K/7P/7k w - - 0 1",
    "K7/p7/k7/8/8/8/8/8 w - - 0 1",
    "7K/7p/7k/8/8/8/8/8 w - - 0 1",
    "8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1",
    "8/8/8/8/8/K7/P7/k7 b - - 0 1",
    "8/8/8/8/8/7K/7P/7k b - - 0 1",
    "K7/p7/k7/8/8/8/8/8 b - - 0 1",
    "7K/7p/7k/8/8/8/8/8 b - - 0 1",
    "8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1",
    "8/8/8/8/8/4k3/4P3/4K3 w - - 0 1",
    "4k3/4p3/4K3/8/8/8/8/8 b - - 0 1",
    "8/8/7k/7p/7P/7K/8/8 w - - 0 1",
    "8/8/k7/p7/P7/K7/8/8 w - - 0 1",
    "8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1",
    "8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1",
    "8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1",
    "k7/8/3p4/8/3P4/8/8/7K w - - 0 1",
    "8/8/7k/7p/7P/7K/8/8 b - - 0 1",
    "8/8/k7/p7/P7/K7/8/8 b - - 0 1",
    "8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1",
    "8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1",
    "8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1",
    "k7/8/3p4/8/3P4/8/8/7K b - - 0 1",
    "7k/3p4/8/8/3P4/8/8/K7 w - - 0 1",
    "7k/8/8/3p4/8/8/3P4/K7 w - - 0 1",
    "k7/8/8/7p/6P1/8/8/K7 w - - 0 1",
    "k7/8/7p/8/8/6P1/8/K7 w - - 0 1",
    "k7/8/8/6p1/7P/8/8/K7 w - - 0 1",
    "k7/8/6p1/8/8/7P/8/K7 w - - 0 1",
    "k7/8/8/3p4/4p3/8/8/7K w - - 0 1",
    "k7/8/3p4/8/8/4P3/8/7K w - - 0 1"
};

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
        parseFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBPPP3/q4N2/P5PP/r2Q1RK1 w kq - 0 2",boardPtr);
        printBitboard(board.occupancies[2]);
        printf("\n");
        makeMove(boardPtr,0b010001000000001100000000);
               decodeMove(0b010001000000001100000000);
        int bb[8][8]={32,32,32,32,32,32,32,32,
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
            nodes=perftPosition(boardPtr,1);
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

    for(int i = 0;i < 1;i++)
    {
        parseFEN(positions[i],boardPtr);
        for(int j = 6;j < 7;j++)
        {
            U64 nodes;
            nodes=perftPosition(boardPtr,j+1);
            printf("Nodes in pos %i depth %i: %i\n\n",i+1,j+1,nodes);
            //if(nodes==nodesPerDepth[i][j])
            //    printf("SUCCESS!\n\n");
            //else
            //    printf("FAILURE :(\n\n");
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
            nodes += perftPosition(board,depth - 1);
        copyBoardState(initBoard,board);
    }

    return nodes;
}


