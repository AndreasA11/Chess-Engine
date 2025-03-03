#include "magic.hpp"

uint32_t Magic::genRandomUINT32() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned int> distrib(0, UINT32_MAX);
	uint32_t randomNumber = distrib(gen);
	randomNumber = distrib(gen);
    return randomNumber;
}

uint64_t Magic::genRandomUINT64() {
	uint64_t n1 = genRandomUINT32() & 0xFFFF;
    uint64_t n2 = genRandomUINT32() & 0xFFFF;
    uint64_t n3 = genRandomUINT32() & 0xFFFF;
    uint64_t n4 = genRandomUINT32() & 0xFFFF;
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

uint64_t Magic::genMagicNumber() {
    return (genRandomUINT64() & genRandomUINT64() & genRandomUINT64());
}

uint64_t Magic::findMagicNumber(int tile, int relevantBits, RB pieceIn) {
    uint64_t occupancies[4096];
    uint64_t attacks[4096];
    //used attacks
    uint64_t usedAttacks[4096];
    //attack mask
    uint64_t attackMask;
    if(pieceIn == bishop) {
        attackMask = MagicBitboard.maskBishopAttacks(tile);
    } else {
        attackMask = MagicBitboard.maskRookAttacks(tile); 
    }
    
    int occupancyIndices = 1 << relevantBits;
    for(int i = 0; i < occupancyIndices; ++i) {
        occupancies[i] = MagicBitboard.setOccupancy(i, relevantBits, attackMask);
        //init attacks
        if(pieceIn == bishop) {
            attacks[i] = MagicBitboard.BishopOnFly(tile, occupancies[i]);
        } else {
            attacks[i] = MagicBitboard.RookOnFly(tile, occupancies[i]);
        }
    }


    //test magic numbers
    for(int ran = 0; ran < 100000000; ++ran) {
        //gen magic number candidate
        uint64_t magicNumber = genMagicNumber();
        //skip bad numbers
        if(MagicBitboard.countBitsOn((attackMask * magicNumber) & 0xFF00000000000000) < 6) {
            continue;
        }
        //init used attack array
        memset(usedAttacks, ZERO, sizeof(usedAttacks));

        //test for good numbers
        bool fail = false;
        for(int index = 0; index < occupancyIndices; ++index) {
            int magicIndex = (int) ((occupancies[index] * magicNumber) >> (64 - relevantBits));
            if(usedAttacks[magicIndex] == ZERO) {
                usedAttacks[magicIndex] = attacks[index];
            } else if(usedAttacks[magicIndex] != attacks[index]) {
                fail = true;
                break;
            }
        }
        if(!fail) {
            return magicNumber;
        }
    }
    std::cout << "Magic Number fails\n";
    return ZERO;
}


void Magic::initMagicNumbers() {
    std::cout << "Rook magic numbers\n";
    for(int tile = 0; tile < 64; ++tile) {
        //rook magic numbers
        rookMagic[tile] = findMagicNumber(tile, MagicBitboard.rookRelevantBits[tile], rook);
        std::cout << "0x" << std::hex << rookMagic[tile] << "\n";
        
    }

    std::cout << "Bishop magic numbers\n";

    for(int tile = 0; tile < 64; ++tile) {
        //bishop magic numbers
        bishopMagic[tile] = findMagicNumber(tile, MagicBitboard.bishopRelevantBits[tile], bishop);
        std::cout << "0x" << std::hex << bishopMagic[tile] << "\n";
        
    }
    
}