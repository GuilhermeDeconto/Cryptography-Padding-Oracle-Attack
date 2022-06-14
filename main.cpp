//
// Created by Guilherme Deconto on 5/30/22.
//

#include "main.h"
#include "network.h"

string main::byteToHexStr(vector<char> const & vector) {
    stringstream ss;

    // For each element in vector transform byte to Hex string
    for (auto& el : vector)
        ss << setfill('0') << setw(2) << hex << (0xff & (unsigned int)el);
    return ss.str();
}

vector<char> main::hexToBytes(const string& hex) {
    //Initialize vector
    vector<char> bytes;

    //For each hex element, transform to bytes and append to vector
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }

    //Return vector of bytes
    return bytes;
}

vector<int> main::getPossibleValues() {
    // Ascii lower case + ascii upper case + white spaces + padding
    string values = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ \t\n\r\v\f";
    vector<int> paddingValues = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    vector<int> possibleValues;

    //Append each possible value to end of vector
    for (int i = 0; i <= values.length(); i ++) {
        possibleValues.emplace_back(values[i]);
    }

    //Append each possible padding values to the end of vector
    for (int & paddingValue : paddingValues) {
        possibleValues.emplace_back(paddingValue);
    }

    //Remove element 0
    possibleValues.erase(possibleValues.begin() + 57);

    //Return all possible values
    return possibleValues;
}

vector<char> main::decryptBlock(const vector<char>& previousBlock, const vector<char>& blockToDecrypt, int blockSize) {
    vector<char> decipheredBlock(blockToDecrypt.size());
    vector<int> possibleValues = getPossibleValues();

    //For each byte in block starting from the last
    for (int i = blockSize - 1; i >= 0; i--) {
        //For each possible guess value
        for (auto& value : possibleValues) {
            //We get back the original ciphertext chunk
            vector<char> guessBlock(previousBlock);
            int paddingValue = blockSize - i;

            //If we are at the last byte of a bloc and possibleValue == padding value,
            //we send the original ciphertext.
            if (i == (blockSize - 1) && value == paddingValue) {
                continue;
            }

            //We XOR the values and the padding to the indexes we already guessed
            for (int k = blockSize - 1; k > i; k--) {
                char clearValue = decipheredBlock[k];
                guessBlock[k] ^= clearValue ^ paddingValue;
            }

            //We apply the guess at the current position
            guessBlock[i] ^= value ^ paddingValue;
            vector<char> newCipherText(guessBlock);
            newCipherText.insert(newCipherText.end(), blockToDecrypt.begin(), blockToDecrypt.end());

            //We query the server
            bool isPaddingError = network::queryUrl(main::byteToHexStr(newCipherText));

            //If we got a padding error it means that the guess is correct
            if (isPaddingError) {
                decipheredBlock[i] = value;
                break;
            }
        }
    }

    return decipheredBlock;
}

void main::printProgress(int p, int total, int width) {
    string totalProgress = to_string(total);
    string progress = to_string(p);
    int barWidth = width - totalProgress.size() * 2 - 4;

    if(barWidth > 1) {
        int fillWidth = barWidth * p / total;
        cout << "[";
        for(int i = 0; i < barWidth; ++i)
        {
            char c = ' ';
            if(i < fillWidth || p == total) c = '=';
            else if(i == fillWidth) c = '>';

            cout << c;
        }
        cout << "] ";
    }
    cout << setfill(' ') << setw(totalProgress.size()) << progress << "/" << totalProgress;
    cout << endl;
}

void main::vectorIntoChunks(const vector<char>& cipheredText, vector<vector<char>>& cipherBlocks) {
    vector<char> auxVector;
    int vector = 0;
    int item = 0;

    for (char i : cipheredText){
        auxVector.emplace_back(i);
        vector += 1;
        item += 1;
        if (item == 16) {
            cipherBlocks.emplace_back(auxVector);
            auxVector.clear();
            item = 0;
            vector += 1;
        }
    }
}

int main() {
    auto start = chrono::steady_clock::now();
    int blockSize = 16;
    vector<char> cipheredTextByteArray = main::hexToBytes("f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4");
    vector<vector<char>> cipherBlocks;
    vector<char> auxResult;
    vector<char> result;

    main::vectorIntoChunks(cipheredTextByteArray, cipherBlocks);

    //Block 0 is the IV, so we start at index 1
    for (int i = 1; i <= cipherBlocks.size() - 1; i++) {
        main::printProgress(i * 25, 100, 80);
        auxResult = main::decryptBlock(cipherBlocks[i - 1], cipherBlocks[i], blockSize);
        result.insert(result.end(), auxResult.begin(), auxResult.end());
        cout << string(result.begin(), result.end()) << endl;
    }

    main::printProgress(4 * 25, 100, 80);
    cout << string(result.begin(), result.end()) << endl;
    cout << "-----------" << endl;

    auto end = chrono::steady_clock::now();

    cout << "Elapsed time in nanoseconds: "
         << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
         << " ns" << endl;

    cout << "Elapsed time in microseconds: "
         << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " µs" << endl;

    cout << "Elapsed time in milliseconds: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    cout << "Elapsed time in seconds: "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec";
    return 0;
}