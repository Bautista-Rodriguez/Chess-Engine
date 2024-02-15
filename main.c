#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "constsAndEnums.h"
#include "boardRepresentation.h"
#include "attacks.h"
#include "bitboard.h"
#include "moveGen.h"
#include "perft.h"
#include "makeUnmake.h"

int main()
{
    initMasks();
    perftTest();
    return 0;
}
