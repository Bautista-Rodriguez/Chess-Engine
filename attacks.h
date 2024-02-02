#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED


U64 pawnAttacks[2][64];
U64 knightAttacks[64];
U64 kingAttacks[64];
U64 rookRay[64];
U64 bishopRay[64];
U64 queenRay[64];


U64 maskPawnAttacks(int side, int square);

void initMaskPawnAttacks();

U64 maskKnightAttacks(int);

void initMaskKnightAttacks();

U64 maskKingAttacks(int);

void initMaskKingAttacks();

U64 maskRookRay(int);

void initMaskRookRay();

U64 maskBishopRay(int);
void initMaskBishopRay();

U64 maskQueenRay(int);

void initMaskQueenRay();

U64 maskRookAttacks(int, U64);

U64 maskBishopAttacks(int, U64);

U64 maskQueenAttacks(int, U64);
#endif // ATTACKS_H_INCLUDED
