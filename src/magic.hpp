#ifndef MAGIC_H
#define MAGIC_H


#include <random>
#include <stdint.h> // for uint
#include "bitboard.hpp"

//magic bitboard 
//all functions and other things needed to calculate the magic numbers for rooks and bishops go here!   

struct Magic{
    BitBoard MagicBitboard;
    uint64_t rookMagic[64]; //new magic rook numbers can be found and placed here, copy to somewhere else for use
    uint64_t bishopMagic[64]; //new magic bishop numbers can be found and placed here, copy to somewhere else for use
    uint32_t genRandomUINT32(); //get random unsigned 32 bit int
    uint64_t genRandomUINT64(); //get random unsigned 64 bit int
    uint64_t genMagicNumber(); //get a good magic number (low number of high bits)
    uint64_t findMagicNumber(int tile, int relevantBits, RB pieceIn); //find working magic numbers for either rook or bishop
    void initMagicNumbers(); //this is used to find the magic numbers for rook and bishop, will not be run again
};
    
/* 
    note: The magic numbers for rook and bishops are put into the bitboard class as they are needed to generate moves there.
        They have been removed from here after being found and there is no further need for Magic class when running the engine
        This is just here to find the (more/better?) magic numbers (if desired?)     
*/


#endif //MAGIC_H

