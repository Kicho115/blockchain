#ifndef VOTE_H
#define VOTE_H

#include <vector>
#include <string>
#include <sstream>

struct Vote {
    std::vector<std::string> preferences;

    // Serializes the vote to a string for hashing
    std::string toString() const {
        std::stringstream ss;
        for (const auto& pref : preferences) {
            ss << pref << ";";
        }
        return ss.str();
    }
};

#endif //VOTE_H

