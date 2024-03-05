#include "search.h"

int killerMoves[2][400];
int historyMoves[12][64];
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
    //clearHashTable();

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
    decodeMove(pvTable[0][8]);
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
    int score;
    int hashFlag = alphaHF;
    if(ply && isRepetition(board.hashKey))
        return 0;
    if(ply && !(beta - alpha > 1) && ((score = hashTableRead(alpha,beta,depth,board.hashKey)) != 300000))
        return score;
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
        ply++;
        repetitionIndex++;
        repetitionTable[repetitionIndex] = board.hashKey;
        if(board.enPassant != 65)
            boardPtr->hashKey ^= enPassantKeys[board.enPassant];
        boardPtr->hashKey ^= sideKey;
        board.sideToMove ^= 1;
        board.enPassant = 65;
        score = -negamax(board,-beta,-beta + 1,depth - 3);
        ply--;
        repetitionIndex--;
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
        repetitionIndex++;
        repetitionTable[repetitionIndex] = board.hashKey;
        if(!makeMove(boardPtr,moveList[i],0))
        {
            ply--;
            repetitionIndex--;
            continue;
        }
        legalMoves++;
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
                score = -negamax(board,-alpha - 1,-alpha,depth - 1);
                if((score > alpha) && (score < beta))
                    score = -negamax(board,-beta,-alpha,depth - 1);
            }
        }
        ply--;
        repetitionIndex--;
        copyBoardState(boardCopy,boardPtr);
        movesSearched++;

        if(score > alpha)
        {
            hashFlag = exactHF;
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
            if(score >= beta)
            {
                hashTableWrite(beta,depth,betaHF,board.hashKey);
                int typeMove = getTypeMove(moveList[i]);
                if((typeMove & capture) == 0)
                {
                    killerMoves[1][ply] = killerMoves[0][ply];
                    killerMoves[0][ply] = moveList[i];
                }
                return beta;
            }
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
    hashTableWrite(alpha,depth,hashFlag,board.hashKey);
    return alpha;
}

int quietSearch(struct BoardState board, int alpha, int beta)
{
    if(ply > 399)
    {
    printf("ERROR: UNEXPECTED NUMBER OF PLY\n");
    exit(11);
    return evaluate(board);
    }
    nodes++;
    if(ply && isRepetition(board.hashKey))
        return 0;
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
        repetitionIndex++;
        repetitionTable[repetitionIndex] = board.hashKey;
        if(!makeMove(boardPtr,moveList[i],1));
        {
            ply--;
            repetitionIndex--;
            continue;
        }
        legalMoves++;
        int score = -quietSearch(board,-beta,-alpha);
        ply--;
        repetitionIndex--;
        copyBoardState(boardCopy,boardPtr);
        if(score > alpha)
        {
            alpha = score;
            if(score >= beta)
                return beta;
        }
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

/*
int evaluate(struct BoardState board)
{
    return 1;
}*/

/**********************************\
 ==================================

             Evaluation

TEMPORARY EVALUATION TAKEN FROM ANOTHER
SOURCE CODE, AS IT WILL LATER BE REPLACED

I COPIED IT ONLY FOR TESTING PURPOSES

 ==================================
\**********************************/


int material_score[12] = {
    100,      // white pawn score
    300,      // white knight scrore
    350,      // white bishop score
    500,      // white rook score
   1000,      // white queen score
  10000,      // white king score
   -100,      // black pawn score
   -300,      // black knight scrore
   -350,      // black bishop score
   -500,      // black rook score
  -1000,      // black queen score
 -10000,      // black king score
};

// pawn positional score
const int pawn_score[64] =
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] =
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rook_score[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
const int king_score[64] =
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};

// mirror positional score tables for opposite side
const int mirror_score[128] =
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

// position evaluation
int evaluate(struct BoardState board)
{
    // static evaluation score
    int score = 0;

    // current pieces bitboard copy
    U64 bitboard;

    // init piece & square
    int piece, square;

    // loop over piece bitboards
    for (int bb_piece = 0; bb_piece < 12; bb_piece++)
    {
        // init piece bitboard copy
        bitboard = board.bitboards[bb_piece];

        // loop over pieces within a bitboard
        while (bitboard)
        {
            // init piece
            piece = bb_piece;

            // init square
            square = bitScan(bitboard);

            // score material weights
            score += material_score[piece];

            // score positional piece scores
            switch (piece)
            {
                // evaluate white pieces
                case 0: score += pawn_score[square]; break;
                case 1: score += knight_score[square]; break;
                case 2: score += bishop_score[square]; break;
                case 3: score += rook_score[square]; break;
                case 5: score += king_score[square]; break;

                // evaluate black pieces
                case 6: score -= pawn_score[mirror_score[square]]; break;
                case 7: score -= knight_score[mirror_score[square]]; break;
                case 8: score -= bishop_score[mirror_score[square]]; break;
                case 9: score -= rook_score[mirror_score[square]]; break;
                case 11: score -= king_score[mirror_score[square]]; break;
                default: break;
            }


            // pop ls1b
            bitboard ^= (1ULL << square);
        }
    }

    // return final evaluation based on side
    return (board.sideToMove == 0) ? score : -score;
}
