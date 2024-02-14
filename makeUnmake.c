#include "constsAndEnums.h"
#include "attacks.h"
#include "moveGen.h"
#include "string.h"
#include "makeUnmake.h"

void copyBoardState(struct BoardState board,struct BoardState *boardCopy)
{
    memcpy(boardCopy->bitboards,board.bitboards,96);
    memcpy(boardCopy->occupancies,board.occupancies,24);
    boardCopy->sideToMove=board.sideToMove;
    boardCopy->castle=board.castle;
    boardCopy->fullMoveCount=board.fullMoveCount;
    boardCopy->halfMoveCount=board.halfMoveCount;
    boardCopy->enPassant=board.enPassant;
    return;
}

int makeMove(struct BoardState *board, int move)
{
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    copyBoardState(*board,boardCopyPtr);
    int toSquare = (move & 0xFFULL);
    int fromSquare = ((move>>8) & 0xFFULL);
    int piece = ((move>>16) & 0x7ULL);
    int side = ((move>>19) & 0x1ULL);
    int typeMove = ((move>>20) & 0xFULL);
    int isPromotion = (typeMove & (1ULL << 3));
    int isCapture = (typeMove & (1ULL << 2));
    int isEnPassant = (typeMove == 0b0101);
    int isDoublePush = (typeMove == 0b0001);
    int isCastle = (typeMove==2 || typeMove==3);
    int enemySide = (side+1) % 2;
    int inCheck = (isSquareAttacked(*board,bitScan(board->bitboards[5 + side*6]),enemySide));

    if(inCheck && isCastle)
        return 1;

    //printf("%s%s%c %s\n",
    //       squareChar[fromSquare],squareChar[toSquare],ascii[piece+side*6],typeMoveChar[typeMove]);
    //decodeMove(move);
    //printf("\n");
    board->bitboards[piece + side*6] ^= (1ULL << fromSquare);
    //board->occupancies[side] ^= (1ULL << fromSquare);
    //board->occupancies[2] ^= (1ULL << fromSquare);
    if(isPromotion)
    {
        int pieceProm = (typeMove & 0b0011);
        pieceProm++;
        board->bitboards[pieceProm + side*6] |= (1ULL << toSquare);
    }
    else
        board->bitboards[piece+side*6] |= (1ULL << toSquare);
    //board->occupancies[side] |= (1ULL << toSquare);
    //board->occupancies[2] |= (1ULL << toSquare);
    if(isCapture)
    {
        if(isEnPassant)
        {
            int enPassantSquare = (!enemySide) ? (toSquare +  8) : (toSquare - 8);
            board->bitboards[enemySide*6] &= ~(1ULL << enPassantSquare);
        }
        else
        {
            int enemyPiece = 0;
            U64 bitboard;
            for(int i = 0;i < 6;i++)
            {
                bitboard = board->bitboards[i + enemySide*6];
                if(bitboard & (1ULL << toSquare))
                {
                    //printf("YOU GOT ME!\n");
                    enemyPiece = i;
                    break;
                }
            }
            board->bitboards[enemyPiece + enemySide*6] &= ~(1ULL << toSquare);
            if(enemyPiece == 3)
            {
                if(toSquare == (a1 + enemySide*56))
                    board->castle &= ~((0b0010 << enemySide*2));
                else if(toSquare == (h1 + enemySide*56))
                    board->castle &= ~((0b0001 << enemySide*2));
            }
        }
        //printBitboard(board->bitboards[enemyPiece + enemySide*6]);
        //printf("\n");
        //printBitboard(board->bitboards[enemyPiece + enemySide*6]);
        //board->occupancies[enemySide] ^= (1ULL << (isEnPassant) ? enPassantSquare : toSquare);
        //board->occupancies[2] ^= (1ULL << (isEnPassant) ? enPassantSquare : 64);
    }
    board->enPassant = 65;
    if(isDoublePush)
        board->enPassant = ((!side) ? (toSquare -  8) : (toSquare + 8));
    if(isCastle)
    {
        int queenSide = (typeMove == 3);
        if(queenSide)
        {
            board->bitboards[3+side*6] &= ~(1ULL << (a1 + side*56));
            board->bitboards[3+side*6] |= (1ULL << (d1 + side*56));
            //board->occupancies[side] ^= a1 + side*56;
            //board->occupancies[side] |= (1ULL << (d1 + side*56));
            //board->occupancies[2] ^= a1 + side*56;
            //board->occupancies[2] |= (1ULL << (d1 + side*56));
        }
        else
        {
            board->bitboards[3+side*6] &= ~(1ULL << (h1 + side*56));
            board->bitboards[3+side*6] |= (1ULL << (f1 + side*56));
            //board->occupancies[side] ^= h1 + side*56;
            //board->occupancies[side] |= (1ULL << (f1 + side*56));
            //board->occupancies[2] ^= h1 + side*56;
            //board->occupancies[2] |= (1ULL << (f1 + side*56));
        }
    }
    if(piece == 5)
    {
        board->castle &= (0b1100 >> side*2);
    }
    if(piece == 3)
    {
        if(fromSquare == a8 || fromSquare == a1)
            board->castle &= ~((0b0010 << side*2));
        else if(fromSquare == h8 || fromSquare == h1)
            board->castle &= ~((0b0001 << side*2));
    }
    board->occupancies[0]=board->bitboards[0] | board->bitboards[1] | board->bitboards[2] |
    board->bitboards[3] | board->bitboards[4] | board->bitboards[5];
    board->occupancies[1]=board->bitboards[6] | board->bitboards[7] | board->bitboards[8] |
    board->bitboards[9] | board->bitboards[10] | board->bitboards[11];
    board->occupancies[2]= board->occupancies[0] | board->occupancies[1];
    if(isSquareAttacked(*board,bitScan(board->bitboards[5 + side*6]),enemySide))
    {
        copyBoardState(boardCopy,board);
        return 1;
    }
    board->sideToMove = enemySide;
    return 0;
}
