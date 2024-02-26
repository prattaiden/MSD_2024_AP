#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include <bitset>


using Block = std::array<uint8_t, 8>;
using Key = std::array<uint8_t , 8>;
using SubTable = std::vector<uint8_t>;

Key computeSecretKey(std::string &password){
    Key key = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < password.length(); i++) {
        //xor
        key[i % 8] ^= password[i];
    }
    return key;
}

void fisherYates(SubTable &table) {
    for (int i = 0; i < 256; i++) {
        table[i] = i;
    }
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(static_cast<unsigned int>(seed));
    for (int i = 255; i > 0; i--) {
        int swap = std::rand() % (i + 1);
        std::swap(table[i], table[swap]);
    }
}

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
////

void printBinaryKeyOrBlock(Key& key){
    for (uint8_t byte: key) {
        std::cout << std::bitset<8>(byte) << " ";
    }
    std::cout << std::endl;
}

//print helpers:

void printStringInBits(std::string& word){
    for (char c: word) {
        std::cout << std::bitset<8>(static_cast<uint8_t>(c)) << " ";
    }
    std::cout << std::endl;
}

void printBitsToString(Key key){
    for(uint8_t byte : key){
        std::cout << static_cast<char>(byte);
    }
    std::cout << std::endl;
}

void printBitsToStringArray(std::vector<uint8_t>& v){
    for(const auto& byte : v){
        std::cout << static_cast<char>(byte);
    }
    std::cout << std::endl;
}


//helper
void changeOneBit(Block& block, int bitIndex){
    block[bitIndex / 8] ^= (1 << (bitIndex % 8));
}




//---------------------------RC4 class--------------------------------------------//
class RC4{
public:
    explicit RC4(const std::vector<uint8_t> &key);
    uint8_t nextByte();
    void rc4EncrpytDecrypt(std::vector<uint8_t>& data);

private:
    std::vector<uint8_t> state_;
    uint8_t i;
    uint8_t j;
};

RC4::RC4(const std::vector<uint8_t> &key) {
    state_ = std::vector<uint8_t>(256);
    //filling
    for(int e = 0; e < 256; e++){
        state_[e] = e;
    }

    i = 0;
    j = 0;

    //permutation based on key
    for(uint16_t k = 0, e = 0 ; k <256; k++){
        j = (j + state_[k] + key[e % key.size()]) % 256;
        std::swap(state_[k], state_[j]);
        e = (e+1) % key.size();
    }
}

uint8_t RC4::nextByte() {
    i = (i+1) % 256;
    j = (j+state_[i]) % 256;
    std::swap(state_[i],state_[j]);
    return state_[(state_[i] + state_[j])%256];
}


void RC4::rc4EncrpytDecrypt(std::vector<uint8_t> &data) {
    for(size_t k = 0 ; k < data.size(); k++){
        uint8_t nextByteValue = nextByte();
        data[k] ^= nextByteValue;
    }
}

