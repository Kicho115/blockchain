#include "Blockchain.h"
#include <iostream>
#include <algorithm>
#include <set>

Blockchain::Blockchain(int diff) : difficulty(diff) {
    // Genesis block
    std::vector<std::string> genesisPref = {"Genesis"};
    Vote genesisVote = {genesisPref};
    Block genesis(0, genesisVote, "0");
    // Genesis block doesn't necessarily need to be mined hard for this sim, but consistency is good
    genesis.mineBlock(difficulty);
    chain.push_back(genesis);
}

void Blockchain::addBlock(Vote v) {
    std::lock_guard<std::mutex> lock(mtx);
    
    Block lastBlock = chain.back();
    Block newBlock(chain.size(), v, lastBlock.getHash());
    
    // Mining happens here (simulating work)
    // std::cout << "Mining block " << newBlock.getIndex() << "..." << std::endl;
    newBlock.mineBlock(difficulty);
    chain.push_back(newBlock);
    
    std::cout << "Block " << newBlock.getIndex() << " added to chain. Hash: " << newBlock.getHash() << std::endl;
}

bool Blockchain::isChainValid() const {
    // Iterate from 1 because 0 is Genesis
    for (size_t i = 1; i < chain.size(); ++i) {
        const Block& current = chain[i];
        const Block& previous = chain[i - 1];

        // 1. Check if current block's hash is valid relative to its content
        if (current.getHash() != current.calculateHash()) {
            std::cout << "Invalid Block Hash at index " << i << std::endl;
            return false;
        }

        // 2. Check if previous hash matches
        if (current.getPreviousHash() != previous.getHash()) {
            std::cout << "Invalid Previous Hash at index " << i << std::endl;
            return false;
        }
    }
    return true;
}

void Blockchain::tamperChain(int index, Vote newVote) {
    if (index > 0 && index < chain.size()) {
        chain[index].setVote(newVote);
        std::cout << "Tampered with block " << index << std::endl;
    }
}

// Helper for Ranked Choice
std::string Blockchain::countVotes() {
    std::cout << "\n--- Counting Votes (Ranked Choice) ---" << std::endl;
    
    // Collect all valid votes (excluding genesis)
    std::vector<std::vector<std::string>> ballots;
    std::set<std::string> candidates;
    
    for (size_t i = 1; i < chain.size(); ++i) {
        ballots.push_back(chain[i].getVote().preferences);
        for (const auto& c : chain[i].getVote().preferences) {
            candidates.insert(c);
        }
    }

    if (ballots.empty()) return "No votes cast.";

    std::set<std::string> eliminated;
    
    while (true) {
        std::map<std::string, int> counts;
        
        // Initialize counts for remaining candidates
        for (const auto& c : candidates) {
            if (eliminated.find(c) == eliminated.end()) {
                counts[c] = 0;
            }
        }

        // Count first choices
        int totalValidBallots = 0;
        for (const auto& ballot : ballots) {
            // Find first non-eliminated candidate
            for (const auto& candidate : ballot) {
                if (eliminated.find(candidate) == eliminated.end()) {
                    counts[candidate]++;
                    totalValidBallots++;
                    break;
                }
            }
        }

        if (totalValidBallots == 0) return "No valid votes remaining.";

        // Check for winner
        std::string winner;
        int minVotes = totalValidBallots + 1;
        std::vector<std::string> minCandidates;

        std::cout << "Round stats: ";
        for (const auto& pair : counts) {
            std::cout << pair.first << ":" << pair.second << " ";
            if (pair.second > totalValidBallots / 2) {
                return pair.first; // Winner found (>50%)
            }
            if (pair.second < minVotes) {
                minVotes = pair.second;
                minCandidates.clear();
                minCandidates.push_back(pair.first);
            } else if (pair.second == minVotes) {
                minCandidates.push_back(pair.first);
            }
        }
        std::cout << std::endl;

        // No winner yet, eliminate loser(s)
        // Simplified: eliminate all with minVotes (could result in tie issues, but sufficient for sim)
        if (minCandidates.size() == counts.size()) {
            return "Tie between remaining candidates";
        }

        for (const auto& loser : minCandidates) {
            eliminated.insert(loser);
            std::cout << "Eliminating: " << loser << std::endl;
        }
    }
}

