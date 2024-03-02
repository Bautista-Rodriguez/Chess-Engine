#include "moveGen.h"

int isSquareAttacked(struct BoardState board, int square, int side)
{
    U64 occupancy = board.occupancies[2];
    //leaper pieces
    if(pawnAttacks[!side][square] & board.bitboards[0+side*6]) return 1 + side*6;
    if(knightAttacks[square] & board.bitboards[1+side*6]) return 2 + side*6;
    if(kingAttacks[square] & board.bitboards[5+side*6]) return 3 + side*6;
    //slider pieces
    if(getBishopAttacks(square, occupancy) & board.bitboards[2+side*6]) return 4 + side*6;
    if(getRookAttacks(square, occupancy) & board.bitboards[3+side*6]) return 5 + side*6;
    if(getQueenAttacks(square, occupancy) & board.bitboards[4+side*6]) return 6 + side*6;

    return 0;
}

int inCheck(struct BoardState board)
{
    int side = board.sideToMove;
    return (isSquareAttacked(board,bitScan(board.bitboards[5 + side*6]),!side));
}

int encodeMove(int type, int side, int piece, int fromSquare, int toSquare)
{
    return ((type << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare);
}

void decodeMove(int move)
{
    int toSquare = (move & 0xFFULL);
    move >>=8;
    int fromSquare = (move & 0xFFULL);
    move >>=8;
    int piece = (move & 0x7ULL);
    move >>=3;
    int side = (move & 0x1ULL);
    move >>=1;
    int typeMove = (move & 0xFULL);
    printf("%s%s%c %s\n",
           squareChar[fromSquare],squareChar[toSquare],ascii[piece+side*6],typeMoveChar[typeMove]);
    return;
}

void moveGenerator(struct BoardState board, int moveArray[])
{
    U64  attacks = 0ULL;
    int fromSquare, toSquare;
    int moveIndex = 0;
    int side = board.sideToMove;
    for(int piece = 0;piece < 6;piece++)
    {
        U64 bitboard=0ULL;
        bitboard=board.bitboards[piece + side*6];
        while(bitboard)
        {
            fromSquare = bitScan(bitboard);
            bitboard ^= (1ULL << fromSquare);
            switch(piece)
            {
            case 0:
                toSquare = (!side) ? fromSquare + 8 : fromSquare - 8;
                if(((1ULL << toSquare) & board.occupancies[2]) == 0)
                {
                    if((fromSquare >= a7 && fromSquare <= h7 && !side) || (fromSquare >= a2 && fromSquare <= h2 && side))
                    {
                        moveArray[moveIndex] = encodeMove(knightProm,side,piece,fromSquare,toSquare);
                        moveArray[moveIndex+1] = encodeMove(bishopProm,side,piece,fromSquare,toSquare);
                        moveArray[moveIndex+2] = encodeMove(rookProm,side,piece,fromSquare,toSquare);
                        moveArray[moveIndex+3] = encodeMove(queenProm,side,piece,fromSquare,toSquare);
                        moveIndex += 4;
                    }
                    else
                    {
                        moveArray[moveIndex] = encodeMove(quietMove,side,piece,fromSquare,toSquare);
                        moveIndex++;
                    }
                    if((fromSquare < a3 && !side) || (fromSquare > h6 && side))
                    {
                        toSquare = (!side) ? fromSquare + 16 : fromSquare - 16;
                        if(((1ULL << toSquare) & board.occupancies[2]) == 0)
                        {
                            moveArray[moveIndex] = encodeMove(doublePush,side,piece,fromSquare,toSquare);
                            moveIndex++;
                        }
                    }
                }
                U64 enemyOccupancy=(board.occupancies[!side]);
                enemyOccupancy |= (0x1ULL << (board.enPassant));
                attacks = pawnAttacks[side][fromSquare];
                attacks &= enemyOccupancy;
                break;
            case 1:
                attacks=knightAttacks[fromSquare];
                break;
            case 2:
                attacks=getBishopAttacks(fromSquare, board.occupancies[2]);
                break;
            case 3:
                attacks=getRookAttacks(fromSquare, board.occupancies[2]);
                break;
            case 4:
                attacks=getQueenAttacks(fromSquare, board.occupancies[2]);
                break;
            case 5:
                if((board.castle & (1ULL << (0 + side*2))) &&
                   (((1ULL << (f1 + side*56)) & board.occupancies[2]) == 0) &&
                   (((1ULL << (g1 + side*56)) & board.occupancies[2]) == 0) &&
                   !isSquareAttacked(board,e1 + side*56,!side) &&
                   !isSquareAttacked(board,f1 + side*56,!side))
                {
                    moveArray[moveIndex] = encodeMove(castleKing,side,piece,fromSquare,(g1 + side*56));
                    moveIndex++;
                }
                if((board.castle & (1ULL << (1 + side*2))) &&
                   (((1ULL << (b1 + side*56)) & board.occupancies[2]) == 0) &&
                   (((1ULL << (c1 + side*56)) & board.occupancies[2]) == 0) &&
                   (((1ULL << (d1 + side*56)) & board.occupancies[2]) == 0) &&
                   !isSquareAttacked(board,e1 + side*56,!side) &&
                   !isSquareAttacked(board,c1 + side*56,!side) &&
                   !isSquareAttacked(board,d1 + side*56,!side))
                {
                    moveArray[moveIndex] = encodeMove(castleQueen,side,piece,fromSquare,(c1 + side*56));
                    moveIndex++;
                }
                attacks=kingAttacks[fromSquare];
                break;
            }
            while(attacks)
            {
                toSquare = bitScan(attacks);
                attacks ^= (1ULL << toSquare);
                if(((1ULL << toSquare) & board.occupancies[side]) == 0)
                {
                    if(!piece)
                    {
                        if((toSquare >= a8 && toSquare <= h8) || (toSquare >= a1 && toSquare <= h1))
                        {
                            moveArray[moveIndex] = encodeMove(knightPromCap,side,piece,fromSquare,toSquare);
                            moveArray[moveIndex+1] = encodeMove(bishopPromCap,side,piece,fromSquare,toSquare);
                            moveArray[moveIndex+2] = encodeMove(rookPromCap,side,piece,fromSquare,toSquare);
                            moveArray[moveIndex+3] = encodeMove(queenPromCap,side,piece,fromSquare,toSquare);
                            moveIndex += 4;
                        }
                        else if(board.enPassant == toSquare)
                        {
                            moveArray[moveIndex] =encodeMove(enPassant,side,piece,fromSquare,toSquare);
                            moveIndex++;
                        }
                        else
                        {
                            moveArray[moveIndex] =encodeMove(capture,side,piece,fromSquare,toSquare);
                            moveIndex++;
                        }
                    }
                    else if(((1ULL << toSquare) & board.occupancies[!side]))
                    {
                        moveArray[moveIndex] = encodeMove(capture,side,piece,fromSquare,toSquare);
                        moveIndex++;
                    }
                    else
                    {
                        moveArray[moveIndex] =  encodeMove(quietMove,side,piece,fromSquare,toSquare);
                        moveIndex++;
                    }
                }
            }
        }
    }
    /*if(moveIndex>=256)
    {
        printf("\n--------------------------------------------------------------------------------\n");
        printf("ERROR: NUMBER OF PSEUDO-LEGAL MOVES WAS GREATER THAN EXPECTED (GREATER THAN 255)");
        printf("\nACTUAL NUMBER: %i",moveIndex);
        printf("\n--------------------------------------------------------------------------------\n");
        exit(-1);
    }*/
    moveArray[moveIndex] = 0;
    return;
}

void copyBoardState(struct BoardState board,struct BoardState *boardCopy)
{
    memcpy(boardCopy->bitboards,board.bitboards,96);
    memcpy(boardCopy->occupancies,board.occupancies,24);
    boardCopy->sideToMove=board.sideToMove;
    boardCopy->castle=board.castle;
    boardCopy->fullMoveCount=board.fullMoveCount;
    boardCopy->halfMoveCount=board.halfMoveCount;
    boardCopy->enPassant=board.enPassant;
    boardCopy->hashKey=board.hashKey;
    return;
}

int makeMove(struct BoardState *board, int move, int captureFlag)
{
    if(captureFlag && !(getTypeMove(move) & capture))
        return 0;
    struct BoardState boardCopy;
    struct BoardState *boardCopyPtr = &boardCopy;
    copyBoardState(*board,boardCopyPtr);
    int toSquare = getToSquare(move);
    int fromSquare = getFromSquare(move);
    int piece = getPiece(move);
    int side = getSide(move);
    int typeMove = getTypeMove(move);
    int isPromotion = (typeMove & (1ULL << 3));
    int isCapture = (typeMove & (1ULL << 2));
    int isEnPassant = (typeMove == 0b0101);
    int isDoublePush = (typeMove == 0b0001);
    int isCastle = (typeMove==2 || typeMove==3);
    int enemySide = !side;
    board->bitboards[piece + side*6] ^= (1ULL << fromSquare);
    board->hashKey ^= pieceKeys[piece + side*6][fromSquare];
    if(isPromotion)
    {
        int pieceProm = (typeMove & 0b0011);
        pieceProm++;
        board->bitboards[pieceProm + side*6] |= (1ULL << toSquare);
        board->hashKey ^= (U64) pieceKeys[pieceProm + side*6][toSquare];
    }
    else
    {
        board->bitboards[piece + side*6] |= (1ULL << toSquare);
        board->hashKey ^= (U64) pieceKeys[piece + side*6][toSquare];
    }
    if(isCapture)
    {
        if(isEnPassant)
        {
            int enPassantSquare = ((!enemySide) ? (toSquare +  8) : (toSquare - 8));
            board->bitboards[enemySide*6] &= ~(1ULL << enPassantSquare);
            board->hashKey ^= (U64) pieceKeys[enemySide*6][enPassantSquare];
        }
        else
        {
            U64 bitboard;
            for(int i = 0;i < 6;i++)
            {
                bitboard = board->bitboards[i + enemySide*6];
                if(bitboard & (1ULL << toSquare))
                {
                    board->bitboards[i + enemySide*6] ^= (1ULL << toSquare);
                    board->hashKey ^= (U64) pieceKeys[i + enemySide*6][toSquare];
                    break;
                }
            }
        }
    }
    if(board->enPassant < 64)
    {
        int enPassantFile = board->enPassant % 8;
        board->hashKey ^= enPassantKeys[enPassantFile];
    }
    board->enPassant = 65;
    if(isDoublePush)
    {
        board->enPassant = ((!side) ? (toSquare -  8) : (toSquare + 8));
        board->hashKey ^= enPassantKeys[(board->enPassant % 8)];
    }
    if(isCastle)
    {
        int queenSide = (typeMove == 3);
        if(queenSide)
        {
            board->bitboards[3+side*6] &= ~(1ULL << (a1 + side*56));
            board->bitboards[3+side*6] |= (1ULL << (d1 + side*56));
            board->hashKey ^= pieceKeys[3 + side*6][a1 + side*56];
            board->hashKey ^= pieceKeys[3 + side*6][d1 + side*56];
        }
        else
        {
            board->bitboards[3+side*6] &= ~(1ULL << (h1 + side*56));
            board->bitboards[3+side*6] |= (1ULL << (f1 + side*56));
            board->hashKey ^= pieceKeys[3 + side*6][h1 + side*56];
            board->hashKey ^= pieceKeys[3 + side*6][f1 + side*56];
        }
    }
    board->hashKey ^= castleKeys[board->castle];
    board->castle &= castlingRights[fromSquare];
    board->castle &= castlingRights[toSquare];
    board->hashKey ^= castleKeys[board->castle];
    board->sideToMove = enemySide;
    board->hashKey ^= sideKey;
    board->occupancies[0]=board->bitboards[0] | board->bitboards[1] | board->bitboards[2] |
    board->bitboards[3] | board->bitboards[4] | board->bitboards[5];
    board->occupancies[1]=board->bitboards[6] | board->bitboards[7] | board->bitboards[8] |
    board->bitboards[9] | board->bitboards[10] | board->bitboards[11];
    board->occupancies[2]= board->occupancies[0] | board->occupancies[1];
    if(isSquareAttacked(*board,bitScan(board->bitboards[5 + side*6]),enemySide))
    {
        copyBoardState(boardCopy,board);
        return 0;
    }
    return 1;
}


/// 0000 1011 0010 0101 0010 1010 0011 1111 0010 1011 1101 1011 1100 0110 1000 1011 0110
/// 0000 1011 0101 0010 1101 1001 0000 1111 1101 0101 1000 0010 1101 0001 0001 1111 1001
