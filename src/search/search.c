#include "search.h"

int killerMoves[2][400];
int historyMoves[12][64];
int ply;
int nodes;
int pvLength[400];
int pvTable[400][400];

const int fullDepth = 4;
const int reductionLimit = 3;

void searchMove(struct BoardState board, int depth)
{
    int score;
    memset(killerMoves,0,sizeof(killerMoves));
    memset(historyMoves,0,sizeof(historyMoves));
    memset(pvLength,0,sizeof(pvLength));
    memset(pvTable,0,sizeof(pvTable));

    /**
    RESEARCH ABOUT:
    ITERATIVE DEEPENING vs DIRECT DEPTH SEARCH
    **/
    int alpha = -200000, beta = 200000;
    for(int i = 0;i < depth;i++)
    {
        nodes=0;
        score = negamax(board, alpha, beta, i+1);
        if((score <= alpha) || (score >= beta))
        {
            alpha = -200000;
            beta = 200000;
            continue;
        }
        alpha = score - 50;
        beta = score + 50;
        printf("depth %i: %i\n",i+1, nodes);
    }
    decodeMove(pvTable[0][0]);
    decodeMove(pvTable[0][1]);
    decodeMove(pvTable[0][2]);
    decodeMove(pvTable[0][3]);
    decodeMove(pvTable[0][4]);
    decodeMove(pvTable[0][5]);
    decodeMove(pvTable[0][6]);
    decodeMove(pvTable[0][7]);
    exit(17);
}

int negamax(struct BoardState board, int alpha, int beta, int depth)
{
    if(ply > 399)
    {
    printf("ERROR: UNEXPECTED NUMBER OF PLY\n");
    exit(11);
    return evaluate(board);
    }
    pvLength[ply] = ply;
    if(depth==0)
        return quietSearch(board,alpha,beta);
    int isInCheck = inCheck(board);
    if(isInCheck)
        depth++;
    nodes++;
    int legalMoves = 0;
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    struct BoardState *boardPtr = &board;
    if(depth >= 3 && !isInCheck && ply)
    {
        copyBoardState(board,boardCopyPtr);
        board.sideToMove ^= 1;
        board.enPassant = 65;
        int score = -negamax(board,-beta,-beta + 1,depth - 3);
        copyBoardState(boardCopy,boardPtr);
        if(score >= beta)
            return beta;
    }
    int moveList[256], side = board.sideToMove;
    moveGenerator(board,moveList);
    sortMoves(board,moveList);
    int movesSearched = 0;
    for(int i = 0;moveList[i] != 0;i++)
    {
        copyBoardState(board,boardCopyPtr);
        ply++;
        if(!makeMove(boardPtr,moveList[i],0))
        {
            ply--;
            continue;
        }
        legalMoves++;
        int score;
        if(!movesSearched)
            score = -negamax(board, -beta, -alpha, depth - 1);
        else
        {
            int typeMove = getTypeMove(moveList[i]);
            if((movesSearched >= fullDepth) && (depth >= reductionLimit) && !isInCheck
               && !(typeMove & capture) && !(typeMove & promotion))
                score = -negamax(board, -alpha - 1, -alpha, depth - 2);
            else
                score = alpha + 1;
            if(score > alpha)
            {
                score = -negamax(board, -alpha - 1, -alpha, depth - 1);
                if((score > alpha) && (score < beta))
                    score = -negamax(board, -beta, -alpha, depth - 1);
            }
        }
        ply--;
        copyBoardState(boardCopy,boardPtr);
        movesSearched++;

        if(score >= beta)
        {
            int typeMove = getTypeMove(moveList[i]);
            if((typeMove & capture) == 0)
            {
                killerMoves[1][ply] = killerMoves[0][ply];
                killerMoves[0][ply] = moveList[i];
            }
            return beta;
        }
        if(score > alpha)
        {
            int piece = getPiece(moveList[i]);
            int side = getSide(moveList[i]);
            int toSquare = getToSquare(moveList[i]);
            piece += side * 6;
            int typeMove = getTypeMove(moveList[i]);
            if((typeMove & capture) == 0)
                historyMoves[piece][toSquare] += depth;
            alpha = score;
            pvTable[ply][ply] = moveList[i];
            for(int i = ply + 1;i < pvLength[ply + 1];i++)
                pvTable[ply][i] = pvTable[ply + 1][i];
            pvLength[ply] = pvLength[ply + 1];
        }
    }
    if(legalMoves == 0)
    {
        ///TEST THAT INCHECK WORKS
        if(inCheck(board))
        {
            return -49000 + ply;
        }
        else
        {
            return 0;
        }
    }
    return alpha;
}

int quietSearch(struct BoardState board, int alpha, int beta)
{
    nodes++;
    int evaluation = evaluate(board);
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
        if(!makeMove(boardPtr,moveList[i],1));
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
    ///is capture
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
    /**TEST WHERE TO WRITE THIS LINE
    if(move == pvTable[0][ply]) return 20000;
    **/
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

int evaluate(struct BoardState board)
{
    return 1;
}
