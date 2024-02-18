#include "search.h"

int killerMoves[2][400];
int historyMoves[12][64];
int ply;

void searchMove(struct BoardState board, int depth)
{
    int bestMove = 0;
    int *bestMovePtr = &bestMove;
    int score = negamax(board, -50000, 50000, depth, bestMovePtr);
    if (bestMove)
    {
        exit(1);
    }
    exit(-1);
}

int negamax(struct BoardState board, int alpha, int beta, int depth, int *bestMove)
{
    if(depth==0)
        return quietSearch(board,alpha,beta);

    if(inCheck(board))
        depth++;

    int oldAlpha = alpha, legalMoves = 0, moveList[1000],
    bestCurrentMove, side = board.sideToMove;
    moveGenerator(board,moveList);
    sortMoves(board,moveList);
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    struct BoardState *boardPtr = &board;
    for(int i = 0;moveList[i] != 0;i++)
    {
        copyBoardState(board,boardCopyPtr);
        ply++;
        if(!makeMove(boardPtr,moveList[i]))
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
            killerMoves[1][ply] = killerMoves[0][ply];
            killerMoves[0][ply] = moveList[i];
            return beta;
        }
        if(score > alpha)
        {
            int piece = getPiece(moveList[i]);
            int side = getSide(moveList[i]);
            int toSquare = getToSquare(moveList[i]);
            piece += side * 6;
            historyMoves[piece][toSquare] += depth;
            alpha = score;
            if(ply == 0)
                bestCurrentMove = moveList[i];
        }
    }
    if(legalMoves == 0)
    {
        //TEST THAT INCHECK WORKS
        if(inCheck(board))
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

int quietSearch(struct BoardState board, int alpha, int beta)
{
    int evaluation = 0;//evaluatePositionFunction(); - TO BE IMPLEMENTED
    if(evaluation >= beta)
    {
        return beta;
    }
    if(evaluation > alpha)
    {
        alpha = evaluation;

    }
    int side = board.sideToMove, legalMoves = 0;
    int moveList[256];
    moveGenerator(board,moveList);
    sortMoves(board,moveList);
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    struct BoardState *boardPtr = &board;
    for(int i = 0;moveList[i] != 0;i++)
    {
        copyBoardState(board,boardCopyPtr);
        ply++;
        //if(!makeMove(boardPtr,moveList[i],1));
        {
            ply--;
            continue;
        }
        legalMoves++;
        int score = -quietSearch(board,-beta,-alpha);
        ply--;
        copyBoardState(boardCopy,boardPtr);
        if(score >= beta)
            return beta;
        if(score > alpha)
            alpha = score;
    }
    return alpha;
}

int scoreMove(struct BoardState board, int move)
{
    int side = getSide(move);
    int piece = getPiece(move);
    int toSquare = getToSquare(move);
    //is capture
    if((move >> 20) & capture)
    {
        piece += side * 6;
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
        return (mvvLva[piece][victimPiece] + 10000);
    }
    if(killerMoves[0][ply] == move)
        return 9000;
    if(killerMoves[1][ply] == move)
        return 8000;
    return historyMoves[piece][toSquare];
}

int sortMoves(struct BoardState board, int moveList[])
{
    int scores[256];
    for(int i = 0;moveList[i] != 0;i++)
        scores[i] = scoreMove(board,moveList[i]);
    for(int i = 0;moveList[i] != 0;i++)
    {
        for(int j = i + 1;moveList[j] != 0;j++)
        {
            if(scores[i]<scores[j])
            {
                int auxScore = scores[i];
                scores[i] = scores[j];
                scores[j] = auxScore;
                int auxMove = moveList[i];
                moveList[i] = moveList[j];
                moveList[j] = auxMove;
            }
        }
    }
    return 0;
}
