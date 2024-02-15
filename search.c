#include "constsAndEnums.h"
#include "moveGen.h"
#include "search.h"

int scoreMove(struct BoardState board, int move)
{
    //is capture
    if((move >> 20) & capture)
    {
        int side = (move >> 19);
        int attackPiece = ((move >> 16) & 7);
        int toSquare = move & 0xFFULL;
        attackPiece += side * 6;
        int victimPiece = 0;
        U64 bitboard;
        for(int i = 0;i < 6;i++)
        {
            bitboard = board.bitboards[i + (!side)*6];
            if(bitboard & (1ULL << toSquare))
            {
                victimPiece = i;
                break;
            }
        }
        victimPiece += (!side)*6;
        return mvvLva[attackPiece][victimPiece];
    }
    else
    {
        return 0;
    }
    exit(-1);
}

int quietSearch(struct BoardState board, int alpha, int beta)
{
    int evaluation = 0;//evaluate();
    if(evaluation >= beta)
    {
        return beta;
    }
    if(evaluation > alpha)
    {
        alpha = evaluation;

    }
    int ply = 0;
    int bestCurrentMove;
    int oldAlpha = alpha;
    int side = board.sideToMove, legalMoves = 0;
    int inCheck = isSquareAttacked(board,bitScan(board.bitboards[5 + side*6]),!side);
    int moveList[256];
    moveGenerator(board,moveList);
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    struct BoardState *boardPtr = &board;
    for(int i = 0;moveList[i] != 0;i++)
    {
        copyBoardState(board,boardCopyPtr);
        ply++;
        if(makeMove(boardPtr,moveList[i],1));
        {
            ply--;
            continue;
        }
        legalMoves++;
        int score = -quietSearch(board,-beta,-alpha);
        ply--;
        copyBoardState(boardCopy,boardPtr);
        if(score >= beta)
        {
            return beta;
        }
        if(score > alpha)
        {
            alpha = score;

        }
    }
    return alpha;
}

int negamax(struct BoardState board, int alpha, int beta, int depth, int *bestMove)
{
    int ply = 0;
    int bestCurrentMove;
    int oldAlpha = alpha;
    if(depth==0)
        //EVALUATION TO BE IMPLEMENTED
        return quietSearch(board,alpha,beta);
    int side = board.sideToMove, legalMoves = 0;
    int inCheck = isSquareAttacked(board,bitScan(board.bitboards[5 + side*6]),!side);
    int moveList[256];
    moveGenerator(board,moveList);
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    struct BoardState *boardPtr = &board;
    for(int i = 0;moveList[i] != 0;i++)
    {
        copyBoardState(board,boardCopyPtr);
        ply++;
        if(makeMove(boardPtr,moveList[i]));
        {
            ply--;
            continue;
        }
        legalMoves++;
        int score = -negamax(board,-beta,-alpha,depth-1, bestMove);
        ply--;
        copyBoardState(boardCopy,boardPtr);

        if(score >= beta)
        {
            return beta;
        }
        if(score > alpha)
        {
            alpha = score;
            if(ply == 0)
            {
                bestCurrentMove = moveList[i];
            }
        }
    }
    if(legalMoves == 0)
    {
        if(inCheck)
        {
            return -49000 + ply;
        }
        else
        {
            return 0;
        }
    }

    if(oldAlpha != alpha)
        *bestMove = bestCurrentMove;
    return alpha;
}
