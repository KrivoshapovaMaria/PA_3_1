#include <iostream>
#include <fstream>

void writeBitsToFile(std::ostream& outputFile, const unsigned char* bytes, int byteCount, int bitCount, int& currentBytePos, int& bitPos) {
    for (int i = 0; i < byteCount; ++i) {
        unsigned char currentByte = bytes[i];

        while (bitCount > 0) {
            bool bit = ((currentByte >> (7 - bitPos)) & 1) == 1;
            outputFile.seekp(currentBytePos);
            outputFile.write(reinterpret_cast<const char*>(&currentByte), sizeof(currentByte));

            --bitCount;
            ++bitPos;
            currentBytePos = outputFile.tellp();

            if (bitPos > 7) {
                currentBytePos++;
                bitPos = 0;
            }
        }
    }
}

void readBitsFromFile(std::istream& inputFile, unsigned char* bytes, int byteCount, int bitCount) {
    int bitsRead = 0;
    int byteIndex = 0;

    while (bitsRead < bitCount && byteIndex < byteCount) {
        unsigned char currentByte = 0;

        for (int j = 7; j >= 0; --j) {
            unsigned char bit;
            inputFile.read(reinterpret_cast<char*>(&bit), sizeof(unsigned char));

            if (inputFile.eof()) {
                std::cerr << "Unexpected end of file." << std::endl;
                return;
            }

            currentByte |= (bit & 1) << j;
            ++bitsRead;

            if (bitsRead >= bitCount) break;
        }

        bytes[byteIndex++] = currentByte;
    }
}

void printBitSequence(const unsigned char* bytes, int byteCount) {
    std::cout << "Bit sequence: ";
    for (int i = 0; i < byteCount; ++i) {
        std::cout << std::hex << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::endl;
}


#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>

void writeBitsToFile(std::ostream& outputFile, const std::string& bitSequence) {
    int bitCount = bitSequence.size();
    unsigned char currentByte = 0;
    int bitPos = 0;

    for (char bit : bitSequence) {
        if (bit == '1') {
            currentByte |= (1 << (7 - bitPos));
        }

        ++bitPos;

        if (bitPos == 8) {
            outputFile.write(reinterpret_cast<const char*>(&currentByte), sizeof(currentByte));
            currentByte = 0;
            bitPos = 0;
        }
    }

    if (bitPos != 0) {
        outputFile.write(reinterpret_cast<const char*>(&currentByte), sizeof(currentByte));
    }
}

void readBitsFromFile(std::istream& inputFile, int numberOfBits) {
    int numberOfBytes = std::ceil(static_cast<double>(numberOfBits) / 8);
    unsigned char* bytes = new unsigned char[numberOfBytes];
    int byteIndex = 0;
    int bitCount = 0;
    int bitPos = 0;

    while (bitCount < numberOfBits) {
        inputFile.read(reinterpret_cast<char*>(&bytes[byteIndex]), sizeof(unsigned char));
        
        for (int j = 7; j >= 0; --j) {
            bool bit = ((bytes[byteIndex] >> j) & 1) == 1;

            std::cout << bit;

            ++bitCount;
            ++bitPos;

            if (bitCount >= numberOfBits) {
                break;
            }
        }

        ++byteIndex;
    }

    delete[] bytes;
}

int main() {
    std::string fileName;
    char operationChoice;

    std::cout << "Enter 'r' to read or 'w' to write bit sequence: ";
    std::cin >> operationChoice;

    std::cout << "Enter 'n' to create a new file or 'e' to open an existing file: ";
    char fileChoice;
    std::cin >> fileChoice;

    if (fileChoice == 'n') {
        std::cout << "Enter the name of the new file: ";
        std::cin >> fileName;
    }
    else if (fileChoice == 'e') {
        std::cout << "Enter the name of the existing file: ";
        std::cin >> fileName;
    }
    else {
        std::cout << "Invalid choice for file operation." << std::endl;
        return 1;
    }

    if (operationChoice == 'w') {
        std::ofstream outputFile(fileName, std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Unable to open file: " << fileName << std::endl;
            return 1;
        }

        std::string bitSequence;
        std::cout << "Enter the bit sequence to write: ";
        std::cin >> bitSequence;

        writeBitsToFile(outputFile, bitSequence);
        outputFile.close();

        std::cout << "Bit sequence successfully written to the file." << std::endl;
    }
    else if (operationChoice == 'r') {
        std::ifstream inputFile(fileName, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "Unable to open file: " << fileName << std::endl;
            return 1;
        }

        int numberOfBitsToRead;
        std::cout << "Enter the number of bits to read: ";
        std::cin >> numberOfBitsToRead;

        std::cout << "The bits read from the file: ";
        readBitsFromFile(inputFile, numberOfBitsToRead);
        std::cout << std::endl;

        inputFile.close();
    }
    else {
        std::cout << "Invalid choice for operation." << std::endl;
        return 1;
    }

    return 0;
}

