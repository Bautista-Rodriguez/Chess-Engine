#include "constsAndEnums.h"
#include "attacks.h"
#include "moveGen.h"
#include "string.h"

int isSquareAttacked(struct BoardState board, int square, int side)
{
    U64 occupancy = board.occupancies[2];
    //leaper pieces
    if(pawnAttacks[(side+1)%2][square] & board.bitboards[0+side*6]) return 1 + side*6;
    if(knightAttacks[square] & board.bitboards[1+side*6]) return 2 + side*6;
    if(kingAttacks[square] & board.bitboards[5+side*6]) return 3 + side*6;
    //slider pieces
    if(getBishopAttacks(square, occupancy) & board.bitboards[2+side*6]) return 4 + side*6;
    if(getRookAttacks(square, occupancy) & board.bitboards[3+side*6]) return 5 + side*6;
    if(getQueenAttacks(square, occupancy) & board.bitboards[4+side*6]) return 6 + side*6;

    return 0;
}

//type piece   from      to
//0000 0000 0000 0000 0000 0000

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
            bitboard &= bitboard - 1;
            switch(piece)
            {
            case 0:
                toSquare = (!side) ? fromSquare + 8 : fromSquare - 8;
                if(((1ULL << toSquare) & board.occupancies[2]) == 0)
                {
                    if((fromSquare >= a7 && fromSquare <= h7 && !side) || (fromSquare >= a2 && fromSquare <= h2 && side))
                    {
                        moveArray[moveIndex] = (knightProm << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveArray[moveIndex+1] = (bishopProm << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveArray[moveIndex+2] = (rookProm << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveArray[moveIndex+3] = (queenProm << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveIndex += 4;
                    }
                    else
                    {
                        moveArray[moveIndex] = (quietMove << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveIndex++;
                    }
                    if((fromSquare < a3 && !side) || (fromSquare > h6 && side))
                    {
                        toSquare = (!side) ? fromSquare + 16 : fromSquare - 16;
                        if(((1ULL << toSquare) & board.occupancies[2]) == 0)
                        {
                            moveArray[moveIndex] = (doublePush << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveIndex++;
                        }
                    }
                }
                U64 aaa=(board.occupancies[(side+1)%2]);
                aaa |= (0x1ULL << (board.enPassant));
                attacks = pawnAttacks[side][fromSquare];
                attacks &= aaa;
                //printf("enpassant: %i\n",board.enPassant);
                //attacks |= (1ULL << board.enPassant);
                //printBitboard(attacks);
                //printf("\n\n");
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
                   !isSquareAttacked(board,e1 + side*56,(side+1)%2) &&
                   !isSquareAttacked(board,f1 + side*56,(side+1)%2))
                {
                    moveArray[moveIndex] = (castleKing << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + (g1 + side*56);
                    moveIndex++;
                }
                if((board.castle & (1ULL << (1 + side*2))) &&
                   (((1ULL << (b1 + side*56)) & board.occupancies[2]) == 0) &&
                   (((1ULL << (c1 + side*56)) & board.occupancies[2]) == 0) &&
                   (((1ULL << (d1 + side*56)) & board.occupancies[2]) == 0) &&
                   !isSquareAttacked(board,e1 + side*56,(side+1)%2) &&
                   !isSquareAttacked(board,c1 + side*56,(side+1)%2) &&
                   !isSquareAttacked(board,d1 + side*56,(side+1)%2))
                {
                    moveArray[moveIndex] = (castleQueen << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + (c1 + side*56);
                    moveIndex++;
                }
                attacks=kingAttacks[fromSquare];
                break;
            }
            while(attacks)
            {
                toSquare = bitScan(attacks);
                attacks &= attacks - 1;
                if(((1ULL << toSquare) & board.occupancies[side]) == 0)
                {
                    if(!piece)
                    {
                        if((toSquare >= a8 && toSquare <= h8) || (toSquare >= a1 && toSquare <= h1))
                        {
                            moveArray[moveIndex] = (knightPromCap << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveArray[moveIndex+1] = (bishopPromCap << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveArray[moveIndex+2] = (rookPromCap << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveArray[moveIndex+3] = (queenPromCap << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveIndex += 4;
                        }
                        else if(board.enPassant == toSquare)
                        {
                            moveArray[moveIndex] = (enPassant << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveIndex++;
                        }
                        else
                        {
                            moveArray[moveIndex] = (capture << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                            moveIndex++;
                        }
                    }
                    else if(((1ULL << toSquare) & board.occupancies[(side+1)%2]))
                    {
                        moveArray[moveIndex] = (capture << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveIndex++;
                    }
                    else
                    {
                        moveArray[moveIndex] = (quietMove << 20) + (side << 19) + (piece << 16) + (fromSquare << 8) + toSquare;
                        moveIndex++;
                    }
                }
            }
        }
    }
    if(moveIndex>=256)
    {
        printf("\n--------------------------------------------------------------------------------\n");
        printf("ERROR: NUMBER OF PSEUDO-LEGAL MOVES WAS GREATER THAN EXPECTED (GREATER THAN 255)");
        printf("\n--------------------------------------------------------------------------------\n");
        exit(-1);
    }
    moveArray[moveIndex] = 0;
    return;
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
