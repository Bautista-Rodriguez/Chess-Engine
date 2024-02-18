#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include "../constsAndEnums.h"

//OPERATIONS
int popCount(U64);
int bitScan(U64);
U64 popLSB(U64 bitboard);

//MODE DECODING

int getToSquare(int chessMove);
int getFromSquare(int chessMove);
int getPiece(int chessMove);
int getSide(int chessMove);
int getTypeMove(int chessMove);

//ATTACKS
U64 maskPawnAttacks(int side, int square);
U64 maskKnightAttacks(int square);
U64 maskKingAttacks(int square);

U64 maskRookRay(int square);
U64 maskBishopRay(int square);
U64 maskQueenRay(int square);

U64 maskRookAttacks(int square, U64 position);
U64 maskBishopAttacks(int square, U64 position);
U64 maskQueenAttacks(int square, U64 position);

U64 occupancyRookAttacks(int square, int number);
U64 occupancyBishopAttacks(int square, int number);

U64 getRookAttacks(int square, U64 occupancy);
U64 getQueenAttacks(int square, U64 occupancy);
U64 getBishopAttacks(int square, U64 occupancy);

void initMaskKingAttacks();
void initMaskKnightAttacks();
void initMaskPawnAttacks();
void initMaskRookRay();
void initMaskBishopRay();
void initMaskQueenRay();
void initRookOccupancyAttacks();
void initBishopOccupancyAttacks();
void initMasks();

#endif // BITBOARD_H_INCLUDED
