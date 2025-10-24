#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>      // Para int64_t
#include <functional>
#include <sstream>
#include <iomanip>      // Para std::setw, std::setfill, std::hex

class Block {
public:
    int64_t timestamp;     // El momento en que se creó el bloque
    std::string data;
    std::string prevBlockHash;
    std::string hash;

    Block(std::string data, std::string prevHash)
        : data(data), prevBlockHash(prevHash) {

        this->timestamp = std::time(nullptr);
        this->hash = this->calculateHash();
    }

    static Block NewGenesisBlock() {
        return Block("Genesis Block", "");
    }

    std::string calculateHash() const {
        std::string headers = this->prevBlockHash + this->data + std::to_string(this->timestamp);

        std::hash<std::string> hasher;
        size_t hashedValue = hasher(headers);

        // Convertir el hash (un size_t) a una cadena hexadecimal
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << hashedValue;
        return ss.str();
    }
};

class Blockchain {
public:
    std::vector<Block> blocks;

    Blockchain() {
        // Añadir el primer bloque (Génesis) a la cadena
        this->blocks.push_back(Block::NewGenesisBlock());
    }

    void AddBlock(std::string data) {
        const Block& prevBlock = this->blocks.back();

        Block newBlock(data, prevBlock.hash);

        this->blocks.push_back(newBlock);
    }
};

int main() {
    Blockchain bc;

    std::cout << "Añadiendo el primer bloque..." << std::endl;
    bc.AddBlock("Enviar 1 BTC a Ivan");

    std::cout << "Añadiendo el segundo bloque..." << std::endl;
    bc.AddBlock("Enviar 2 BTC más a Ivan");

    std::cout << "\nContenido de la Blockchain\n" << std::endl;

    for (const auto& block : bc.blocks) {
        std::cout << "Prev. hash: " << block.prevBlockHash << std::endl;
        std::cout << "Data:       " << block.data << std::endl;
        std::cout << "Hash:       " << block.hash << std::endl;
        std::cout << "====================================" << std::endl;
    }

    return 0;
}