#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include "blockchain/Blockchain.h"
#include "vote/Vote.h"

// Candidates for the election
const std::vector<std::string> CANDIDATES = {"Sheinbaum", "Amlo", "Penia", "Obama"};

// Function to simulate a polling station
void pollingStation(int id, Blockchain& blockchain, int numVotes) {
    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 0; i < numVotes; ++i) {
        // Create a random preference list
        std::vector<std::string> preferences = CANDIDATES;
        std::shuffle(preferences.begin(), preferences.end(), g);

        Vote v = {preferences};
        
        // Simulate some delay between votes
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
        
        blockchain.addBlock(v);
        std::cout << "[Station " << id << "] Cast vote: " << v.toString() << std::endl;
    }
}

int main() {
    std::cout << "Initializing Blockchain Election System..." << std::endl;
    
    // Difficulty 2 for faster simulation, 4 or 5 for realistic delay
    Blockchain electionChain(3); 

    int votesPerStation = 3;
    std::vector<std::thread> stations;

    std::cout << "Starting 3 Polling Stations..." << std::endl;

    // Launch 3 threads
    for (int i = 1; i <= 3; ++i) {
        stations.emplace_back(pollingStation, i, std::ref(electionChain), votesPerStation);
    }

    // Wait for all stations to close
    for (auto& station : stations) {
        station.join();
    }

    std::cout << "\nAll votes cast." << std::endl;

    // Validate Chain
    std::cout << "Verifying Chain Integrity... " << (electionChain.isChainValid() ? "VALID" : "INVALID") << std::endl;

    // Count Votes
    std::string winner = electionChain.countVotes();
    std::cout << "Election Winner: " << winner << std::endl;

    // --- TAMPERING SIMULATION ---
    std::cout << "\n!!! SIMULATING ATTACK !!!" << std::endl;
    std::cout << "Tampering with block 2..." << std::endl;
    
    // Create a fake vote
    std::vector<std::string> fakePrefs = {"HACKER", "Alice", "Bob", "Charlie"};
    Vote fakeVote = {fakePrefs};
    
    electionChain.tamperChain(2, fakeVote);

    // Validate Chain Again
    std::cout << "Verifying Chain Integrity after attack... " << std::endl;
    bool isValid = electionChain.isChainValid();
    std::cout << "Chain Status: " << (isValid ? "VALID" : "INVALID (Tampering Detected!)") << std::endl;

    return 0;
}
