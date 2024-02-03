#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "constsAndEnums.h"

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
void initMaskKingAttacks();
void initMaskKnightAttacks();
void initMaskPawnAttacks();
void initMaskRookRay();
void initMaskBishopRay();
void initMaskQueenRay();

U64 getRookAttacks(int square, U64 occupancy);
U64 getQueenAttacks(int square, U64 occupancy);
U64 getBishopAttacks(int square, U64 occupancy);
void initRookOccupancyAttacks();
void initBishopOccupancyAttacks();
#endif // ATTACKS_H_INCLUDED
