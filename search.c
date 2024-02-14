#include "constsAndEnums.h"
#include "search.h"

int negamax(struct BoardState board, int alpha, int beta, int depth, int *bestMove)
{
    int ply = 0;
    int bestCurrentMove;
    int oldAlpha = alpha;
    if(depth==0)
        //EVALUATION TO BE IMPLEMENTED
        return 0;
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
    if(oldAlpha != alpha)
        *bestMove = bestCurrentMove;
    return alpha;
}
