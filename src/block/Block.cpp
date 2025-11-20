#include "Block.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <iostream>

Block::Block(int idx, Vote v, std::string prevHash) 
    : index(idx), voteData(v), previousHash(prevHash), nonce(0) {
    timestamp = std::time(nullptr);
    hash = calculateHash();
}

std::string Block::getHash() const {
    return hash;
}

std::string Block::getPreviousHash() const {
    return previousHash;
}

Vote Block::getVote() const {
    return voteData;
}

int Block::getIndex() const {
    return index;
}

void Block::setVote(Vote v) {
    voteData = v;
    // Note: We deliberately do NOT recalculate hash here to simulate a broken chain
    // or if we wanted to try to fake it, we would Recalculate.
    // For the "tamper" requirement, usually we want to show that the hash doesn't match the content anymore.
    // OR we recalculate the hash, but then the NEXT block's previousHash won't match this new hash.
    // Let's just change the data. If we recalculateHash(), the chain breaks at the link.
    // If we don't, the check "calculateHash() == hash" fails for this block.
    // Let's leave the hash as is to show data integrity failure within the block.
}

std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << index << timestamp << voteData.toString() << previousHash << nonce;
    std::string input = ss.str();

    unsigned char hashBuffer[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hashBuffer);

    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hexStream << std::setw(2) << (int)hashBuffer[i];
    }
    return hexStream.str();
}

void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        hash = calculateHash();
    }
    // std::cout << "Block mined: " << hash << std::endl;
}
