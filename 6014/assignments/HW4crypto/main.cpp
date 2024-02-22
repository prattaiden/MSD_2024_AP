#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <bitset>

//a Key is an array of 8 8bits unsigned integers

using Block = std::array<uint8_t, 8>;
using Key = std::array<uint8_t , 8>;
using SubTable = std::vector<uint8_t>;

//3 parts:
//computing secret key on a password of any length
//compute a set of substitution tables that will be used during encryption/decryption
//
Key computeSecretKey(std::string &password){
    Key key = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < password.length(); i++) {
        //xor
        key[i % 8] ^= password[i];
    }
    return key;
}

//todo make the tables
void fisherYates(SubTable &table) {
    for (int i = 0; i < 256; i++) {
        table[i] = i;
    }
    for (int i = 255; i > 0; i--) {
        int swap = rand() % (i + 1);
        std::swap(table[i], table[swap]);
    }
}

//todo encrypt
Block encrypt(std::string &state, const Key &key, const std::vector<SubTable> &subTables) {
    Block encryptedState{};
    //translating the string to the encryptedState array of bytes
    for (int i = 0; i < state.size() && i < encryptedState.size(); i++) {
        encryptedState[i] = static_cast<uint8_t>(state[i]);
    }

    //16 times
    for (int round = 0; round < 16; round++) {

        //XOR state bits(8) with the key bits
        for (int i = 0; i < 8; i++) {
            encryptedState[i] ^= key[i];
        }

        //byte 0 should use table 0, byte 1 should use table 1, etc);
        for (int i = 0; i < 8; i++) {
            encryptedState[i] = subTables[i][encryptedState[i]];
        }

            //getting the most significant BIT from the BYTE at [0] in state - for later
            int MSB = encryptedState[0] >> 7;
            //loop processes the first 7 bytes in the state
            for (int i = 0; i < 7; i++) {
                encryptedState[i] = ((encryptedState[i] << 1) | (encryptedState[i + 1] >> 7));
            }
            encryptedState[7] = ((encryptedState[7] << 1) | MSB);
    }
    return encryptedState;
}

Block decrypt(Block &cipherText, const Key &key, std::vector<SubTable> &subTables){
    Block decryptedState = cipherText;

    for(int round = 0 ; round < 16 ; round++){

    //shifting the bits
        int MSB = decryptedState[7] << 7;
        for (int i = 7; i > 0; i--) {
            decryptedState[i] = ((decryptedState[i] >> 1) | (decryptedState[i - 1] << 7));
        }
        decryptedState[0] = (decryptedState[0] >> 1) | MSB;

        //inverse of last xor
        for(int j = 0; j < 8 ; j++){
            for(int k = 0 ; k < 256; k++){
                if(subTables[j][k] == decryptedState[j]){
                    decryptedState[j] = static_cast<uint8_t>(k);
                    break;
                }
            }
        }
        //reversing XOR operation
        for (int i = 0; i < 8; i++) {
            decryptedState[i] ^= key[i];
        }
    }
    return decryptedState;
}

    int main() {
//    std::srand(static_cast<unsigned int>(std::time(0)));
        std::vector<SubTable> substitutionTables(8, SubTable(256));

        for (SubTable &table: substitutionTables) {
            fisherYates(table);
        }

        std::string password = "longpasswordtestonthis";
        Key key = computeSecretKey(password);

        //todo make different password and try to decrypt my message

        std::cout << "password in binary: ";
        for (char c: password) {
            std::cout << std::bitset<8>(static_cast<uint8_t>(c)) << " ";
        }
        std::cout << std::endl;
        std::cout << "key in binary: ";

        for (uint8_t byte: key) {
            std::cout << std::bitset<8>(byte) << " ";
        }
        std::cout << std::endl;

        std::string inputString = "helloman";
        std::cout << "intput message: " << inputString << std::endl;
        std::cout << "input message in binary: ";

        for (char c: inputString) {
            std::cout << std::bitset<8>(static_cast<uint8_t>(c)) << " ";
        }
        std::cout << std::endl;

        Block encrypted = encrypt(inputString, key, substitutionTables);

        std::cout << "encrypted state of input message: ";
        for (uint8_t byte: encrypted) {
            std::cout << std::bitset<8>(byte) << " ";
        }
        std::cout << std::endl;

        Block decrypted = decrypt(encrypted, key, substitutionTables);

        std::cout << "decrypted input message: ";
        for(uint8_t byte : decrypted){
            std::cout << std::bitset<8>(byte) << " ";
        }
        std::cout << std::endl;

        std::cout << "decrypted message: ";
        for(uint8_t byte : decrypted){
            std::cout << static_cast<char>(byte);
        }


        return 0;
    }