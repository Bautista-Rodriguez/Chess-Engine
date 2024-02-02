int popCount(U64 bitboard)
{
    int c = 0;
   for(;bitboard;c++)
       bitboard &= bitboard - 1;
   return c;
}

int bitScan(U64 bitboard)
{
    if(!bitboard)
        return -1;
   return popCount((bitboard & -bitboard) - 1);
}
