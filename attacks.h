#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED


U64 pawnAttacks[2][64];
U64 knightAttacks[64];
U64 kingAttacks[64];
U64 rookRay[64];
U64 bishopRay[64];
U64 queenRay[64];


U64 maskPawnAttacks(int, int);
U64 maskKnightAttacks(int);
#endif // ATTACKS_H_INCLUDED
