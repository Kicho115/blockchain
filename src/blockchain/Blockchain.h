#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <mutex>
#include "../block/Block.h"
#include <map>

class Blockchain {
public:
    Blockchain(int difficulty);
    
    void addBlock(Vote v);
    bool isChainValid() const;
    void tamperChain(int index, Vote newVote);
    
    // Ranked choice voting logic
    std::string countVotes();

private:
    std::vector<Block> chain;
    int difficulty;
    mutable std::mutex mtx; // Mutable to allow locking in const methods if needed, though usually not for basic getters

    Block getLastBlock() const;
};

#endif //BLOCKCHAIN_H