int main() {

    //--------------------------------MY RC4 IMPLEMENTATION-------------------------------------

    //uncomment sections below:


    std::vector<uint8_t> rc4Message = {'h', 'e', 'l','l','o','m','i','s','t','e','r'};
    std::cout << "original rc4 message1: ";
    printBitsToStringArray(rc4Message);

    std::vector<uint8_t> rc4Message2 = {'h', 'i','d','u','d','e'};
    std::cout << "original rc4 message2: ";
    printBitsToStringArray(rc4Message2);


    std::vector<uint8_t> rc4Key = {'g', 'o', 'o', 'd', 'k', 'e', 'y', 'n', 'a', 'm' ,'e'};
    RC4 rc4(rc4Key);

    std::vector<uint8_t> encryptedRC4 = rc4Message;
    rc4.rc4EncrpytDecrypt(encryptedRC4);
    std::cout << "encrypted message1: ";
    printBitsToStringArray(encryptedRC4);

    rc4 = RC4(rc4Key);
    std::vector<uint8_t> decryptedRC4 = encryptedRC4;
    rc4.rc4EncrpytDecrypt(decryptedRC4);
    std::cout << "Decrypted message1 with correct key: ";
    printBitsToStringArray(decryptedRC4);

    std::vector<uint8_t> rc4BadKey = {'b', 'a', 'd', 'k', 'e', 'y'};
    RC4 badRC4Decryption(rc4BadKey);
    std::vector<uint8_t> badKeyDecryptRC4 = encryptedRC4;
    badRC4Decryption.rc4EncrpytDecrypt(badKeyDecryptRC4);
    std::cout << "bad key decrypted message does not reveal plain text: ";
    printBitsToStringArray(badKeyDecryptRC4);


    std::vector<uint8_t> encryptedRC42 = rc4Message2;
    rc4.rc4EncrpytDecrypt(encryptedRC42);
    std::cout << "encrypted second message2: ";
    printBitsToStringArray(encryptedRC42);


    //xor two messages using the same key to see results:
    //reveals some of the messages
    for (size_t i = 0; i < encryptedRC4.size(); ++i) {
        encryptedRC4[i] ^= encryptedRC42[i];
    }
    std::cout << "encrypted message after XOR the two messages: ";
    printBitsToStringArray(encryptedRC4);


    //SALARY SECTION!::

    //modifying a message using a bit flipping attack:
    std::vector<uint8_t> moneyMessage = {'Y', 'o', 'u','r',' ', 's','a','l','a','r','y',' ', 'i','s', ' ', '$', '1','0','0','0'};
    std::cout << "original money message: ";
    printBitsToStringArray(moneyMessage);
    rc4 = RC4(rc4Key); //restating this key before sending the money messages
    std::vector<uint8_t> moneyEncryptionRC4 = moneyMessage;
    rc4.rc4EncrpytDecrypt(moneyEncryptionRC4);
    std::cout << "encrypted money RC4 message: ";
    printBitsToStringArray(moneyEncryptionRC4);


    // Modifying the Cipher Text (bit-flipping attack)
    std::vector<uint8_t> modifiedMoneyMessage = {'9', '9', '9', '9'};

    // XOR the original ciphertext with the XOR of original and modified plaintexts
    for (size_t i = 0; i < modifiedMoneyMessage.size(); i++) {
        moneyEncryptionRC4[i + 16] ^= (moneyMessage[i+ 16] ^ modifiedMoneyMessage[i]);
    }

    std::cout << "Modified encrypted money RC4 message: ";
    printBitsToStringArray(moneyEncryptionRC4);

    // Decrypt the modified ciphertext
    rc4 = RC4(rc4Key); // Reinitialize RC4 with the original key
    std::vector<uint8_t> modifiedDecryptionRC4 = moneyEncryptionRC4;
    rc4.rc4EncrpytDecrypt(modifiedDecryptionRC4);
    std::cout << "Decrypted modified money RC4 message: ";
    printBitsToStringArray(modifiedDecryptionRC4);



    //-------------------------------------MY AES IMPLEMENTATION: -----------------------------------------------//

    //uncomment sections below:

    //setting up the substitution tables:
//    std::vector<SubTable> substitutionTables(8, SubTable(256));
//    for (SubTable &table: substitutionTables) {
//        fisherYates(table);
//    }
//
//    //real password and real key:
//    std::string password = "abcdefgh";
//    Key key = computeSecretKey(password);
//    std::cout << "password in binary: ";
//    printStringInBits(password);
//    std::cout << "key in binary: ";
//    printBinaryKeyOrBlock(key);
//
//
//
//
//    //fake password nad fake key:
//    std::string fakePassword = "fakeword";
//    Key fakeKey = computeSecretKey(fakePassword);
//    std::cout << "fake password in binary: ";
//    printStringInBits(fakePassword);
//    std::cout << "fake key in binary: ";
//    printBinaryKeyOrBlock(fakeKey);
//
//    //message to be sent:
//    std::string inputString = "helloman";
//    std::cout << "intput message: " << inputString << std::endl;
//    std::cout << "input message in binary: ";
//    printStringInBits(inputString);
//
//    //ENCRYPTION:
//    Block encrypted = encrypt(inputString, key, substitutionTables);
//    std::cout << "encrypted state of input message: ";
//    printBinaryKeyOrBlock(encrypted);
//
//    //FLIPPING A BIT:
////    int bitFlip = 4;
////    changeOneBit(encrypted, bitFlip);
////    std::cout << "one bit changed cyphertext: ";
////    printBinaryKeyOrBlock(encrypted);
//
//    //DECRYPTION:
//    Block decrypted = decrypt(encrypted, key, substitutionTables);
//    std::cout << "decrypted input message: ";
//    printBinaryKeyOrBlock(decrypted);
//    std::cout << "decrypted message: ";
//    printBitsToString(decrypted);
//
//
//    //fake password decryption:
//    Block fakeDecryption = decrypt(encrypted, fakeKey, substitutionTables);
//    std::cout << "fake decryption password bits: ";
//    printBinaryKeyOrBlock(fakeDecryption);
//    std::cout << "fake decryption password message: ";
//    printBitsToString(fakeDecryption);

    return 0;
}