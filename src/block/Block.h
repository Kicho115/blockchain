#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>
#include "../vote/Vote.h"

class Block {
public:
    Block(int idx, Vote v, std::string prevHash);

    std::string getHash() const;
    std::string getPreviousHash() const;
    Vote getVote() const;
    int getIndex() const; // Added accessor
    void mineBlock(int difficulty);
    std::string calculateHash() const;
    
    // For tampering simulation only
    void setVote(Vote v); 

private:
    int index;
    time_t timestamp;
    Vote voteData;
    std::string previousHash;
    std::string hash;
    long nonce;
};

#endif //BLOCK_H
